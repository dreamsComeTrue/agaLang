#ifndef _AGA_ASTBINARYOPERATOR_H_
#define _AGA_ASTBINARYOPERATOR_H_

#include "agaASTConstant.h"
#include "agaASTExpression.h"
#include "agaInstructions.h"

namespace aga
{
    class agaASTBinaryOperator : public agaASTExpression
    {
      public:
        agaASTBinaryOperator (agaToken token, std::unique_ptr<agaASTNode> left, std::unique_ptr<agaASTNode> right,
                              std::shared_ptr<agaASTNode> parentNode)
            : agaASTExpression (BinaryOperationNode, BinaryOperationExpression, token, parentNode),
              m_Operator (token.GetLiteral ().at (0)),
              m_Left (std::move (left)),
              m_Right (std::move (right))
        {
        }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator)
        {
            llvm::Value *left = m_Left->Evaluate (codeGenerator);
            llvm::Value *right = m_Right->Evaluate (codeGenerator);

            if (!left || !right)
            {
                return nullptr;
            }

            std::string code = "";

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

                right =
                    builder.CreateUIToFP (right, llvm::Type::getDoubleTy (codeGenerator->GetModule ()->getContext ()), "uitofp");
            }
            else if (rightType->isDoubleTy ())
            {
                isDouble = true;

                left =
                    builder.CreateUIToFP (left, llvm::Type::getDoubleTy (codeGenerator->GetModule ()->getContext ()), "uitofp");
            }

            switch (m_Operator)
            {
            case '*':
                code = instructions[InstructionType::MUL].word;
                if (isDouble)
                {
                    return builder.CreateFMul (left, right, "fmul");
                }
                else
                {
                    return builder.CreateMul (left, right, "mul");
                }

            case '+':
                code = instructions[InstructionType::ADD].word;
                if (isDouble)
                {
                    return builder.CreateFAdd (left, right, "fadd");
                }
                else
                {
                    return builder.CreateAdd (left, right, "add");
                }

            case '-':
                code = instructions[InstructionType::SUB].word;
                if (isDouble)
                {
                    return builder.CreateFSub (left, right, "fsub");
                }
                else
                {
                    return builder.CreateSub (left, right, "sub");
                }

            case '/':
                code = instructions[InstructionType::DIV].word;
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

            m_AllocationBlock.SetCode (code);

            return nullptr;
        }

        const std::unique_ptr<agaASTNode> &GetLeft () { return m_Left; }

        const std::unique_ptr<agaASTNode> &GetRight () { return m_Right; }

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer)
        {
            m_Left->SemanticCheck (analyzer);
            m_Right->SemanticCheck (analyzer);
        }

        virtual const std::string ToString ()
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
