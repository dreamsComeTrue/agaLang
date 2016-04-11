#ifndef _AGA_ASTBOOLEANRELATION_H_
#define _AGA_ASTBOOLEANRELATION_H_

#include "agaASTConstant.h"
#include "agaASTExpression.h"
#include "agaInstructions.h"

namespace aga
{
    class agaASTBooleanRelation : public agaASTExpression
    {
      public:
        agaASTBooleanRelation (agaToken token, std::unique_ptr<agaASTNode> left, std::unique_ptr<agaASTNode> right,
                               std::shared_ptr<agaASTNode> parentNode)
            : agaASTExpression (BooleanRelationNode, BooleanRelationExpression, token, parentNode),
              m_Operator (token.GetLiteral ()),
              m_Left (std::move (left)),
              m_Right (std::move (right))
        {
        }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator)
        {
            std::string code = "";

            if (m_Operator == "==")
            {
                code = instructions[InstructionType::MUL].word;
            }
            else if (m_Operator == "<=")
            {
                code = instructions[InstructionType::MUL].word;
            }
            else if (m_Operator == "<")
            {
                code = instructions[InstructionType::MUL].word;
            }

            else if (m_Operator == ">=")
            {
                code = instructions[InstructionType::MUL].word;
            }

            else if (m_Operator == ">")
            {
                code = instructions[InstructionType::MUL].word;
            }

            m_AllocationBlock.SetCode (code);

            m_Left->Evaluate (codeGenerator);
            m_Right->Evaluate (codeGenerator);

            return nullptr;
        }

        const std::unique_ptr<agaASTNode> &GetLeft () { return m_Left; }

        const std::unique_ptr<agaASTNode> &GetRight () { return m_Right; }

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer)
        {
            m_Left->SemanticCheck (analyzer);
            m_Right->SemanticCheck (analyzer);
        }

        virtual const std::string ToString () { return "BOOL"; }

      private:
        std::unique_ptr<agaASTNode> m_Left;
        std::unique_ptr<agaASTNode> m_Right;
        std::string m_Operator;
    };
}

#endif //	_AGA_ASTBOOLEANRELATION_H_
