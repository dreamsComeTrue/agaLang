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

        void AddCase (agaASTNode *caseExpr) { m_Cases.push_back (caseExpr); }

        const std::vector<agaASTNode *> &GetCases () { return m_Cases; }

        void SetExpression (agaASTNode *expression) { m_Expression = expression; }

        const agaASTNode *GetExpression () const { return m_Expression; }

        virtual void Evaluate () {}

        virtual const std::string ToString ()
        {
            return "?" + m_Expression->ToString ();
        }

      private:
        agaASTNode *m_Expression;
        std::vector<agaASTNode *> m_Cases;
    };
}

#endif //	_AGA_ASTMATCH_H_
