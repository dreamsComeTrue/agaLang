#ifndef _AGA_ASTBOOLEANRELATION_H_
#define _AGA_ASTBOOLEANRELATION_H_

#include "agaASTExpression.h"
#include "agaASTConstant.h"
#include "agaInstructions.h"

namespace aga
{
	class agaASTBooleanRelation : public agaASTExpression
	{
	public:
		agaASTBooleanRelation (agaToken token, agaASTNode *left, agaASTNode *right) :
			agaASTExpression (BooleanRelationNode, BooleanRelation, token),
            m_Operator (token.GetLiteral()), m_Left (left), m_Right (right)
		{
		}

		virtual void Evaluate ()
		{
			std::string code = "";

			if (m_Operator == "==")
			{
				code = instructions[InstructionType::MUL].word;
			}
			else
				if (m_Operator == "<=")
				{
					code = instructions[InstructionType::MUL].word;
				}
				else
					if (m_Operator == "<")
					{
						code = instructions[InstructionType::MUL].word;
					}

					else
						if (m_Operator == ">=")
						{
							code = instructions[InstructionType::MUL].word;
						}

						else
							if (m_Operator == ">")
							{
								code = instructions[InstructionType::MUL].word;
							}

			m_AllocationBlock.SetCode (code);

            m_Left->Evaluate ();
            m_Right->Evaluate ();
		}

        agaASTNode* GetLeft ()
        {
            return m_Left;
        }

        agaASTNode* GetRight ()
        {
            return m_Right;
        }

        virtual const std::string ToString ()
        {
            return "KOT";
        }

	private:
        agaASTNode      *m_Left;
        agaASTNode      *m_Right;
		std::string		m_Operator;
	};
}

#endif	//	_AGA_ASTBOOLEANRELATION_H_
