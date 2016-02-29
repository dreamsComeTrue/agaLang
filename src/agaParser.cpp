#include <cstdlib>
#include <string>

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
#include "agaASTAssignment.h"

namespace aga
{
//--------------------------------------------------------------------------------

agaParser::agaParser (const std::string &source)
{
    m_Lexer = new agaLexer (source);
}

//--------------------------------------------------------------------------------

agaParser::~agaParser()
{
    if (m_Lexer != NULL)
    {
        delete m_Lexer;
    }
}

//--------------------------------------------------------------------------------

agaASTProgram *agaParser::ParseProgram ()
{
    ReadNextToken();

    agaASTProgram *program = new agaASTProgram ();

    while (m_CurrentToken.GetType() != TokenUnknown && m_CurrentToken.GetType() != TokenEndOfFile)
    {
        if (AcceptToken (TokenIdentifier))
        {
            agaASTNode *assignment = ParseAssignment();

            program->AddStatement (assignment);
        }

        ReadNextToken();
    }

    if (m_CurrentToken.GetType() != TokenEndOfFile)
    {
        throw agaException (UNEXPECTED_TOKEN_EOF, tokenWords[m_CurrentToken.GetType()].word, m_CurrentToken.GetLiteral().c_str());
    }

    return program;
}

//--------------------------------------------------------------------------------

agaASTNode *agaParser::ParseAssignment ()
{
    agaASTNode *assignment = nullptr;

    if (AcceptToken (TokenIdentifier))
    {
        agaToken token = m_CurrentToken;

        ReadNextToken();
        AssertToken (TokenEquals);
        ReadNextToken();

        agaASTExpression *expression = static_cast<agaASTExpression *> (ParseExpression());

        assignment = new agaASTAssignment (token, expression);

        AssertTokens ({TokenComma, TokenEndOfLine, TokenEndOfFile});
    }

    return assignment;
}

//--------------------------------------------------------------------------------

agaASTNode *agaParser::ParseExpression()
{
    return ParseBooleanExpression();
}

//--------------------------------------------------------------------------------

agaASTNode *agaParser::ParseBooleanExpression()
{
    agaASTNode *booleanTerm = ParseBooleanTerm();

    if (AcceptToken (TokenOr))
    {
        agaToken token = m_CurrentToken;

        ReadNextToken();

        agaASTNode *booleanExpression = ParseBooleanExpression ();

        return new agaASTLogicalRelation (token, booleanTerm, booleanExpression);
    }

    return booleanTerm;
}

//--------------------------------------------------------------------------------

agaASTNode *agaParser::ParseBooleanTerm()
{
    agaASTNode *booleanFactor = ParseBooleanFactor();

    if (AcceptToken (TokenAnd))
    {
        agaToken token = m_CurrentToken;

        ReadNextToken();

        agaASTNode *booleanTerm = ParseBooleanTerm ();

        return new agaASTLogicalRelation (token, booleanFactor, booleanTerm);
    }

    return booleanFactor;
}

//--------------------------------------------------------------------------------

agaASTNode *agaParser::ParseBooleanFactor()
{
    if (AcceptToken (TokenNot))
    {
        agaToken token = m_CurrentToken;

        ReadNextToken();

        agaASTNode *booleanRelation = ParseBooleanRelation ();

        return new agaASTLogicalRelation (token, booleanRelation);
    }

    return ParseBooleanRelation ();
}

//--------------------------------------------------------------------------------

agaASTNode *agaParser::ParseBooleanRelation()
{
    agaASTNode *sumExpression = ParseSumExpression();

    if (AcceptToken (TokenSameAs) || AcceptToken (TokenLessEqualThan) || AcceptToken (TokenLessThan) ||
            AcceptToken (TokenGreaterEqualThan) || AcceptToken (TokenGreaterThan))
    {
        agaToken token = m_CurrentToken;

        ReadNextToken();

        agaASTNode *rightExpression = ParseSumExpression ();

        return new agaASTBooleanRelation (token, sumExpression, rightExpression);
    }

    return sumExpression;
}

//--------------------------------------------------------------------------------

//	expression = ["+"|"-"] term {("+"|"-") term} .
agaASTNode *agaParser::ParseSumExpression ()
{
    if (m_CurrentToken.GetType () == TokenIdentifier && m_Lexer->CheckNextToken().GetType () == TokenEquals)
    {
        return ParseAssignment();
    }

    //	Unary plus|minus
    if (m_CurrentToken.GetType () == TokenPlus || m_CurrentToken.GetType () == TokenMinus)
    {
        ReadNextToken();
    }

    agaASTNode *leftTermExpression = ParseTerm ();

    while (m_CurrentToken.GetType () == TokenPlus || m_CurrentToken.GetType () == TokenMinus)
    {
        agaToken binaryOperationToken = m_CurrentToken;

        ReadNextToken();

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

        ReadNextToken();

        agaASTNode *rightFactorExpression  = ParseFactor();

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
        result = new agaASTVariable (m_CurrentToken);
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
                        ReadNextToken();

                        result = ParseExpression ();

                        AssertToken (TokenRightParenthesis);
                    }
                    else
                    {
                        throw agaException (UNEXPECTED_TOKEN, tokenWords[m_CurrentToken.GetType()].word);
                    }
                }
            }
        }
    }

    ReadNextToken();

    return result;
}

//--------------------------------------------------------------------------------

bool agaParser::AssertToken (TokenType tokenToCheck)
{
    if (AcceptToken (tokenToCheck))
    {
        return true;
    }

    throw agaException (UNEXPECTED_TOKEN_EXPECTING, tokenWords[m_CurrentToken.GetType()].word, tokenWords[tokenToCheck].word);

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

    throw agaException (UNEXPECTED_TOKEN_EXPECTING, tokenWords[m_CurrentToken.GetType()].word, tokenWords[*tokens.begin()].word);

    return false;
}

//--------------------------------------------------------------------------------

bool agaParser::AcceptToken (TokenType compareToken)
{
    if (m_CurrentToken.GetType() == TokenUnknown)
    {
        throw agaException (END_OF_TOKEN_STREAM);
    }

    if (m_CurrentToken.GetType() == compareToken)
    {
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------

agaToken agaParser::ReadNextToken()
{
    m_CurrentToken = m_Lexer->GetNextToken();

    return m_CurrentToken;
}

//--------------------------------------------------------------------------------
}

