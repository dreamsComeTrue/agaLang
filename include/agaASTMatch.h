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

        void AddCase (std::shared_ptr<agaASTNode> caseExpr) { m_Cases.push_back (caseExpr); }

        const std::vector<std::shared_ptr<agaASTNode>> &GetCases () { return m_Cases; }

        void SetExpression (std::shared_ptr<agaASTExpression> &expression) { m_Expression = expression; }

        const std::shared_ptr<agaASTExpression> &GetExpression () { return m_Expression; }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator) { return nullptr; }

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer) { m_Expression->SemanticCheck (analyzer); }

        virtual const std::string ToString () { return "?" + m_Expression->ToString (); }

      private:
        std::shared_ptr<agaASTExpression> m_Expression;
        std::vector<std::shared_ptr<agaASTNode>> m_Cases;
    };
}

#endif //	_AGA_ASTMATCH_H_
