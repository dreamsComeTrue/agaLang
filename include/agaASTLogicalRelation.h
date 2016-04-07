#ifndef _AGA_ASTLOGICALRELATION_H_
#define _AGA_ASTLOGICALRELATION_H_

#include "agaASTConstant.h"
#include "agaASTExpression.h"
#include "agaInstructions.h"

namespace aga
{
    class agaASTLogicalRelation : public agaASTExpression
    {
      public:
        agaASTLogicalRelation (agaToken token, std::unique_ptr<agaASTNode> child)
            : agaASTExpression (LogicalRelationNode, LogicalRelation, token),
              m_Operator (token.GetLiteral ()),
              m_Left (std::move (child))
        {
        }

        agaASTLogicalRelation (agaToken token, std::unique_ptr<agaASTNode> left, std::unique_ptr<agaASTNode> right)
            : agaASTExpression (LogicalRelationNode, LogicalRelation, token),
              m_Operator (token.GetLiteral ()),
              m_Left (std::move (left)),
              m_Right (std::move (right))
        {
        }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator)
        {
            std::string code = "";

            if (m_Operator == "&&")
            {
                code = instructions[InstructionType::MUL].word;
            }
            else
            {
                if (m_Operator == "||")
                {
                    code = instructions[InstructionType::MUL].word;
                }
                else
                {
                    if (m_Operator == "!")
                    {
                        code = instructions[InstructionType::MUL].word;
                    }
                }
            }

            m_AllocationBlock.SetCode (code);

            if (m_Left != nullptr)
            {
                m_Left->Evaluate (codeGenerator);
            }

            if (m_Right != nullptr)
            {
                m_Right->Evaluate (codeGenerator);
            }

            return nullptr;
        }

        const std::unique_ptr<agaASTNode> &GetLeft () { return m_Left; }

        const std::unique_ptr<agaASTNode> &GetRight () { return m_Right; }

        virtual const std::string ToString () { return "KOT"; }

      private:
        std::unique_ptr<agaASTNode> m_Left;
        std::unique_ptr<agaASTNode> m_Right;
        std::string m_Operator;
    };
}

#endif //	_AGA_ASTLOGICALRELATION_H_
