#ifndef _AGA_PARSER_H_
#define _AGA_PARSER_H_

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "agaASTNode.h"
#include "agaToken.h"

namespace aga
{
    class agaLexer;
    class agaASTProgram;
    class agaASTBlock;

    class agaParser
    {
      public:
        agaParser (const std::string &source);
        ~agaParser ();

        std::unique_ptr<agaASTProgram> ParseProgram ();

      private:
        //	Internal
        agaToken ReadNextToken ();

        std::unique_ptr<agaASTNode> ParseBlock ();
        std::unique_ptr<agaASTNode> ParseFunctionCall ();
        std::unique_ptr<agaASTNode> ParseExpression ();
        std::unique_ptr<agaASTNode> ParseMatch ();
        std::unique_ptr<agaASTNode> ParseBooleanExpression ();
        std::unique_ptr<agaASTNode> ParseBooleanTerm ();
        std::unique_ptr<agaASTNode> ParseBooleanFactor ();
        std::unique_ptr<agaASTNode> ParseBooleanRelation ();
        std::unique_ptr<agaASTNode> ParseSumExpression ();
        std::unique_ptr<agaASTNode> ParseAssignment ();
        std::unique_ptr<agaASTNode> ParseTerm ();
        std::unique_ptr<agaASTNode> ParseFactor ();

        bool AcceptToken (TokenType compareToken);
        bool CheckPreviousToken (TokenType compareToken);
        bool CheckNextToken (TokenType compareToken);
        bool CheckNextTokens (std::initializer_list<TokenType> tokens);
        bool AssertToken (TokenType tokenToCheck);
        bool AssertTokens (std::initializer_list<TokenType> tokens);

        void ReadEOL ();
        void ThrowExpecting (std::initializer_list<TokenType> tokens);
        void ThrowExpecting (std::initializer_list<std::string> constructs);
        void ThrowExpecting (std::initializer_list<ASTNodeType> constructs);

      private:
        agaLexer *m_Lexer;
        std::shared_ptr<agaASTNode> m_CurrentBlock;
        agaToken m_LastToken;
        agaToken m_CurrentToken;
    };
}

#endif //	_AGA_PARSER_H_
