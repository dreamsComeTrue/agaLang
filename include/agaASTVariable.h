#ifndef _AGA_ASTVARIABLE_H_
#define _AGA_ASTVARIABLE_H_

#include <string>

#include "agaASTExpression.h"
#include "agaToken.h"

namespace aga
{
	class agaASTVariable : public agaASTExpression
	{
	public:
		agaASTVariable (agaToken token) :
			agaASTExpression (VariableExpression),
			m_Token (token)	{ }
			
		virtual double Evaluate (agaCodeGenerator* codeGenerator)
		{
			std::string line = "VAR " + m_Token.GetLiteral();
			
			codeGenerator->AddCodeLine(line);
			return 0;
		}

	private:
		agaToken		m_Token;
	};
}

#endif	//	_AGA_ASTVARIABLE_H_