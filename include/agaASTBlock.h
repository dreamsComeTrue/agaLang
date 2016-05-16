#ifndef _AGA_ASTBLOCK_H_
#define _AGA_ASTBLOCK_H_

#include "agaASTNode.h"
#include "agaSymbol.h"
#include "agaTypeInfo.h"

namespace aga
{
    class agaASTBlockParameter;

    class agaASTBlock : public agaASTNode
    {
      public:
        agaASTBlock (std::shared_ptr<agaASTNode> parentNode) : agaASTNode (BlockNode, parentNode) {}

        void AddStatement (std::shared_ptr<agaASTNode> &node) { m_Statements.push_back (node); }

        const std::vector<std::shared_ptr<agaASTNode>> &GetStatements () { return m_Statements; }

        void AddParameter (std::shared_ptr<agaASTBlockParameter> &parameter) { m_Parameters.push_back (parameter); }

        const std::vector<std::shared_ptr<agaASTBlockParameter>> &GetParameters () { return m_Parameters; }

        std::shared_ptr<agaASTBlockParameter> GetParameter (const std::string &name);

        void SetName (const std::string &name) { m_Name = name; }

        const std::string &GetName () const { return m_Name; }

        void SetReturnType (agaTypeInfo &typeInfo) { m_TypeInfo = typeInfo; }

        agaTypeInfo &GetReturnType () { return m_TypeInfo; }

        void SetReturnExpr (std::shared_ptr<agaASTNode> returnExpr) { m_ReturnExpr = returnExpr; }

        const std::shared_ptr<agaASTNode> &GetReturnExpr () { return m_ReturnExpr; }

        std::vector<std::shared_ptr<agaSymbol>> &GetSymbols () { return m_Symbols; }

        std::shared_ptr<agaSymbol> GetSymbol (const std::string &name);

        void PutSymbol (const std::string &name, agaASTNode *node, llvm::AllocaInst *value, agaASTNode *parent);

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator) override;

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer) override {}

        virtual const std::string ToString () override;

      private:
        std::string m_Name;
        agaTypeInfo m_TypeInfo;
        std::vector<std::shared_ptr<agaASTBlockParameter>> m_Parameters;
        std::vector<std::shared_ptr<agaASTNode>> m_Statements;
        std::shared_ptr<agaASTNode> m_ReturnExpr;
        std::vector<std::shared_ptr<agaSymbol>> m_Symbols;
    };
}

#endif //	_AGA_ASTBLOCK_H_
