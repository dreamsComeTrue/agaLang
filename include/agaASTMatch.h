#ifndef _AGA_ASTMATCH_H_
#define _AGA_ASTMATCH_H_

#include <vector>

#include "agaASTExpression.h"
#include "agaASTNode.h"

namespace aga
{
    class agaASTMatch : public agaASTNode
    {
      public:
        agaASTMatch (std::shared_ptr<agaASTNode> &parentNode) : agaASTNode (MatchNode, parentNode) {}

        void AddCase (std::unique_ptr<agaASTNode> caseExpr) { m_Cases.push_back (std::move (caseExpr)); }

        const std::vector<std::unique_ptr<agaASTNode>> &GetCases () { return m_Cases; }

        void SetExpression (std::unique_ptr<agaASTExpression> &expression) { m_Expression = std::move (expression); }

        const std::unique_ptr<agaASTExpression> &GetExpression () { return m_Expression; }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator) { return nullptr; }

        virtual const std::string ToString () { return "?" + m_Expression->ToString (); }

      private:
        std::unique_ptr<agaASTExpression> m_Expression;
        std::vector<std::unique_ptr<agaASTNode>> m_Cases;
    };
}

#endif //	_AGA_ASTMATCH_H_
