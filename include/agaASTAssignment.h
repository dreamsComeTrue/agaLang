#ifndef _AGA_ASTASSIGNMENT_H_
#define _AGA_ASTASSIGNMENT_H_

#include "agaASTExpression.h"
#include "agaASTVariable.h"

namespace aga
{
    class agaASTAssignment : public agaASTExpression
    {
      public:
        agaASTAssignment (std::unique_ptr<agaASTVariable> &variable, std::unique_ptr<agaASTExpression> &expression)
            : agaASTExpression (ASTNodeType::AssignmentNode, ExpressionType::Assignment, variable->GetToken ()),
              m_Variable (std::move (variable)),
              m_Expression (std::move (expression))
        {
        }

        const std::unique_ptr<agaASTExpression> &GetExpression () const { return m_Expression; }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator)
        {
            std::map<std::string, llvm::AllocaInst *> &namedValues = codeGenerator->GetNamedValues ();
            std::string valueName = m_Variable->GetToken ().GetLiteral ();

            if (namedValues.find (valueName) == namedValues.end ())
            {
                m_Variable->Evaluate (codeGenerator);
            }

            llvm::AllocaInst *alloc = namedValues[valueName];

            llvm::Value *expressionValue = m_Expression->Evaluate (codeGenerator);
            llvm::Value *result = codeGenerator->GetBuilder ().CreateStore (expressionValue, alloc);

            std::string code = m_Token.GetLiteral ();
            GetAllocationBlock ().SetCode (code);

            return expressionValue;
        }

        virtual const std::string ToString () { return m_Token.GetLiteral () + " = " + m_Expression->ToString (); }

      protected:
        std::unique_ptr<agaASTVariable> m_Variable;
        std::unique_ptr<agaASTExpression> m_Expression;
    };
}

#endif //	_AGA_ASTASSIGNMENT_H_
