#ifndef _AGA_ASTBLOCK_H_
#define _AGA_ASTBLOCK_H_

#include "agaASTNode.h"
#include "agaSymbol.h"

namespace aga
{
    class agaASTBlock : public agaASTNode
    {
      public:
        agaASTBlock (std::shared_ptr<agaASTNode> parentNode) : agaASTNode (BlockNode, parentNode) {}

        void AddStatement (std::unique_ptr<agaASTNode> &node) { m_Statements.push_back (std::move (node)); }

        const std::vector<std::unique_ptr<agaASTNode>> &GetStatements () { return m_Statements; }

        void AddParameter (const std::string &parameter) { m_Parameters.push_back (parameter); }

        const std::vector<std::string> &GetParameters () { return m_Parameters; }

        void SetName (const std::string &name) { m_Name = name; }

        const std::string &GetName () const { return m_Name; }

        void SetReturnExpr (std::unique_ptr<agaASTNode> returnExpr) { m_ReturnExpr = std::move (returnExpr); }

        const std::unique_ptr<agaASTNode> &GetReturnExpr () { return m_ReturnExpr; }

        std::vector<std::shared_ptr<agaSymbol>> &GetSymbols () { return m_Symbols; }

        std::shared_ptr<agaSymbol> GetSymbol (const std::string &name);

        void PutSymbol (const std::string &name, llvm::AllocaInst *value, agaASTNode *parent);

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator) override;

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer) override {}

        virtual const std::string ToString () override;

      private:
        std::string m_Name;
        std::vector<std::string> m_Parameters;
        std::vector<std::unique_ptr<agaASTNode>> m_Statements;
        std::unique_ptr<agaASTNode> m_ReturnExpr;
        std::vector<std::shared_ptr<agaSymbol>> m_Symbols;
    };
}

#endif //	_AGA_ASTBLOCK_H_
