#ifndef _AGA_CODEGENERATOR_H_
#define _AGA_CODEGENERATOR_H_

#include <string>
#include <vector>

#include "agaInstructions.h"

namespace aga
{
	class agaASTExpression;
	class agaASTNode;
	class agaASTProgram;
	
	class agaCodeGenerator
	{
	public:
		agaCodeGenerator ();
		
		const std::vector<std::string>& GenerateCode (agaASTProgram* program);
		
	protected:
		const std::vector<std::string> &GenerateCode (agaASTNode *node);
		
		void AddCodeLine (const std::string& codeLine);
		
		void GenerateBinaryExpression (agaASTNode* node);
		
		void EmitInstruction (InstructionType instruction, int dstRegisterIndex, int srcRegisterIndex);
		void EmitInstruction (InstructionType instruction, int registerIndex, agaASTNode* node);
		
		InstructionType GetInstructionTypeFromCode (const std::string& code);
		
		std::vector<std::string> m_Code;
	};
}

#endif	//	_AGA_CODEGENERATOR_H_