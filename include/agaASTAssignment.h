#ifndef _AGA_ASTASSIGNMENT_H_
#define _AGA_ASTASSIGNMENT_H_

#include "agaASTExpression.h"

namespace aga
{
    class agaASTVariable;

    class agaASTAssignment : public agaASTExpression
    {
      public:
        agaASTAssignment (std::unique_ptr<agaASTVariable> &variable, std::unique_ptr<agaASTExpression> &expression,
                          std::shared_ptr<agaASTNode> parentNode);

        const std::unique_ptr<agaASTExpression> &GetExpression () const { return m_Expression; }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator) override;

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer) override;

        virtual const std::string ToString () override { return m_Token.GetLiteral () + " = " + m_Expression->ToString (); }

      protected:
        std::unique_ptr<agaASTVariable> m_Variable;
        std::unique_ptr<agaASTExpression> m_Expression;
    };
}

#endif //	_AGA_ASTASSIGNMENT_H_
