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
			agaASTExpression (IntegerExpression),
			m_Token (token),
			m_TypeInfo (value) { }

		agaASTConstant (agaToken token, double value) :
			agaASTExpression (FloatExpression),
			m_Token (token),
			m_TypeInfo (value) { }

		virtual agaASTNode *Evaluate (agaASTNode *parent)
		{
			std::string line = "CONST " + m_Token.GetLiteral();

			agaASTNode *node = new agaASTNode (ASTNodeType::Constant, parent, line);

			return node;
		}

	private:
		agaToken		m_Token;
		agaTypeInfo		m_TypeInfo;
	};
}

#endif	//	_AGA_ASTCONSTANT_H_
