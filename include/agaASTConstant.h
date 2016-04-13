#ifndef _AGA_ASTCONSTANT_H_
#define _AGA_ASTCONSTANT_H_

#include "agaASTExpression.h"
#include "agaToken.h"
#include "agaTypeInfo.h"

namespace aga
{
    class agaASTConstant : public agaASTExpression
    {
      public:
        agaASTConstant (agaToken token, long value, std::shared_ptr<agaASTNode> parentNode)
            : agaASTExpression (ConstantNode, IntegerConstExpression, token, parentNode), m_TypeInfo (value)
        {
        }

        agaASTConstant (agaToken token, double value, std::shared_ptr<agaASTNode> parentNode)
            : agaASTExpression (ConstantNode, FloatConstExpression, token, parentNode), m_TypeInfo (value)
        {
        }

        agaASTConstant (agaToken token, std::string value, std::shared_ptr<agaASTNode> &parentNode)
            : agaASTExpression (ConstantNode, StringConstExpression, token, parentNode),
              m_TypeInfo (const_cast<char *> (value.c_str ()))
        {
        }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator) override;

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer) override {}

        virtual const std::string ToString () override { return m_Token.GetLiteral (); }

      private:
        agaTypeInfo m_TypeInfo;
    };
}

#endif //	_AGA_ASTCONSTANT_H_
