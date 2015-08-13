#include "agaToken.h"
#include "agaLogger.h"

namespace aga
{
	//--------------------------------------------------------------------------------
	
	std::string TokenNames[] = 
	{
		"Unknown",
		"TokenInteger",
		"TokenFloat",
		"String",
		"LeftParenthesis",
		"RightParenthesis",
		"LeftBracket",
		"RightBracket",
		"LeftBrace",
		"RightBrace",		
		"Identifier",
		"Colon",
		"Semicolon",
		"Plus",
		"Minus",
		"Multiply",
		"Divide",
		"Equals",
		"Pound"
	};
	
	//--------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------
	
	agaToken::agaToken () :
		m_Literal(""),
		m_Type(TokenUnknown),
		m_Line(-1),
		m_BeginColumn(-1),
		m_EndColumn(-1),
		m_Position(-1)
	{		
	}	
	
	//--------------------------------------------------------------------------------

	agaToken::agaToken (const std::string& literal, TokenType type, long line, long beginColumn, long endColumn, long posiiton) :
		m_Literal(literal),
		m_Type(type),
		m_Line(line),
		m_BeginColumn(beginColumn),
		m_EndColumn(endColumn),
		m_Position(posiiton)
	{		
	}
	
	//--------------------------------------------------------------------------------
	
	TokenType agaToken::GetType() const
	{
		return m_Type;
	}

	//--------------------------------------------------------------------------------
	
	const std::string &agaToken::GetLiteral() const
	{
		return m_Literal;
	}

	//--------------------------------------------------------------------------------
	
	void agaToken::Print()
	{
		agaLogger::log(true, "[s: s <d, d:d>]", TokenNames[m_Type].c_str(), m_Literal.c_str(), m_Line, m_BeginColumn, m_EndColumn);
	}
	
	//--------------------------------------------------------------------------------
}