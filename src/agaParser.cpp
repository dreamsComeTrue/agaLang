#include <cstdlib>
#include <string>

#include "agaConstants.h"
#include "agaParser.h"
#include "agaLexer.h"
#include "agaLogger.h"
#include "agaException.h"
#include "agaASTConstant.h"
#include "agaASTVariable.h"
#include "agaASTBinaryOperator.h"

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

	agaASTExpression *agaParser::Parse()
	{
		m_CurrentToken = m_Lexer->GetNextToken();

		agaASTExpression *expression = ParseExpression();

		if (m_CurrentToken.GetType() != TokenUnknown)
		{
			throw agaException (UNEXPECTED_TOKEN_EOF, tokenWords[m_CurrentToken.GetType()].word, m_CurrentToken.GetLiteral().c_str());
		}

		return expression;
	}

	//--------------------------------------------------------------------------------

	void agaParser::ParseAssignment ()
	{

	}

	//--------------------------------------------------------------------------------

	//	expression = ["+"|"-"] term {("+"|"-") term} .
	agaASTExpression *agaParser::ParseExpression ()
	{
		//	Unary plus|minus
		if (m_CurrentToken.GetType () == TokenPlus || m_CurrentToken.GetType () == TokenMinus)
		{
			m_CurrentToken = m_Lexer->GetNextToken();
		}

		agaASTExpression *leftTermExpression = ParseTerm ();

		while (m_CurrentToken.GetType () == TokenPlus || m_CurrentToken.GetType () == TokenMinus)
		{
			char op = m_CurrentToken.GetLiteral().at (0);

			m_CurrentToken = m_Lexer->GetNextToken();

			agaASTExpression *rightTermExpression = ParseTerm ();

			leftTermExpression = new agaASTBinaryOperator (op, leftTermExpression, rightTermExpression);
		}

		return leftTermExpression;
	}

	//--------------------------------------------------------------------------------

	//	term = factor {("*"|"/") factor} .
	agaASTExpression *agaParser::ParseTerm ()
	{
		agaASTExpression *leftFactorExpression = ParseFactor ();

		while (m_CurrentToken.GetType () == TokenMultiply || m_CurrentToken.GetType () == TokenDivide)
		{
			char op = m_CurrentToken.GetLiteral().at (0);

			m_CurrentToken = m_Lexer->GetNextToken();

			agaASTExpression *rightFactorExpression  = ParseFactor();

			leftFactorExpression = new agaASTBinaryOperator (op, leftFactorExpression, rightFactorExpression);
		}

		return leftFactorExpression;
	}

	//--------------------------------------------------------------------------------

	//	 factor =
	//		 | ident
	//		 | integer
	//		 | float
	//		 | "(" expression ")" .
	agaASTExpression *agaParser::ParseFactor ()
	{
		agaASTExpression *result;

		if (AcceptToken (TokenIdentifier))
		{
			result = new agaASTVariable (m_CurrentToken);
		}
		else
			if (AcceptToken (TokenInteger))
			{
				long value = atol (m_CurrentToken.GetLiteral ().c_str ());

				result = new agaASTConstant (m_CurrentToken, value);
			}
			else
				if (AcceptToken (TokenFloat))
				{
					double value = atof (m_CurrentToken.GetLiteral ().c_str ());

					result = new agaASTConstant (m_CurrentToken, value);
				}
				else
					if (AcceptToken (TokenLeftParenthesis))
					{
						m_CurrentToken = m_Lexer->GetNextToken ();

						result = ParseExpression ();

						AssertToken (TokenRightParenthesis);
					}
					else
					{
						throw agaException (UNEXPECTED_TOKEN, tokenWords[m_CurrentToken.GetType()].word);
					}

		m_CurrentToken = m_Lexer->GetNextToken ();

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
}

