#ifndef _AGA_CODEGENERATOR_H_
#define _AGA_CODEGENERATOR_H_

#include <string>
#include <vector>

namespace aga
{
	class agaASTExpression;
	class agaASTNode;
	
	class agaCodeGenerator
	{
	public:
		agaCodeGenerator ();
		
		const std::string& GenerateCode (agaASTExpression* expression);
		
	protected:
		void AddCodeLine (const std::string& codeLine);
		void GenerateCode (agaASTNode* node);
		bool CheckNodesAreFinal (std::vector<agaASTNode *> children);
		
		std::string		m_Code;
	};
}

#endif	//	_AGA_CODEGENERATOR_H_