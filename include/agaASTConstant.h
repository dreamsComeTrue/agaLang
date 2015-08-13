#ifndef _AGA_ASTCONSTANT_H_
#define _AGA_ASTCONSTANT_H_

#include "agaASTExpression.h"
#include "agaToken.h"

namespace aga
{
	class agaASTConstant : public agaASTExpression
	{
	public:
		agaASTConstant (agaToken token, long value) :
			agaASTExpression (IntegerExpression),
			m_Token (token),
			m_LongValue (value) { }

		agaASTConstant (agaToken token, double value) :
			agaASTExpression (FloatExpression),
			m_Token (token),
			m_DoubleValue (value) { }

		virtual double Evaluate (agaCodeGenerator* codeGenerator)
		{
			std::string line = "CONST " + m_Token.GetLiteral();
			codeGenerator->AddCodeLine(line);
			
			if (m_Type == IntegerExpression)
			{
				return m_LongValue;
			}
			else if (m_Type == FloatExpression)
			{
				return m_DoubleValue;
			}
		}
		
		long GetLongValue ()
		{
			return m_LongValue;
		}
		
		double GetDoubleValue ()
		{
			return m_DoubleValue;
		}

	private:
		agaToken		m_Token;
		
		union
		{
			long		m_LongValue;
			double		m_DoubleValue;
		};
	};
}

#endif	//	_AGA_ASTCONSTANT_H_
