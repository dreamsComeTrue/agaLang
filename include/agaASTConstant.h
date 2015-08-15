#ifndef _AGA_ASTCONSTANT_H_
#define _AGA_ASTCONSTANT_H_

#include "agaASTExpression.h"
#include "agaToken.h"
#include "agaTypeInfo.h"

namespace aga
{
	class agaASTConstant : public agaASTExpression
	{
	public:
		agaASTConstant (agaToken token, long value) :
			agaASTExpression (ConstantNode, IntegerConst, token),
			m_TypeInfo (value) { }

		agaASTConstant (agaToken token, double value) :
			agaASTExpression (ConstantNode, FloatConst, token),
			m_TypeInfo (value) { }

		virtual void Evaluate ()
		{
			std::string line = "CONST " + m_Token.GetLiteral();

			m_AllocationBlock.SetCode (line);
		}

	private:
		agaTypeInfo		m_TypeInfo;
	};
}

#endif	//	_AGA_ASTCONSTANT_H_
