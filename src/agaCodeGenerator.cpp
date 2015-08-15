#include <sstream>

#include "agaCodeGenerator.h"
#include "agaASTExpression.h"
#include "agaASTProgram.h"

namespace aga
{
	//--------------------------------------------------------------------------------

	agaCodeGenerator::agaCodeGenerator()
	{

	}

	//--------------------------------------------------------------------------------

	void agaCodeGenerator::AddCodeLine (const std::string &codeLine)
	{
		m_Code.push_back (codeLine);
	}

	//--------------------------------------------------------------------------------

	const std::vector<std::string> &agaCodeGenerator::GenerateCode (agaASTProgram *program)
	{
		std::vector<agaASTNode *> statements = program->GetStatements ();

		for (int i = 0; i < statements.size(); ++i)
		{
			agaASTNode *statement = statements[i];

			statement->Evaluate();

			GenerateCode (statement);
		}

		return m_Code;
	}

	//--------------------------------------------------------------------------------

	const std::vector<std::string> &agaCodeGenerator::GenerateCode (agaASTNode *node)
	{
		if (node->GetType() == ASTNodeType::BinaryOperationNode)
		{
			GenerateBinaryExpression (node);
		}
		else
		{
			std::string codeLine = node->GetAllocationBlock().GetCode();

			AddCodeLine (codeLine);

			std::vector<agaASTNode *> children = node->GetChildren();

			for (int i = 0; i < children.size(); ++i)
			{
				GenerateCode (children[i]);
			}
		}

		return m_Code;
	}

	//--------------------------------------------------------------------------------

	int currentRegisterIndex = 1;

	//--------------------------------------------------------------------------------

	std::string ToString (int value)
	{
		std::stringstream sstream;

		sstream << value;

		return sstream.str();
	}

	//--------------------------------------------------------------------------------

	void agaCodeGenerator::GenerateBinaryExpression (agaASTNode *node)
	{
		std::vector<agaASTNode *> children = node->GetChildren();

		ASTNodeType leftType = children[0]->GetType();
		ASTNodeType rightType = children[1]->GetType();

		if (leftType == ASTNodeType::BinaryOperationNode)
		{
			GenerateBinaryExpression (children[0]);
			children[0]->GetAllocationBlock ().SetRegisterIndex (currentRegisterIndex-1);
		}

		if (rightType == ASTNodeType::BinaryOperationNode)
		{
			GenerateBinaryExpression (children[1]);
			children[1]->GetAllocationBlock ().SetRegisterIndex (currentRegisterIndex-1);
		}

		if (leftType != ASTNodeType::BinaryOperationNode)
		{
			EmitInstruction (InstructionType::MOV, currentRegisterIndex, children[0]);
		}
		else
		{
			EmitInstruction (InstructionType::MOV, currentRegisterIndex, children[0]->GetAllocationBlock().GetRegisterIndex ());
		}
		
		InstructionType instructionType = GetInstructionTypeFromCode (node->GetAllocationBlock().GetCode());

		if (rightType != ASTNodeType::BinaryOperationNode)
		{
			EmitInstruction (instructionType, currentRegisterIndex, children[1]);
		}
		else
		{
			EmitInstruction (instructionType, currentRegisterIndex, children[1]->GetAllocationBlock().GetRegisterIndex ());
		}

		++currentRegisterIndex;
	}

	//--------------------------------------------------------------------------------

	void agaCodeGenerator::EmitInstruction (InstructionType instruction, int dstRegisterIndex, int srcRegisterIndex)
	{
		std::string codeLine = std::string(instructions[instruction].word) + " #";
		codeLine += ToString (dstRegisterIndex) + ", ";
		codeLine += "#" + ToString (srcRegisterIndex);

		AddCodeLine (codeLine);
	}

	//--------------------------------------------------------------------------------

	void agaCodeGenerator::EmitInstruction (InstructionType instruction, int registerIndex, agaASTNode *node)
	{
		std::string codeLine = std::string(instructions[instruction].word) + " #";
		codeLine += ToString (registerIndex) + ", ";
		codeLine += node->GetAllocationBlock().GetCode();

		AddCodeLine (codeLine);
	}
	
	//--------------------------------------------------------------------------------
	
	InstructionType agaCodeGenerator::GetInstructionTypeFromCode (const std::string& code)
	{
		for (int i = 0; i < instructionsCount; ++i)
		{
			if (instructions[i].word == code)
			{
				return instructions[i].type;
			}
		}
		
		return InstructionType::Unknown;
	}

	//--------------------------------------------------------------------------------
}
