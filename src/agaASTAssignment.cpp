#include "agaASTAssignment.h"
#include "agaASTBlock.h"
#include "agaASTVariable.h"
#include "agaCast.h"

namespace aga
{
    //--------------------------------------------------------------------------------

    agaASTAssignment::agaASTAssignment (std::shared_ptr<agaASTVariable> &variable, std::shared_ptr<agaASTExpression> &expression,
                                        std::shared_ptr<agaASTNode> parentNode)
        : agaASTExpression (ASTNodeType::AssignmentNode, ExpressionType::AssignmentExpression, variable->GetToken (), parentNode),
          m_Variable (variable),
          m_Expression (expression)
    {
    }

    //--------------------------------------------------------------------------------

    llvm::Value *agaASTAssignment::Evaluate (agaCodeGenerator *codeGenerator)
    {
        llvm::Value *expressionValue = m_Expression->Evaluate (codeGenerator);

        //    m_Variable->SetIRType (expressionValue->getType ());

        std::string valueName = m_Variable->GetToken ().GetLiteral ();
        std::shared_ptr<agaASTBlock> parent = GetParent ();

        if (parent->GetSymbol (valueName) == nullptr || parent->GetSymbol (valueName)->GetValue () == nullptr)
        {
            m_Variable->Evaluate (codeGenerator);
        }

        llvm::AllocaInst *alloc = parent->GetSymbol (valueName)->GetValue ();
        llvm::Type *type = parent->GetSymbol (valueName)->GetNode ()->GetIRType ();
        llvm::Value *assignValue = agaCast::CreateCast (expressionValue, type, codeGenerator->GetBuilder ());

        codeGenerator->GetBuilder ().CreateStore (assignValue, alloc);

        std::string code = m_Token.GetLiteral ();
        GetAllocationBlock ().SetCode (code);

        return expressionValue;
    }

    //--------------------------------------------------------------------------------

    void agaASTAssignment::SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer)
    {
        analyzer->SetPhase (VariablePhase);
        m_Variable->SemanticCheck (analyzer);

        analyzer->SetPhase (ExpressionPhase);
        m_Expression->SemanticCheck (analyzer);
    }

    //--------------------------------------------------------------------------------
}
