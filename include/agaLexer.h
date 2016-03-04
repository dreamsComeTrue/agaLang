#ifndef _AGA_LEXER_H_
#define _AGA_LEXER_H_

#include <string>

#include "agaToken.h"

namespace aga
{
	class agaLexer
	{
	public:
		agaLexer (const std::string& source);
		
		agaToken GetNextToken ();
		agaToken CheckNextToken ();
        bool CheckUntilToken (TokenType checkToken, std::initializer_list<TokenType> beforeTokens);
		bool HasMoreTokens ();
		
	private:
		bool isWhiteSpace (char character);
		bool isAlpha (char character);
		bool isAlphaNumeric (char character);
		bool isDigit (char character);
		
		char GetNextCharacter ();
		char CheckNextCharacter ();
		
		std::string	m_Source;
		long		m_CurrentLine;
		long		m_CurrentColumn;
		long		m_CurrentPosition;
		char		m_CurrentCharacter;
	};
}

#endif	//	_AGA_LEXER_H_
