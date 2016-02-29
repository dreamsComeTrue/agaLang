#ifndef _AGA_CODEGENERATOR_H_
#define _AGA_CODEGENERATOR_H_

#include <string>
#include <vector>

#include "agaInstructions.h"

namespace aga
{
	class agaASTExpression;
	class agaASTBinaryOperator;
	class agaASTBooleanRelation;
	class agaASTLogicalRelation;
	class agaASTNode;
	class agaASTProgram;
	class agaASTAssignment;

	class agaCodeGenerator
	{
	public:
		agaCodeGenerator ();

		const std::vector<std::string> &GenerateCode (agaASTProgram *program);

	protected:
		const std::vector<std::string> &GenerateCode (agaASTNode *node);

		void AddCodeLine (const std::string &codeLine, int indentLevel = 0);

		void GenerateAssignment (agaASTAssignment *node);
		void GenerateBinaryExpression (agaASTBinaryOperator *node);
		void GenerateBooleanRelation (agaASTBooleanRelation *node);
		void GenerateLogicalRelation (agaASTLogicalRelation *node);

        void EmitInstruction (InstructionType instruction, const std::string &label = "");
        void EmitInstruction (InstructionType instruction, int dstRegisterIndex);
		void EmitInstruction (InstructionType instruction, int dstRegisterIndex, int srcRegisterIndex);
		void EmitInstruction (InstructionType instruction, int registerIndex, const std::string &label);
		void EmitInstruction (InstructionType instruction, const std::string &label, int registerIndex);
		const std::string GenerateLabel ();
		const std::string EmitLabel (const std::string &overrideLabel = "");
		const std::string GetTrueLiteral ();
		const std::string GetFalseLiteral ();

		InstructionType GetInstructionTypeFromCode (const std::string &code);

		std::vector<std::string> m_Code;
		int m_CurrentRegisterIndex;
		int m_CurrentLabelIndex;
	};
}

#endif	//	_AGA_CODEGENERATOR_H_
