#ifndef _AGA_ASTPROGRAM_H_
#define _AGA_ASTPROGRAM_H_

#include <vector>

#include "agaASTNode.h"

namespace aga
{
	class agaASTProgram : public agaASTNode
	{
	public:
		agaASTProgram () :
			agaASTNode (ProgramNode) { }

		void AddStatement (agaASTNode *node)
		{
			m_Statements.push_back (node);
		}
		
		const std::vector<agaASTNode *>& GetStatements ()
		{
			return m_Statements;
		}

		virtual void Evaluate ()
		{
		}

	private:
		std::vector<agaASTNode *> m_Statements;
	};
}

#endif	//	_AGA_ASTPROGRAM_H_
