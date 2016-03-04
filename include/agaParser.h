#ifndef _AGA_PARSER_H_
#define _AGA_PARSER_H_

#include <string>
#include <vector>
#include <initializer_list>

#include "agaToken.h"
#include "agaASTNode.h"

namespace aga
{
    class agaLexer;
    class agaASTProgram;
    class agaASTBlock;

    class agaParser
    {
    public:
        agaParser (const std::string& source);
        ~agaParser ();

        agaASTProgram* ParseProgram ();

    private:
        //	Internal
        agaToken ReadNextToken ();

        agaASTNode* ParseBlock (agaASTBlock* parentBlock);
        agaASTNode* ParseFunctionCall ();
        agaASTNode* ParseExpression ();
        agaASTNode* ParseBooleanExpression ();
        agaASTNode* ParseBooleanTerm ();
        agaASTNode* ParseBooleanFactor ();
        agaASTNode* ParseBooleanRelation ();
        agaASTNode* ParseSumExpression ();
        agaASTNode* ParseAssignment ();
        agaASTNode* ParseTerm ();
        agaASTNode* ParseFactor ();

        bool AcceptToken (TokenType compareToken);
        bool CheckPreviousToken (TokenType compareToken);
        bool CheckNextToken (TokenType compareToken);
        bool AssertToken (TokenType tokenToCheck);
        bool AssertTokens (std::initializer_list<TokenType> tokens);

        void ReadEOL ();
        void ThrowExpecting (std::initializer_list<TokenType> tokens);
        void ThrowExpecting (std::initializer_list<std::string> constructs);
        void ThrowExpecting (std::initializer_list<ASTNodeType> constructs);

    private:
        agaLexer*	m_Lexer;
        agaToken	m_LastToken;
        agaToken	m_CurrentToken;
    };
}

#endif	//	_AGA_PARSER_H_
