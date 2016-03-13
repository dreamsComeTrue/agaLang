#include <cstdlib>
#include <string>
#include <sstream>

#include "agaConstants.h"
#include "agaParser.h"
#include "agaLexer.h"
#include "agaException.h"
#include "agaASTConstant.h"
#include "agaASTVariable.h"
#include "agaASTBinaryOperator.h"
#include "agaASTBooleanRelation.h"
#include "agaASTLogicalRelation.h"
#include "agaASTProgram.h"
#include "agaASTBlock.h"
#include "agaASTAssignment.h"
#include "agaASTFunctionCall.h"
#include "agaASTMatch.h"

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

    agaASTProgram *agaParser::ParseProgram ()
    {
        agaASTProgram *program = new agaASTProgram ();
        agaASTBlock *globalBlock = new agaASTBlock (nullptr);

        ReadNextToken ();
        while (m_CurrentToken.GetType () != TokenUnknown && m_CurrentToken.GetType () != TokenEndOfFile)
        {
            if (AcceptToken (TokenIdentifier) || AcceptToken (TokenColon))
            {
                agaASTBlock *block = static_cast<agaASTBlock *> (ParseBlock (globalBlock));

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

    agaASTNode *agaParser::ParseBlock (agaASTBlock *parentBlock)
    {
        agaASTBlock *block = new agaASTBlock (parentBlock);

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

            while (m_CurrentToken.GetType () != TokenDot && m_CurrentToken.GetType () != TokenEndOfFile)
            {
                agaASTNode *partNode = nullptr;

                if (AcceptToken (TokenIdentifier))
                {
                    if (CheckNextToken (TokenEquals))
                    {
                        partNode = ParseAssignment ();
                    }
                    else if (m_Lexer->CheckUntilToken (TokenColon, {TokenIdentifier}))
                    {
                        partNode = ParseBlock (block);
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

            AssertToken (TokenDot);

            //  Read "."
            ReadNextToken ();
            ReadEOL ();
        }

        return block;
    }

    //--------------------------------------------------------------------------------

    /*
     * FunctionName arg1 arg2 (AnotherFunction arg0)
     */
    agaASTNode *agaParser::ParseFunctionCall ()
    {
        agaASTFunctionCall *functionCall = nullptr;

        if (AcceptToken (TokenIdentifier))
        {
            functionCall = new agaASTFunctionCall ();
            functionCall->SetName (m_CurrentToken.GetLiteral ());

            // Read block identifier
            ReadNextToken ();

            TokenType tokenType = m_CurrentToken.GetType ();

            while (tokenType != TokenComma && tokenType != TokenEndOfLine && tokenType != TokenEndOfFile &&
                   tokenType != TokenRightParenthesis && tokenType != TokenDot)
            {
                agaASTNode *parameter = ParseExpression ();

                functionCall->AddParameter (parameter);

                tokenType = m_CurrentToken.GetType ();
            }
        }

        return functionCall;
    }

    //--------------------------------------------------------------------------------

    /*
     * var = expr [EOL EOF , .]
     */
    agaASTNode *agaParser::ParseAssignment ()
    {
        agaASTNode *assignment = nullptr;

        if (AcceptToken (TokenIdentifier) && CheckNextToken (TokenEquals))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();
            AssertToken (TokenEquals);
            ReadNextToken ();

            agaASTExpression *expression = static_cast<agaASTExpression *> (ParseExpression ());

            assignment = new agaASTAssignment (token, expression);

            AssertTokens ({TokenComma, TokenDot, TokenEndOfLine, TokenEndOfFile});

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
    agaASTNode *agaParser::ParseMatch ()
    {
        agaASTMatch *match = nullptr;

        if (AcceptToken (TokenQuestion))
        {
            ReadNextToken ();

            agaASTExpression *matchExpression = static_cast<agaASTExpression *> (ParseExpression ());

            match = new agaASTMatch ();

            match->SetExpression (matchExpression);

            //  Read optional EOL
            ReadEOL ();

            TokenType tokenType = m_CurrentToken.GetType ();

            while (tokenType != TokenEndOfFile && tokenType != TokenDot)
            {
                agaASTNode *caseBlock = ParseBlock (nullptr);

                match->AddCase (caseBlock);

                tokenType = m_CurrentToken.GetType ();
            }

            ReadEOL ();

            AssertToken (TokenDot);
            ReadNextToken ();
        }

        return match;
    }

    //--------------------------------------------------------------------------------

    agaASTNode *agaParser::ParseExpression () { return ParseBooleanExpression (); }

    //--------------------------------------------------------------------------------

    agaASTNode *agaParser::ParseBooleanExpression ()
    {
        agaASTNode *booleanTerm = ParseBooleanTerm ();

        if (AcceptToken (TokenOr))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();

            agaASTNode *booleanExpression = ParseBooleanExpression ();

            return new agaASTLogicalRelation (token, booleanTerm, booleanExpression);
        }

        return booleanTerm;
    }

    //--------------------------------------------------------------------------------

    agaASTNode *agaParser::ParseBooleanTerm ()
    {
        agaASTNode *booleanFactor = ParseBooleanFactor ();

        if (AcceptToken (TokenAnd))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();

            agaASTNode *booleanTerm = ParseBooleanTerm ();

            return new agaASTLogicalRelation (token, booleanFactor, booleanTerm);
        }

        return booleanFactor;
    }

    //--------------------------------------------------------------------------------

    agaASTNode *agaParser::ParseBooleanFactor ()
    {
        if (AcceptToken (TokenNot))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();

            agaASTNode *booleanRelation = ParseBooleanRelation ();

            return new agaASTLogicalRelation (token, booleanRelation);
        }

        return ParseBooleanRelation ();
    }

    //--------------------------------------------------------------------------------

    agaASTNode *agaParser::ParseBooleanRelation ()
    {
        agaASTNode *sumExpression = ParseSumExpression ();

        if (AcceptToken (TokenSameAs) || AcceptToken (TokenLessEqualThan) || AcceptToken (TokenLessThan) ||
            AcceptToken (TokenGreaterEqualThan) || AcceptToken (TokenGreaterThan))
        {
            agaToken token = m_CurrentToken;

            ReadNextToken ();

            agaASTNode *rightExpression = ParseSumExpression ();

            return new agaASTBooleanRelation (token, sumExpression, rightExpression);
        }

        return sumExpression;
    }

    //--------------------------------------------------------------------------------

    //	expression = ["+"|"-"] term {("+"|"-") term} .
    agaASTNode *agaParser::ParseSumExpression ()
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

        agaASTNode *leftTermExpression = ParseTerm ();

        while (m_CurrentToken.GetType () == TokenPlus || m_CurrentToken.GetType () == TokenMinus)
        {
            agaToken binaryOperationToken = m_CurrentToken;

            ReadNextToken ();

            agaASTNode *rightTermExpression = ParseTerm ();

            leftTermExpression = new agaASTBinaryOperator (binaryOperationToken, leftTermExpression, rightTermExpression);
        }

        return leftTermExpression;
    }

    //--------------------------------------------------------------------------------

    //	term = factor {("*"|"/") factor} .
    agaASTNode *agaParser::ParseTerm ()
    {
        agaASTNode *leftFactorExpression = ParseFactor ();

        while (m_CurrentToken.GetType () == TokenMultiply || m_CurrentToken.GetType () == TokenDivide)
        {
            agaToken binaryOperationToken = m_CurrentToken;

            ReadNextToken ();

            agaASTNode *rightFactorExpression = ParseFactor ();

            leftFactorExpression = new agaASTBinaryOperator (binaryOperationToken, leftFactorExpression, rightFactorExpression);
        }

        return leftFactorExpression;
    }

    //--------------------------------------------------------------------------------

    //	 factor =
    //		 | ident
    //		 | integer
    //		 | float
    //		 | "(" expression ")" .
    agaASTNode *agaParser::ParseFactor ()
    {
        agaASTNode *result;

        if (AcceptToken (TokenIdentifier))
        {
            bool varInsideParenths =
                (CheckPreviousToken (TokenLeftParenthesis) && CheckNextToken (TokenRightParenthesis)); // ( x )
            bool varWithoutParenths =
                (!CheckPreviousToken (TokenLeftParenthesis) && !CheckNextToken (TokenRightParenthesis)); // x
            bool varStartsWithParenths = (CheckPreviousToken (TokenLeftParenthesis) && !CheckNextToken (TokenIdentifier) &&
                                          !CheckNextToken (TokenInteger) && !CheckNextToken (TokenFloat)); // ( x >

            if (varInsideParenths || varWithoutParenths || varStartsWithParenths)
            {
                result = new agaASTVariable (m_CurrentToken);
                ReadNextToken ();
            }
            else
            {
                result = ParseFunctionCall ();
            }
        }
        else
        {
            if (AcceptToken (TokenInteger))
            {
                long value = atol (m_CurrentToken.GetLiteral ().c_str ());

                result = new agaASTConstant (m_CurrentToken, value);
            }
            else
            {
                if (AcceptToken (TokenFloat))
                {
                    double value = atof (m_CurrentToken.GetLiteral ().c_str ());

                    result = new agaASTConstant (m_CurrentToken, value);
                }
                else
                {
                    if (AcceptToken (TokenString))
                    {
                        result = new agaASTConstant (m_CurrentToken, m_CurrentToken.GetLiteral ());
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
