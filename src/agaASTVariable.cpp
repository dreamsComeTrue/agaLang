#include "agaASTVariable.h"
#include "agaASTBlock.h"

namespace aga
{
    //--------------------------------------------------------------------------------

    llvm::Value *agaASTVariable::Evaluate (agaCodeGenerator *codeGenerator)
    {
        std::string name = m_Token.GetLiteral ();
        std::shared_ptr<agaASTBlock> parent = GetParent ();

        if (parent->GetSymbol (name) == nullptr || parent->GetSymbol (name)->GetValue () == nullptr)
        {
            m_IRType = m_TypeInfo.GetLLVMType ();
            llvm::AllocaInst *allocValue = codeGenerator->GetBuilder ().CreateAlloca (m_IRType, nullptr, name);

            parent->PutSymbol (name, allocValue, parent.get ());
        }

        llvm::Value *load = codeGenerator->GetBuilder ().CreateLoad (parent->GetSymbol (name)->GetValue (), name);

        if (m_InitExpression != nullptr)
        {
            llvm::Value *expressionValue = m_InitExpression->Evaluate (codeGenerator);
            llvm::AllocaInst *alloc = parent->GetSymbol (name)->GetValue ();

            if (expressionValue->getType () == m_IRType)
            {
                codeGenerator->GetBuilder ().CreateStore (expressionValue, alloc);
            }
            else if (expressionValue->getType ()->getScalarSizeInBits () > m_IRType->getScalarSizeInBits ())
            {
                llvm::Value *castValue =
                    codeGenerator->GetBuilder ().CreateCast (llvm::Instruction::Trunc, expressionValue, m_IRType);
                codeGenerator->GetBuilder ().CreateStore (castValue, alloc);
            }
            else if (expressionValue->getType ()->getScalarSizeInBits () < m_IRType->getScalarSizeInBits ())
            {
                llvm::Value *castValue =
                    codeGenerator->GetBuilder ().CreateCast (llvm::Instruction::SExt, expressionValue, m_IRType);
                codeGenerator->GetBuilder ().CreateStore (castValue, alloc);
            }

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
            block->PutSymbol (name, nullptr, nullptr);
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
