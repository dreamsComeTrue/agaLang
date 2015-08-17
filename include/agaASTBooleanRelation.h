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
			m_Operator (token.GetLiteral())
		{
			m_Children.push_back (left);
			m_Children.push_back (right);
		}

		virtual void Evaluate ()
		{
			std::string code = "";
			
			if (m_Operator == "==")
			{
				code = instructions[InstructionType::MUL].word;
			}

			m_AllocationBlock.SetCode (code);

			m_Children[0]->Evaluate ();
			m_Children[1]->Evaluate ();
		}

	private:
		std::string		m_Operator;
	};
}

#endif	//	_AGA_ASTBOOLEANRELATION_H_
