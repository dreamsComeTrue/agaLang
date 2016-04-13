#include "agaASTBinaryOperator.h"
#include "agaASTBlock.h"

namespace aga
{
    //--------------------------------------------------------------------------------

    agaASTBinaryOperator::agaASTBinaryOperator (agaToken token, std::unique_ptr<agaASTNode> left,
                                                std::unique_ptr<agaASTNode> right, std::shared_ptr<agaASTNode> parentNode)
        : agaASTExpression (BinaryOperationNode, BinaryOperationExpression, token, parentNode),
          m_Operator (token.GetLiteral ().at (0)),
          m_Left (std::move (left)),
          m_Right (std::move (right))
    {
    }

    //--------------------------------------------------------------------------------

    llvm::Value *agaASTBinaryOperator::Evaluate (agaCodeGenerator *codeGenerator)
    {
        llvm::Value *left = m_Left->Evaluate (codeGenerator);
        llvm::Value *right = m_Right->Evaluate (codeGenerator);

        if (!left || !right)
        {
            return nullptr;
        }

        llvm::IRBuilder<> &builder = codeGenerator->GetBuilder ();

        llvm::Type *leftType = left->getType ();
        llvm::Type *rightType = right->getType ();
        bool isDouble = false;

        if (leftType->isDoubleTy () && rightType->isDoubleTy ())
        {
            isDouble = true;
        }
        else if (leftType->isDoubleTy ())
        {
            isDouble = true;

            right = builder.CreateUIToFP (right, llvm::Type::getDoubleTy (codeGenerator->GetModule ()->getContext ()), "uitofp");
        }
        else if (rightType->isDoubleTy ())
        {
            isDouble = true;

            left = builder.CreateUIToFP (left, llvm::Type::getDoubleTy (codeGenerator->GetModule ()->getContext ()), "uitofp");
        }

        switch (m_Operator)
        {
        case '*':
            if (isDouble)
            {
                return builder.CreateFMul (left, right, "fmul");
            }
            else
            {
                return builder.CreateMul (left, right, "mul");
            }

        case '+':
            if (isDouble)
            {
                return builder.CreateFAdd (left, right, "fadd");
            }
            else
            {
                return builder.CreateAdd (left, right, "add");
            }

        case '-':
            if (isDouble)
            {
                return builder.CreateFSub (left, right, "fsub");
            }
            else
            {
                return builder.CreateSub (left, right, "sub");
            }

        case '/':
            if (isDouble)
            {
                return builder.CreateFDiv (left, right, "fdiv");
            }
            else
            {
                return builder.CreateSDiv (left, right, "div");
            }

        default:
            break;
        }

        return nullptr;
    }

    //--------------------------------------------------------------------------------

    void agaASTBinaryOperator::SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer)
    {
        m_Left->SemanticCheck (analyzer);
        m_Right->SemanticCheck (analyzer);
    }

    //--------------------------------------------------------------------------------
}
