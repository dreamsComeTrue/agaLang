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
    class agaTypeInfo;

    template <class T_SRC, class T_DEST> std::unique_ptr<T_DEST> unique_cast (std::unique_ptr<T_SRC> &&src);

    class agaParser
    {
      public:
        agaParser (const std::string &source);
        ~agaParser ();

        std::shared_ptr<agaASTProgram> ParseProgram ();

      private:
        //	Internal
        agaToken ReadNextToken ();

        std::shared_ptr<agaASTNode> ParseBlock ();
        std::shared_ptr<agaASTBlockParameter> ParseBlockParameter ();
        agaTypeInfo ParseTypeInfo ();
        std::shared_ptr<agaASTNode> ParseFunctionCall ();
        std::shared_ptr<agaASTNode> ParseExpression ();
        std::shared_ptr<agaASTNode> ParseMatch ();
        std::shared_ptr<agaASTNode> ParseBooleanExpression ();
        std::shared_ptr<agaASTNode> ParseBooleanTerm ();
        std::shared_ptr<agaASTNode> ParseBooleanFactor ();
        std::shared_ptr<agaASTNode> ParseBooleanRelation ();
        std::shared_ptr<agaASTNode> ParseSumExpression ();
        std::shared_ptr<agaASTNode> ParseAssignment ();
        std::shared_ptr<agaASTNode> ParseTerm ();
        std::shared_ptr<agaASTNode> ParseFactor ();
        std::shared_ptr<agaASTNode> ParseVariableDefinition ();

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
