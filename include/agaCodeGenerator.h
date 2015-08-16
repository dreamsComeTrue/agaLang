#ifndef _AGA_CODEGENERATOR_H_
#define _AGA_CODEGENERATOR_H_

#include <string>
#include <vector>

#include "agaInstructions.h"

namespace aga
{
	class agaASTExpression;
	class agaASTBinaryOperator;
	class agaASTNode;
	class agaASTProgram;
	class agaASTAssignment;
	
	class agaCodeGenerator
	{
	public:
		agaCodeGenerator ();
		
		const std::vector<std::string>& GenerateCode (agaASTProgram* program);
		
	protected:
		const std::vector<std::string> &GenerateCode (agaASTNode *node);
		
		void AddCodeLine (const std::string& codeLine);
		
		void GenerateAssignment (agaASTAssignment* node);
		void GenerateBinaryExpression (agaASTBinaryOperator* node);
		
		void EmitInstruction (InstructionType instruction, int dstRegisterIndex, int srcRegisterIndex);
		void EmitInstruction (InstructionType instruction, int registerIndex, agaASTNode* node);
		void EmitInstruction (InstructionType instruction, agaASTNode* node, int registerIndex);
		
		InstructionType GetInstructionTypeFromCode (const std::string& code);
		
		std::vector<std::string> m_Code;
		int m_CurrentRegisterIndex;
	};
}

#endif	//	_AGA_CODEGENERATOR_H_