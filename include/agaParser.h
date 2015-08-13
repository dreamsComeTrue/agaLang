#ifndef _AGA_PARSER_H_
#define _AGA_PARSER_H_

#include <string>

#include "agaToken.h"
#include "agaASTExpression.h"

namespace aga
{
	class agaLexer;
	
	class agaParser
	{
	public:
		agaParser (const std::string& source);
		~agaParser ();
		
		agaASTExpression* Parse ();
		
	private:
		void ParseAssignment ();
		
		//	Internal
		agaASTExpression* ParseExpression ();
		agaASTExpression* ParseTerm ();
		agaASTExpression* ParseFactor ();				
		
		bool AcceptToken (TokenType compareToken);
		bool AssertToken (TokenType tokenToCheck);
		
	private:
		agaLexer*	m_Lexer;
		agaToken	m_CurrentToken;
	};
}

#endif	//	_AGA_PARSER_H_