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
        agaASTBinaryOperator (agaToken token, std::unique_ptr<agaASTNode> left, std::unique_ptr<agaASTNode> right)
            : agaASTExpression (BinaryOperationNode, BinaryOperation, token),
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

            switch (m_Operator)
            {
            case '*':
                code = instructions[InstructionType::MUL].word;
                return codeGenerator->GetBuilder ().CreateMul (left, right, "mul");

            case '+':
                code = instructions[InstructionType::ADD].word;
                return codeGenerator->GetBuilder ().CreateAdd (left, right, "add");

            case '-':
                code = instructions[InstructionType::SUB].word;
                return codeGenerator->GetBuilder ().CreateSub (left, right, "sub");

            case '/':
                code = instructions[InstructionType::DIV].word;
                return codeGenerator->GetBuilder ().CreateSDiv (left, right, "div");

            default:
                break;
            }

            m_AllocationBlock.SetCode (code);

            return nullptr;
        }

        const std::unique_ptr<agaASTNode> &GetLeft () { return m_Left; }

        const std::unique_ptr<agaASTNode> &GetRight () { return m_Right; }

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
