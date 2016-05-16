#ifndef _AGA_ASTVARIABLE_H_
#define _AGA_ASTVARIABLE_H_

#include "agaASTExpression.h"
#include "agaToken.h"
#include "agaTypeInfo.h"

namespace aga
{
    class agaASTVariable : public agaASTExpression
    {
      public:
        agaASTVariable (agaToken token, std::shared_ptr<agaASTNode> &parentNode)
            : agaASTExpression (VariableNode, VariableExpression, token, parentNode), m_InitExpression (nullptr)
        {
        }

        agaASTVariable (agaToken token, const std::string &typeName, std::shared_ptr<agaASTNode> &parentNode)
            : agaASTExpression (VariableNode, VariableExpression, token, parentNode),
              m_InitExpression (nullptr),
              m_TypeInfo (typeName)
        {
            m_IRType = m_TypeInfo.GetLLVMType ();
        }

        agaASTVariable (agaToken token, const agaTypeInfo &typeInfo, std::shared_ptr<agaASTExpression> &initExpression,
                        std::shared_ptr<agaASTNode> &parentNode)
            : agaASTExpression (VariableNode, VariableExpression, token, parentNode),
              m_InitExpression (initExpression),
              m_TypeInfo (typeInfo)
        {
            m_IRType = m_TypeInfo.GetLLVMType ();
        }

        const std::shared_ptr<agaASTExpression> &GetExpression () const { return m_InitExpression; }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator) override;

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer) override;

        virtual const std::string ToString () override { return m_Token.GetLiteral (); }

      private:
        std::shared_ptr<agaASTExpression> m_InitExpression;
        agaTypeInfo m_TypeInfo;
    };
}

#endif //	_AGA_ASTVARIABLE_H_
