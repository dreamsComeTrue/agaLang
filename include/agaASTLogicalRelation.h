#ifndef _AGA_ASTLOGICALRELATION_H_
#define _AGA_ASTLOGICALRELATION_H_

#include "agaASTExpression.h"
#include "agaASTConstant.h"
#include "agaInstructions.h"

namespace aga
{
	class agaASTLogicalRelation : public agaASTExpression
	{
	public:
		agaASTLogicalRelation (agaToken token, agaASTNode *child) :
			agaASTExpression (LogicalRelationNode, LogicalRelation, token),
			m_Operator (token.GetLiteral())
		{
			m_Children.push_back (child);
		}
		
		agaASTLogicalRelation (agaToken token, agaASTNode *left, agaASTNode *right) :
			agaASTExpression (LogicalRelationNode, LogicalRelation, token),
			m_Operator (token.GetLiteral())
		{
			m_Children.push_back (left);
			m_Children.push_back (right);
		}

		virtual void Evaluate ()
		{
			std::string code = "";

			if (m_Operator == "&&")
			{
				code = instructions[InstructionType::MUL].word;
			}
			else
				if (m_Operator == "||")
				{
					code = instructions[InstructionType::MUL].word;
				}
				else
					if (m_Operator == "!")
					{
						code = instructions[InstructionType::MUL].word;
					}

			m_AllocationBlock.SetCode (code);

			for (agaASTNode *&child : m_Children)
			{
				child->Evaluate ();
			}
		}

	private:
		std::string		m_Operator;
	};
}

#endif	//	_AGA_ASTLOGICALRELATION_H_
