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

    std::shared_ptr<agaASTProgram> agaParser::ParseProgram ()
    {
        std::shared_ptr<agaASTProgram> program = std::shared_ptr<agaASTProgram> (new agaASTProgram ());
        std::shared_ptr<agaASTBlock> globalBlock = std::shared_ptr<agaASTBlock> (new agaASTBlock (nullptr));

        ReadNextToken ();
        while (m_CurrentToken.GetType () != TokenUnknown && m_CurrentToken.GetType () != TokenEndOfFile)
        {
            if (AcceptToken (TokenIdentifier) || AcceptToken (TokenColon))
            {
                std::shared_ptr<agaASTBlock> block = std::static_pointer_cast<agaASTBlock> (ParseBlock (globalBlock));

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

    std::shared_ptr<agaASTNode> agaParser::ParseBlock (std::shared_ptr<agaASTBlock> parentBlock)
    {
        std::shared_ptr<agaASTBlock> block = std::shared_ptr<agaASTBlock> (new agaASTBlock (parentBlock));

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
                std::shared_ptr<agaASTNode> partNode = nullptr;

                if (AcceptToken (TokenIdentifier))
                {
                    if (CheckNextTokens ({TokenColon, TokenEquals}))
                    {
                        partNode = std::shared_ptr<agaASTNode> (ParseAssignment ());
                    }
                    else if (m_Lexer->CheckUntilToken (TokenColon, {TokenIdentifier}))
                    {
                        partNode = std::shared_ptr<agaASTNode> (ParseBlock (block));
                    }
                    else
                    {
                        partNode = std::shared_ptr<agaASTNode> (ParseFunctionCall ());
                    }
                }
                else if (AcceptToken (TokenQuestion))
                {
                    partNode = std::shared_ptr<agaASTNode> (ParseMatch ());
                }
                else
                {
                    ThrowExpecting ({AssignmentNode, BlockNode, FunctionCallNode});
                }

                if (partNode != nullptr)
                {
                    std::shared_ptr<agaASTNode> statement = std::shared_ptr<agaASTNode> (partNode);
                    block->AddStatement (statement);
                }

                ReadEOL ();
            }

            //  Read "="
            AssertToken (TokenEquals);
            ReadNextToken ();

            std::shared_ptr<agaASTNode> returnExpression = std::shared_ptr<agaASTNode> (ParseExpression ());

            block->SetReturnExpr (returnExpression);

            ReadEOL ();
        }

        return block;
    }

    //--------------------------------------------------------------------------------

    /*
     * FunctionName arg1 arg2 (AnotherFunction arg0)
     */
    std::shared_ptr<agaASTNode> agaParser::ParseFunctionCall ()
    {
        std::shared_ptr<agaASTFunctionCall> functionCall = nullptr;

        if (AcceptToken (TokenIdentifier))
        {
            functionCall = std::shared_ptr<agaASTFunctionCall> (new agaASTFunctionCall ());
            functionCall->SetName (m_CurrentToken.GetLiteral ());

            // Read block identifier
            ReadNextToken ();

            TokenType tokenType = m_CurrentToken.GetType ();

            while (tokenType != TokenComma && tokenType != TokenEndOfLine && tokenType != TokenEndOfFile &&
                   tokenType != TokenRightParenthesis && tokenType != TokenEquals)
            {
                std::shared_ptr<agaASTNode> parameter = ParseExpression ();

                functionCall->AddParameter (parameter);

                tokenType = m_CurrentToken.GetType ();
            }
        }

        return functionCall;
    }

    //--------------------------------------------------------------------------------

    /*
     * var := expr [EOL EOF , .]
     */
    std::shared_ptr<agaASTNode> agaParser::ParseAssignment ()
    {
        std::shared_ptr<agaASTAssignment> assignment = nullptr;

        if (AcceptToken (TokenIdentifier) && CheckNextTokens ({TokenColon, TokenEquals}))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();
            AssertTokens ({TokenColon, TokenEquals});
            ReadNextToken ();
            ReadNextToken ();

            std::shared_ptr<agaASTVariable> variable = std::shared_ptr<agaASTVariable> (new agaASTVariable (token));
            std::shared_ptr<agaASTExpression> expression =
                std::shared_ptr<agaASTExpression> (std::static_pointer_cast<agaASTExpression> (ParseExpression ()));

            assignment = std::shared_ptr<agaASTAssignment> (new agaASTAssignment (variable, expression));

            AssertTokens ({TokenComma, TokenEquals, TokenEndOfLine, TokenEndOfFile});

            //  Read optional EOL
            ReadEOL ();
        }

        return assignment;
    }

    //--------------------------------------------------------------------------------

    /*
     * ?expr :
     * |expr expr
     * .
     */
    std::shared_ptr<agaASTNode> agaParser::ParseMatch ()
    {
        std::shared_ptr<agaASTMatch> match = nullptr;

        if (AcceptToken (TokenQuestion))
        {
            ReadNextToken ();

            std::shared_ptr<agaASTExpression> matchExpression = std::static_pointer_cast<agaASTExpression> (ParseExpression ());

            match = std::shared_ptr<agaASTMatch> (new agaASTMatch ());

            match->SetExpression (matchExpression);

            //  Read optional EOL
            ReadEOL ();

            TokenType tokenType = m_CurrentToken.GetType ();

            while (tokenType != TokenEndOfFile && tokenType != TokenEquals)
            {
                std::shared_ptr<agaASTNode> caseBlock = ParseBlock (nullptr);

                match->AddCase (caseBlock);

                tokenType = m_CurrentToken.GetType ();
            }

            ReadEOL ();

            AssertToken (TokenEquals);
            ReadNextToken ();
        }

        return match;
    }

    //--------------------------------------------------------------------------------

    std::shared_ptr<agaASTNode> agaParser::ParseExpression () { return ParseBooleanExpression (); }

    //--------------------------------------------------------------------------------

    std::shared_ptr<agaASTNode> agaParser::ParseBooleanExpression ()
    {
        std::shared_ptr<agaASTNode> booleanTerm = ParseBooleanTerm ();

        if (AcceptToken (TokenOr))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();

            std::shared_ptr<agaASTNode> booleanExpression = ParseBooleanExpression ();

            return std::shared_ptr<agaASTNode> (new agaASTLogicalRelation (token, booleanTerm, booleanExpression));
        }

        return booleanTerm;
    }

    //--------------------------------------------------------------------------------

    std::shared_ptr<agaASTNode> agaParser::ParseBooleanTerm ()
    {
        std::shared_ptr<agaASTNode> booleanFactor = ParseBooleanFactor ();

        if (AcceptToken (TokenAnd))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();

            std::shared_ptr<agaASTNode> booleanTerm = ParseBooleanTerm ();

            return std::shared_ptr<agaASTNode> (new agaASTLogicalRelation (token, booleanFactor, booleanTerm));
        }

        return booleanFactor;
    }

    //--------------------------------------------------------------------------------

    std::shared_ptr<agaASTNode> agaParser::ParseBooleanFactor ()
    {
        if (AcceptToken (TokenNot))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();

            std::shared_ptr<agaASTNode> booleanRelation = ParseBooleanRelation ();

            return std::shared_ptr<agaASTNode> (new agaASTLogicalRelation (token, booleanRelation));
        }

        return ParseBooleanRelation ();
    }

    //--------------------------------------------------------------------------------

    std::shared_ptr<agaASTNode> agaParser::ParseBooleanRelation ()
    {
        std::shared_ptr<agaASTNode> sumExpression = ParseSumExpression ();

        if (AcceptToken (TokenSameAs) || AcceptToken (TokenLessEqualThan) || AcceptToken (TokenLessThan) ||
            AcceptToken (TokenGreaterEqualThan) || AcceptToken (TokenGreaterThan))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();

            std::shared_ptr<agaASTNode> rightExpression = ParseSumExpression ();

            return std::shared_ptr<agaASTNode> (new agaASTBooleanRelation (token, sumExpression, rightExpression));
        }

        return sumExpression;
    }

    //--------------------------------------------------------------------------------

    //	expression = ["+"|"-"] term {("+"|"-") term} .
    std::shared_ptr<agaASTNode> agaParser::ParseSumExpression ()
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

        std::shared_ptr<agaASTNode> leftTermExpression = ParseTerm ();

        while (m_CurrentToken.GetType () == TokenPlus || m_CurrentToken.GetType () == TokenMinus)
        {
            agaToken binaryOperationToken = m_CurrentToken;

            ReadNextToken ();

            std::shared_ptr<agaASTNode> rightTermExpression = ParseTerm ();

            leftTermExpression = std::shared_ptr<agaASTNode> (
                new agaASTBinaryOperator (binaryOperationToken, leftTermExpression, rightTermExpression));
        }

        return leftTermExpression;
    }

    //--------------------------------------------------------------------------------

    //	term = factor {("*"|"/") factor} .
    std::shared_ptr<agaASTNode> agaParser::ParseTerm ()
    {
        std::shared_ptr<agaASTNode> leftFactorExpression = ParseFactor ();

        while (m_CurrentToken.GetType () == TokenMultiply || m_CurrentToken.GetType () == TokenDivide)
        {
            agaToken binaryOperationToken = m_CurrentToken;

            ReadNextToken ();

            std::shared_ptr<agaASTNode> rightFactorExpression = ParseFactor ();

            leftFactorExpression = std::shared_ptr<agaASTNode> (
                new agaASTBinaryOperator (binaryOperationToken, leftFactorExpression, rightFactorExpression));
        }

        return leftFactorExpression;
    }

    //--------------------------------------------------------------------------------

    //	 factor =
    //		 | ident
    //		 | integer
    //		 | float
    //		 | "(" expression ")" .
    std::shared_ptr<agaASTNode> agaParser::ParseFactor ()
    {
        std::shared_ptr<agaASTNode> result;

        if (AcceptToken (TokenIdentifier))
        {
            bool funcStart = CheckNextToken (TokenLeftParenthesis) || CheckNextToken (TokenIdentifier) ||
                             CheckNextToken (TokenInteger) || CheckNextToken (TokenFloat) || CheckNextToken (TokenString);

            if (funcStart)
            {
                result = std::shared_ptr<agaASTNode> (ParseFunctionCall ());
            }
            else
            {
                result = std::shared_ptr<agaASTNode> (new agaASTVariable (m_CurrentToken));
                ReadNextToken ();
            }
        }
        else
        {
            if (AcceptToken (TokenInteger))
            {
                long value = atol (m_CurrentToken.GetLiteral ().c_str ());

                result = std::shared_ptr<agaASTNode> (new agaASTConstant (m_CurrentToken, value));
            }
            else
            {
                if (AcceptToken (TokenFloat))
                {
                    double value = atof (m_CurrentToken.GetLiteral ().c_str ());

                    result = std::shared_ptr<agaASTNode> (new agaASTConstant (m_CurrentToken, value));
                }
                else
                {
                    if (AcceptToken (TokenString))
                    {
                        result = std::shared_ptr<agaASTNode> (new agaASTConstant (m_CurrentToken, m_CurrentToken.GetLiteral ()));
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
