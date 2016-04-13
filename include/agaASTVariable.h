#ifndef _AGA_ASTVARIABLE_H_
#define _AGA_ASTVARIABLE_H_

#include "agaASTExpression.h"
#include "agaToken.h"

namespace aga
{
    class agaASTVariable : public agaASTExpression
    {
      public:
        agaASTVariable (agaToken token, std::shared_ptr<agaASTNode> &parentNode)
            : agaASTExpression (VariableNode, VariableExpression, token, parentNode)
        {
        }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator) override;

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer) override;

        virtual const std::string ToString () override { return m_Token.GetLiteral (); }
    };
}

#endif //	_AGA_ASTVARIABLE_H_
