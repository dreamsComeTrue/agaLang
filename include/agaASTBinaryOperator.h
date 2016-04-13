#ifndef _AGA_ASTBINARYOPERATOR_H_
#define _AGA_ASTBINARYOPERATOR_H_

#include "agaASTExpression.h"

namespace aga
{
    class agaASTBinaryOperator : public agaASTExpression
    {
      public:
        agaASTBinaryOperator (agaToken token, std::unique_ptr<agaASTNode> left, std::unique_ptr<agaASTNode> right,
                              std::shared_ptr<agaASTNode> parentNode);

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator) override;

        const std::unique_ptr<agaASTNode> &GetLeft () { return m_Left; }

        const std::unique_ptr<agaASTNode> &GetRight () { return m_Right; }

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer) override;

        virtual const std::string ToString () override
        {
            return "(" + m_Left->ToString () + " " + m_Operator + " " + m_Right->ToString () + ")";
        }

      private:
        char m_Operator;
        std::unique_ptr<agaASTNode> m_Left;
        std::unique_ptr<agaASTNode> m_Right;
    };
}

#endif //	_AGA_ASTBINARYOPERATOR_H_
