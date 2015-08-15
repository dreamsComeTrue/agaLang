#ifndef _AGA_ASTASSIGNMENT_H_
#define _AGA_ASTASSIGNMENT_H_

#include "agaASTExpression.h"

namespace aga
{
	class agaASTAssignment : public agaASTExpression
	{
	public:
		agaASTAssignment (agaToken token) :
			agaASTExpression (ExpressionType::Assignment),
			m_Token (token) { }

		virtual agaASTNode *Evaluate (agaASTNode *parent) = 0;

	protected:
		agaToken		m_Token;
	};
}

#endif	//	_AGA_ASTASSIGNMENT_H_
