#ifndef _AGA_ASTBINARYOPERATOR_H_
#define _AGA_ASTBINARYOPERATOR_H_

#include "agaASTExpression.h"
#include "agaASTConstant.h"

namespace aga
{
	class agaASTBinaryOperator : public agaASTExpression
	{
	public:
		agaASTBinaryOperator (char op, agaASTExpression *left, agaASTExpression *right) :
			agaASTExpression (VariableExpression),
			m_Operator (op),
			m_Left (left),
			m_Right (right) { }


		virtual double Evaluate (agaCodeGenerator* codeGenerator)
		{
			long leftLong;
			double leftDouble;
			ExpressionType leftType = VariableExpression;

			long rightLong;
			double rightDouble;
			ExpressionType rightType = VariableExpression;

			if (m_Left->GetType() == IntegerExpression)
			{
				leftLong = ( (agaASTConstant*) m_Left)->GetLongValue();
				leftType = IntegerExpression;
			}
			else
				if (m_Left->GetType() == FloatExpression)
				{
					leftDouble = ( (agaASTConstant*) m_Left)->GetDoubleValue();
					leftType = FloatExpression;
				}

			if (m_Right->GetType() == IntegerExpression)
			{
				rightLong = ( (agaASTConstant*) m_Right)->GetLongValue();
				rightType = IntegerExpression;
			}
			else
				if (m_Right->GetType() == FloatExpression)
				{
					rightDouble = ( (agaASTConstant*) m_Right)->GetDoubleValue();
					rightType = FloatExpression;
				}

			switch (m_Operator)
			{
				case '*':
					codeGenerator->AddCodeLine("MUL");
					return m_Left->Evaluate (codeGenerator) * m_Right->Evaluate (codeGenerator);

				case '+':
					codeGenerator->AddCodeLine("ADD");
					return m_Left->Evaluate (codeGenerator) + m_Right->Evaluate (codeGenerator);

				case '-':
					codeGenerator->AddCodeLine("SUB");
					return m_Left->Evaluate (codeGenerator) - m_Right->Evaluate (codeGenerator);

				case '/':
					codeGenerator->AddCodeLine("DIV");
					return m_Left->Evaluate (codeGenerator) / m_Right->Evaluate (codeGenerator);
					
				default
						:
					break;
			}
		}

	private:
		char				m_Operator;
		agaASTExpression	*m_Left;
		agaASTExpression	*m_Right;
	};
}

#endif	//	_AGA_ASTBINARYOPERATOR_H_
