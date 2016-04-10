#include <cstdlib>
#include <sstream>
#include <string>

#include "agaASTAssignment.h"
#include "agaASTBinaryOperator.h"
#include "agaASTBlock.h"
#include "agaASTBooleanRelation.h"
#include "agaASTConstant.h"
#include "agaASTFunctionCall.h"
#include "agaASTLogicalRelation.h"
#include "agaASTMatch.h"
#include "agaASTProgram.h"
#include "agaASTVariable.h"
#include "agaConstants.h"
#include "agaException.h"
#include "agaLexer.h"
#include "agaParser.h"

namespace aga
{
    //--------------------------------------------------------------------------------

    template <class T_SRC, class T_DEST> std::unique_ptr<T_DEST> unique_cast (std::unique_ptr<T_SRC> &&src)
    {
        if (!src)
        {
            return std::unique_ptr<T_DEST> ();
        }

        // Throws a std::bad_cast() if this doesn't work out
        T_DEST *dest_ptr = &dynamic_cast<T_DEST &> (*src.get ());

        src.release ();
        std::unique_ptr<T_DEST> ret (dest_ptr);

        return ret;
    }

    //--------------------------------------------------------------------------------

    agaParser::agaParser (const std::string &source) { m_Lexer = new agaLexer (source); }

    //--------------------------------------------------------------------------------

    agaParser::~agaParser ()
    {
        if (m_Lexer != NULL)
        {
            delete m_Lexer;
        }
    }

    //--------------------------------------------------------------------------------

    std::unique_ptr<agaASTProgram> agaParser::ParseProgram ()
    {
        m_CurrentBlock = std::shared_ptr<agaASTProgram> (new agaASTProgram (nullptr));

        std::unique_ptr<agaASTProgram> program = std::make_unique<agaASTProgram> (m_CurrentBlock);

        ReadNextToken ();
        while (m_CurrentToken.GetType () != TokenUnknown && m_CurrentToken.GetType () != TokenEndOfFile)
        {
            if (AcceptToken (TokenIdentifier) || AcceptToken (TokenColon))
            {
                std::unique_ptr<agaASTBlock> block = unique_cast<agaASTNode, agaASTBlock> (ParseBlock ());

                if (block != nullptr)
                {
                    program->AddBlock (block);
                }
            }
            else
            {
                ThrowExpecting ({nodeWords[ASTNodeType::BlockNode].word});
            }
        }

        if (m_CurrentToken.GetType () != TokenEndOfFile)
        {
            throw agaException (UNEXPECTED_TOKEN_EOF, m_CurrentToken.GetInfo ().c_str (), m_CurrentToken.GetLiteral ().c_str ());
        }

        return program;
    }

    //--------------------------------------------------------------------------------

    std::unique_ptr<agaASTNode> agaParser::ParseBlock ()
    {
        std::shared_ptr<agaASTNode> lastBlock = m_CurrentBlock;

        m_CurrentBlock = std::shared_ptr<agaASTBlock> (new agaASTBlock (lastBlock));
        std::unique_ptr<agaASTBlock> block = std::make_unique<agaASTBlock> (m_CurrentBlock);

        if (AcceptToken (TokenIdentifier) || AcceptToken (TokenColon))
        {
            if (AcceptToken (TokenIdentifier))
            {
                block->SetName (m_CurrentToken.GetLiteral ());

                // Read block identifier
                ReadNextToken ();

                //  Read all optional parameters
                while (!AcceptToken (TokenColon))
                {
                    if (AcceptToken (TokenIdentifier))
                    {
                        block->AddParameter (m_CurrentToken.GetLiteral ());
                    }
                    else
                    {
                        ThrowExpecting ({TokenIdentifier, TokenColon});
                    }

                    ReadNextToken ();
                }
            }

            AssertToken (TokenColon);

            //  Read ":"
            ReadNextToken ();

            //  Read optional EOL
            ReadEOL ();

            while (m_CurrentToken.GetType () != TokenEquals && m_CurrentToken.GetType () != TokenEndOfFile)
            {
                std::unique_ptr<agaASTNode> partNode = nullptr;

                if (AcceptToken (TokenIdentifier))
                {
                    if (CheckNextTokens ({TokenColon, TokenEquals}))
                    {
                        partNode = ParseAssignment ();
                    }
                    else if (m_Lexer->CheckUntilToken (TokenColon, {TokenIdentifier}))
                    {
                        partNode = ParseBlock ();
                    }
                    else
                    {
                        partNode = ParseFunctionCall ();
                    }
                }
                else if (AcceptToken (TokenQuestion))
                {
                    partNode = ParseMatch ();
                }
                else
                {
                    ThrowExpecting ({AssignmentNode, BlockNode, FunctionCallNode});
                }

                if (partNode != nullptr)
                {
                    block->AddStatement (partNode);
                }

                ReadEOL ();
            }

            //  Read "="
            AssertToken (TokenEquals);
            ReadNextToken ();

            //  If we encounter ',' - we end block without result expression,
            //  otherwise - set next expression as a result
            if (AcceptToken (TokenComma))
            {
                AssertToken (TokenComma);
                ReadNextToken ();
            }
            else
            {
                block->SetReturnExpr (ParseExpression ());
            }

            ReadEOL ();
        }

        m_CurrentBlock = lastBlock;

        return std::unique_ptr<agaASTNode> (block.release ());
    }

    //--------------------------------------------------------------------------------

    /*
     * FunctionName arg1 arg2 (AnotherFunction arg0)
     */
    std::unique_ptr<agaASTNode> agaParser::ParseFunctionCall ()
    {
        std::unique_ptr<agaASTFunctionCall> functionCall = nullptr;

        if (AcceptToken (TokenIdentifier))
        {
            functionCall = std::make_unique<agaASTFunctionCall> (m_CurrentBlock);
            functionCall->SetName (m_CurrentToken.GetLiteral ());

            // Read block identifier
            ReadNextToken ();

            TokenType tokenType = m_CurrentToken.GetType ();

            while (tokenType != TokenComma && tokenType != TokenEndOfLine && tokenType != TokenEndOfFile &&
                   tokenType != TokenRightParenthesis && tokenType != TokenEquals)
            {
                functionCall->AddParameter (ParseExpression ());

                tokenType = m_CurrentToken.GetType ();
            }
        }

        return std::unique_ptr<agaASTNode> (functionCall.release ());
    }

    //--------------------------------------------------------------------------------

    /*
     * var := expr [EOL EOF , .]
     */
    std::unique_ptr<agaASTNode> agaParser::ParseAssignment ()
    {
        std::unique_ptr<agaASTAssignment> assignment = nullptr;

        if (AcceptToken (TokenIdentifier) && CheckNextTokens ({TokenColon, TokenEquals}))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();
            AssertTokens ({TokenColon, TokenEquals});
            ReadNextToken ();
            ReadNextToken ();

            std::unique_ptr<agaASTVariable> variable = std::make_unique<agaASTVariable> (token, m_CurrentBlock);
            std::unique_ptr<agaASTExpression> expression = unique_cast<agaASTNode, agaASTExpression> (ParseExpression ());

            assignment = std::make_unique<agaASTAssignment> (variable, expression, m_CurrentBlock);

            AssertTokens ({TokenComma, TokenEquals, TokenEndOfLine, TokenEndOfFile});

            //  Read optional EOL
            ReadEOL ();
        }

        return std::unique_ptr<agaASTNode> (assignment.release ());
    }

    //--------------------------------------------------------------------------------

    /*
     * ?expr :
     * |expr expr
     * .
     */
    std::unique_ptr<agaASTNode> agaParser::ParseMatch ()
    {
        std::unique_ptr<agaASTMatch> match = nullptr;

        if (AcceptToken (TokenQuestion))
        {
            ReadNextToken ();

            std::unique_ptr<agaASTExpression> matchExpression = unique_cast<agaASTNode, agaASTExpression> (ParseExpression ());

            match = std::make_unique<agaASTMatch> (m_CurrentBlock);

            match->SetExpression (matchExpression);

            //  Read optional EOL
            ReadEOL ();

            TokenType tokenType = m_CurrentToken.GetType ();

            while (tokenType != TokenEndOfFile && tokenType != TokenEquals)
            {
                match->AddCase (ParseBlock ());

                tokenType = m_CurrentToken.GetType ();
            }

            ReadEOL ();

            AssertToken (TokenEquals);
            ReadNextToken ();
        }

        return std::unique_ptr<agaASTNode> (match.release ());
    }

    //--------------------------------------------------------------------------------

    std::unique_ptr<agaASTNode> agaParser::ParseExpression () { return ParseBooleanExpression (); }

    //--------------------------------------------------------------------------------

    std::unique_ptr<agaASTNode> agaParser::ParseBooleanExpression ()
    {
        std::unique_ptr<agaASTNode> booleanTerm = ParseBooleanTerm ();

        if (AcceptToken (TokenOr))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();

            std::unique_ptr<agaASTNode> booleanExpression = ParseBooleanExpression ();

            return std::unique_ptr<agaASTNode> (
                new agaASTLogicalRelation (token, std::move (booleanTerm), std::move (booleanExpression), m_CurrentBlock));
        }

        return booleanTerm;
    }

    //--------------------------------------------------------------------------------

    std::unique_ptr<agaASTNode> agaParser::ParseBooleanTerm ()
    {
        std::unique_ptr<agaASTNode> booleanFactor = ParseBooleanFactor ();

        if (AcceptToken (TokenAnd))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();

            std::unique_ptr<agaASTNode> booleanTerm = ParseBooleanTerm ();

            return std::unique_ptr<agaASTNode> (
                new agaASTLogicalRelation (token, std::move (booleanFactor), std::move (booleanTerm), m_CurrentBlock));
        }

        return booleanFactor;
    }

    //--------------------------------------------------------------------------------

    std::unique_ptr<agaASTNode> agaParser::ParseBooleanFactor ()
    {
        if (AcceptToken (TokenNot))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();

            std::unique_ptr<agaASTNode> booleanRelation = ParseBooleanRelation ();

            return std::unique_ptr<agaASTNode> (new agaASTLogicalRelation (token, std::move (booleanRelation), m_CurrentBlock));
        }

        return ParseBooleanRelation ();
    }

    //--------------------------------------------------------------------------------

    std::unique_ptr<agaASTNode> agaParser::ParseBooleanRelation ()
    {
        std::unique_ptr<agaASTNode> sumExpression = ParseSumExpression ();

        if (AcceptToken (TokenSameAs) || AcceptToken (TokenLessEqualThan) || AcceptToken (TokenLessThan) ||
            AcceptToken (TokenGreaterEqualThan) || AcceptToken (TokenGreaterThan))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();

            std::unique_ptr<agaASTNode> rightExpression = ParseSumExpression ();

            return std::unique_ptr<agaASTNode> (
                new agaASTBooleanRelation (token, std::move (sumExpression), std::move (rightExpression), m_CurrentBlock));
        }

        return sumExpression;
    }

    //--------------------------------------------------------------------------------

    //	expression = ["+"|"-"] term {("+"|"-") term} .
    std::unique_ptr<agaASTNode> agaParser::ParseSumExpression ()
    {
        if (m_CurrentToken.GetType () == TokenIdentifier && CheckNextToken (TokenEquals))
        {
            return ParseAssignment ();
        }

        //	Unary plus|minus
        if (m_CurrentToken.GetType () == TokenPlus || m_CurrentToken.GetType () == TokenMinus)
        {
            ReadNextToken ();
        }

        std::unique_ptr<agaASTNode> leftTermExpression = ParseTerm ();

        while (m_CurrentToken.GetType () == TokenPlus || m_CurrentToken.GetType () == TokenMinus)
        {
            agaToken binaryOperationToken = m_CurrentToken;

            ReadNextToken ();

            std::unique_ptr<agaASTNode> rightTermExpression = ParseTerm ();

            leftTermExpression = std::make_unique<agaASTBinaryOperator> (binaryOperationToken, std::move (leftTermExpression),
                                                                         std::move (rightTermExpression), m_CurrentBlock);
        }

        return leftTermExpression;
    }

    //--------------------------------------------------------------------------------

    //	term = factor {("*"|"/") factor} .
    std::unique_ptr<agaASTNode> agaParser::ParseTerm ()
    {
        std::unique_ptr<agaASTNode> leftFactorExpression = ParseFactor ();

        while (m_CurrentToken.GetType () == TokenMultiply || m_CurrentToken.GetType () == TokenDivide)
        {
            agaToken binaryOperationToken = m_CurrentToken;

            ReadNextToken ();

            std::unique_ptr<agaASTNode> rightFactorExpression = ParseFactor ();

            leftFactorExpression = std::make_unique<agaASTBinaryOperator> (binaryOperationToken, std::move (leftFactorExpression),
                                                                           std::move (rightFactorExpression), m_CurrentBlock);
        }

        return leftFactorExpression;
    }

    //--------------------------------------------------------------------------------

    //	 factor =
    //		 | ident
    //		 | integer
    //		 | float
    //		 | "(" expression ")" .
    std::unique_ptr<agaASTNode> agaParser::ParseFactor ()
    {
        std::unique_ptr<agaASTNode> result;

        if (AcceptToken (TokenIdentifier))
        {
            bool funcStart = CheckNextToken (TokenLeftParenthesis) || CheckNextToken (TokenIdentifier) ||
                             CheckNextToken (TokenInteger) || CheckNextToken (TokenFloat) || CheckNextToken (TokenString);

            if (funcStart)
            {
                result = ParseFunctionCall ();
            }
            else
            {
                result = std::make_unique<agaASTVariable> (m_CurrentToken, m_CurrentBlock);
                ReadNextToken ();
            }
        }
        else
        {
            if (AcceptToken (TokenInteger))
            {
                long value = atol (m_CurrentToken.GetLiteral ().c_str ());

                result = std::make_unique<agaASTConstant> (m_CurrentToken, value, m_CurrentBlock);
            }
            else
            {
                if (AcceptToken (TokenFloat))
                {
                    double value = atof (m_CurrentToken.GetLiteral ().c_str ());

                    result = std::make_unique<agaASTConstant> (m_CurrentToken, value, m_CurrentBlock);
                }
                else
                {
                    if (AcceptToken (TokenString))
                    {
                        result = std::make_unique<agaASTConstant> (m_CurrentToken, m_CurrentToken.GetLiteral (), m_CurrentBlock);
                    }
                    else
                    {
                        if (AcceptToken (TokenLeftParenthesis))
                        {
                            ReadNextToken ();

                            result = ParseExpression ();

                            AssertToken (TokenRightParenthesis);
                        }
                        else
                        {
                            throw agaException (UNEXPECTED_TOKEN, m_CurrentToken.GetInfo ().c_str ());
                        }
                    }
                }
            }

            ReadNextToken ();
        }

        return result;
    }

    //--------------------------------------------------------------------------------

    bool agaParser::AssertToken (TokenType tokenToCheck)
    {
        if (AcceptToken (tokenToCheck))
        {
            return true;
        }

        ThrowExpecting ({tokenToCheck});

        return false;
    }

    //--------------------------------------------------------------------------------

    bool agaParser::AssertTokens (std::initializer_list<TokenType> tokens)
    {
        for (auto tokenType : tokens)
        {
            if (AcceptToken (tokenType))
            {
                return true;
            }
        }

        ThrowExpecting (tokens);

        return false;
    }

    //--------------------------------------------------------------------------------

    void agaParser::ThrowExpecting (std::initializer_list<TokenType> tokens)
    {
        std::stringstream sstream;

        for (auto tokenType : tokens)
        {
            sstream << tokenWords[tokenType].word << " ";
        }

        throw agaException (UNEXPECTED_TOKEN_EXPECTING, m_CurrentToken.GetInfo ().c_str (), sstream.str ().c_str ());
    }

    //--------------------------------------------------------------------------------

    void agaParser::ThrowExpecting (std::initializer_list<std::string> constructs)
    {
        std::stringstream sstream;

        for (const std::string &part : constructs)
        {
            sstream << part << " ";
        }

        throw agaException (UNEXPECTED_TOKEN_EXPECTING, m_CurrentToken.GetInfo ().c_str (), sstream.str ().c_str ());
    }

    //--------------------------------------------------------------------------------

    void agaParser::ThrowExpecting (std::initializer_list<ASTNodeType> constructs)
    {
        std::stringstream sstream;

        for (const ASTNodeType &part : constructs)
        {
            sstream << nodeWords[part].word << " ";
        }

        throw agaException (UNEXPECTED_TOKEN_EXPECTING, m_CurrentToken.GetInfo ().c_str (), sstream.str ().c_str ());
    }

    //--------------------------------------------------------------------------------

    bool agaParser::AcceptToken (TokenType compareToken)
    {
        if (m_CurrentToken.GetType () == TokenUnknown)
        {
            throw agaException (END_OF_TOKEN_STREAM);
        }

        if (m_CurrentToken.GetType () == compareToken)
        {
            return true;
        }

        return false;
    }

    //--------------------------------------------------------------------------------

    bool agaParser::CheckNextToken (TokenType compareToken)
    {
        if (m_Lexer->CheckNextToken ().GetType () == compareToken)
        {
            return true;
        }

        return false;
    }

    //--------------------------------------------------------------------------------

    bool agaParser::CheckNextTokens (std::initializer_list<TokenType> tokens)
    {
        int i = 1;
        for (auto token : tokens)
        {
            if (m_Lexer->CheckNextToken (i).GetType () != token)
            {
                return false;
            }

            ++i;
        }

        return true;
    }

    //--------------------------------------------------------------------------------

    bool agaParser::CheckPreviousToken (TokenType compareToken)
    {
        if (m_LastToken.GetType () == compareToken)
        {
            return true;
        }

        return false;
    }

    //--------------------------------------------------------------------------------

    agaToken agaParser::ReadNextToken ()
    {
        m_LastToken = m_CurrentToken;
        m_CurrentToken = m_Lexer->GetNextToken ();

        return m_CurrentToken;
    }

    //--------------------------------------------------------------------------------

    void agaParser::ReadEOL ()
    {
        if (AcceptToken (TokenEndOfLine) || AcceptToken (TokenComma))
        {
            while (m_CurrentToken.GetType () == TokenEndOfLine || m_CurrentToken.GetType () == TokenComma)
            {
                ReadNextToken ();
            }
        }
    }

    //--------------------------------------------------------------------------------
}
