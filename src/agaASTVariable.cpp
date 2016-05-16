#include "agaASTVariable.h"
#include "agaASTBlock.h"
#include "agaCast.h"

namespace aga
{
    //--------------------------------------------------------------------------------

    llvm::Value *agaASTVariable::Evaluate (agaCodeGenerator *codeGenerator)
    {
        m_IRType = m_TypeInfo.GetLLVMType ();

        std::string name = m_Token.GetLiteral ();
        std::shared_ptr<agaASTBlock> parent = GetParent ();

        if (parent->GetSymbol (name) == nullptr || parent->GetSymbol (name)->GetValue () == nullptr)
        {
            llvm::AllocaInst *allocValue = codeGenerator->GetBuilder ().CreateAlloca (m_IRType, nullptr, name);

            parent->PutSymbol (name, this, allocValue, parent.get ());
        }

        llvm::Value *load = codeGenerator->GetBuilder ().CreateLoad (parent->GetSymbol (name)->GetValue (), name);

        if (m_InitExpression != nullptr)
        {
            llvm::Value *expressionValue = m_InitExpression->Evaluate (codeGenerator);
            llvm::AllocaInst *alloc = parent->GetSymbol (name)->GetValue ();
            llvm::Value *assignvalue = agaCast::CreateCast (expressionValue, m_IRType, codeGenerator->GetBuilder ());

            codeGenerator->GetBuilder ().CreateStore (assignvalue, alloc);

            return expressionValue;
        }

        return load;
    }

    //--------------------------------------------------------------------------------

    void agaASTVariable::SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer)
    {
        const std::shared_ptr<agaASTBlock> &block = analyzer->GetEnclosingBlock ();
        std::string name = m_Token.GetLiteral ();

        if (analyzer->GetPhase () == VariablePhase)
        {
            block->PutSymbol (name, nullptr, nullptr, nullptr);
        }
        else
        {
            if (!block->GetSymbol (name))
            {
                throw agaException (m_Token.GetInfo () + ": Variable '" + name + "' does not exist in current scope!");
            }
        }
    }

    //--------------------------------------------------------------------------------
}
