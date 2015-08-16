#ifndef _AGA_PARSER_H_
#define _AGA_PARSER_H_

#include <string>

#include "agaToken.h"

namespace aga
{
	class agaLexer;
	class agaASTNode;
	class agaASTProgram;
	
	class agaParser
	{
	public:
		agaParser (const std::string& source);
		~agaParser ();
		
		agaASTProgram* ParseProgram ();
		
	private:
		//	Internal
		agaToken ReadNextToken ();
		agaASTNode* ParseExpression ();
		agaASTNode* ParseAssignment ();
		agaASTNode* ParseTerm ();
		agaASTNode* ParseFactor ();				
		
		bool AcceptToken (TokenType compareToken);
		bool AssertToken (TokenType tokenToCheck);
		
	private:
		agaLexer*	m_Lexer;
		agaToken	m_CurrentToken;
	};
}

#endif	//	_AGA_PARSER_H_