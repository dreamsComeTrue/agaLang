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
			agaASTExpression (VariableNode, Variable, token)	{ }

		virtual void Evaluate ()
		{
			std::string line = "VAR " + m_Token.GetLiteral();
			
			m_AllocationBlock.SetCode(line);
		}

	private:
	};
}

#endif	//	_AGA_ASTVARIABLE_H_
