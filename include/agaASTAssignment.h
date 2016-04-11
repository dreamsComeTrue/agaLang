#ifndef _AGA_ASTASSIGNMENT_H_
#define _AGA_ASTASSIGNMENT_H_

#include "agaASTExpression.h"
#include "agaASTVariable.h"

namespace aga
{
    class agaASTAssignment : public agaASTExpression
    {
      public:
        agaASTAssignment (std::unique_ptr<agaASTVariable> &variable, std::unique_ptr<agaASTExpression> &expression,
                          std::shared_ptr<agaASTNode> parentNode)
            : agaASTExpression (ASTNodeType::AssignmentNode, ExpressionType::AssignmentExpression, variable->GetToken (),
                                parentNode),
              m_Variable (std::move (variable)),
              m_Expression (std::move (expression))
        {
        }

        const std::unique_ptr<agaASTExpression> &GetExpression () const { return m_Expression; }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator)
        {
            llvm::Value *expressionValue = m_Expression->Evaluate (codeGenerator);

            m_Variable->SetIRType (expressionValue->getType ());

            std::string valueName = m_Variable->GetToken ().GetLiteral ();
            std::shared_ptr<agaASTBlock> block = GetBlock ();

            if (block->GetSymbol (valueName)->GetValue () == nullptr)
            {
                m_Variable->Evaluate (codeGenerator);
            }

            llvm::AllocaInst *alloc = block->GetSymbol (valueName)->GetValue ();

            codeGenerator->GetBuilder ().CreateStore (expressionValue, alloc);

            std::string code = m_Token.GetLiteral ();
            GetAllocationBlock ().SetCode (code);

            return expressionValue;
        }

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer)
        {
            analyzer->SetPhase (VariablePhase);
            m_Variable->SemanticCheck (analyzer);

            analyzer->SetPhase (ExpressionPhase);
            m_Expression->SemanticCheck (analyzer);
        }

        virtual const std::string ToString () { return m_Token.GetLiteral () + " = " + m_Expression->ToString (); }

      protected:
        std::unique_ptr<agaASTVariable> m_Variable;
        std::unique_ptr<agaASTExpression> m_Expression;
    };
}

#endif //	_AGA_ASTASSIGNMENT_H_
