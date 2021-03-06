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
        agaASTLogicalRelation (agaToken token, std::shared_ptr<agaASTNode> child, std::shared_ptr<agaASTNode> &parentNode)
            : agaASTExpression (LogicalRelationNode, LogicalRelationExpression, token, parentNode),
              m_Operator (token.GetLiteral ()),
              m_Left (child)
        {
        }

        agaASTLogicalRelation (agaToken token, std::shared_ptr<agaASTNode> left, std::shared_ptr<agaASTNode> right,
                               std::shared_ptr<agaASTNode> &parentNode)
            : agaASTExpression (LogicalRelationNode, LogicalRelationExpression, token, parentNode),
              m_Operator (token.GetLiteral ()),
              m_Left (left),
              m_Right (right)
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

        const std::shared_ptr<agaASTNode> &GetLeft () { return m_Left; }

        const std::shared_ptr<agaASTNode> &GetRight () { return m_Right; }

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer)
        {
            m_Left->SemanticCheck (analyzer);
            m_Right->SemanticCheck (analyzer);
        }

        virtual const std::string ToString () { return "LOGICAL"; }

      private:
        std::shared_ptr<agaASTNode> m_Left;
        std::shared_ptr<agaASTNode> m_Right;
        std::string m_Operator;
    };
}

#endif //	_AGA_ASTLOGICALRELATION_H_
