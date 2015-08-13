#ifndef _AGA_ASTEXPRESSION_H_
#define _AGA_ASTEXPRESSION_H_

#include "agaCodeGenerator.h"

namespace aga
{
	enum ExpressionType
	{
		IntegerExpression,
		FloatExpression,
		StringExpression,
		VariableExpression
	};
	
	class agaASTExpression
	{
	public:
		agaASTExpression (ExpressionType type) :
			m_Type (type) { }
			
		ExpressionType GetType ()
		{
			return m_Type;
		}
			
		virtual double Evaluate (agaCodeGenerator* codeGenerator) = 0;
		
	protected:
		ExpressionType	m_Type;
	};
}

#endif	//	_AGA_ASTEXPRESSION_H_