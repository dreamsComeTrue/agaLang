#ifndef _AGA_ASTMATCH_H_
#define _AGA_ASTMATCH_H_

#include <vector>

#include "agaASTNode.h"

namespace aga
{
    class agaASTMatch : public agaASTNode
    {
      public:
        agaASTMatch () : agaASTNode (MatchNode) {}

        void AddCase (std::shared_ptr<agaASTNode> caseExpr) { m_Cases.push_back (caseExpr); }

        const std::vector<std::shared_ptr<agaASTNode>> &GetCases () { return m_Cases; }

        void SetExpression (std::shared_ptr<agaASTNode> expression) { m_Expression = expression; }

        const std::shared_ptr<agaASTNode> GetExpression () const { return m_Expression; }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator) { return nullptr; }

        virtual const std::string ToString () { return "?" + m_Expression->ToString (); }

      private:
        std::shared_ptr<agaASTNode> m_Expression;
        std::vector<std::shared_ptr<agaASTNode>> m_Cases;
    };
}

#endif //	_AGA_ASTMATCH_H_
