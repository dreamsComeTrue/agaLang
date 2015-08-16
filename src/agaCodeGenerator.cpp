#include <sstream>

#include "agaCodeGenerator.h"
#include "agaASTExpression.h"
#include "agaASTAssignment.h"
#include "agaASTBinaryOperator.h"
#include "agaASTProgram.h"

namespace aga
{
	//--------------------------------------------------------------------------------

	agaCodeGenerator::agaCodeGenerator() :
		m_CurrentRegisterIndex (1)
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
		if (node->GetType() == ASTNodeType::AssignmentNode)
		{
			GenerateAssignment (static_cast<agaASTAssignment *> (node));
		}
		else
			if (node->GetType() == ASTNodeType::BinaryOperationNode)
			{
				GenerateBinaryExpression (static_cast<agaASTBinaryOperator *> (node));
			}
			else
			{
				std::string codeLine = node->GetAllocationBlock().GetCode();

				if (node->GetType() == ConstantNode || node->GetType() == VariableNode)
				{
					EmitInstruction (InstructionType::MOV, m_CurrentRegisterIndex,  node);
				}
				
				m_CurrentRegisterIndex++;

			//	AddCodeLine (codeLine);

				std::vector<agaASTNode *> children = node->GetChildren();

				for (int i = 0; i < children.size(); ++i)
				{
					GenerateCode (children[i]);
				}
			}

		return m_Code;
	}

	//--------------------------------------------------------------------------------

	std::string ToString (int value)
	{
		std::stringstream sstream;

		sstream << value;

		return sstream.str();
	}

	//--------------------------------------------------------------------------------

	void agaCodeGenerator::GenerateAssignment (agaASTAssignment *node)
	{
		agaASTExpression *expression = node->GetExpression();

		expression->Evaluate();

		GenerateCode (expression);

		node->Evaluate();

		EmitInstruction (InstructionType::MOV, node, m_CurrentRegisterIndex - 1);
	}

	//--------------------------------------------------------------------------------

	void agaCodeGenerator::GenerateBinaryExpression (agaASTBinaryOperator *node)
	{
		std::vector<agaASTNode *> children = node->GetChildren();

		node->Evaluate();
		children[0]->Evaluate ();
		children[1]->Evaluate ();

		ASTNodeType leftType = children[0]->GetType();
		ASTNodeType rightType = children[1]->GetType();

		if (leftType == ASTNodeType::BinaryOperationNode)
		{
			GenerateBinaryExpression (static_cast<agaASTBinaryOperator *> (children[0]));
			children[0]->GetAllocationBlock ().SetRegisterIndex (m_CurrentRegisterIndex - 1);
		}

		if (rightType == ASTNodeType::BinaryOperationNode)
		{
			GenerateBinaryExpression (static_cast<agaASTBinaryOperator *> (children[1]));
			children[1]->GetAllocationBlock ().SetRegisterIndex (m_CurrentRegisterIndex - 1);
		}

		if (leftType != ASTNodeType::BinaryOperationNode)
		{
			EmitInstruction (InstructionType::MOV, m_CurrentRegisterIndex, children[0]);
		}
		else
		{
			EmitInstruction (InstructionType::MOV, m_CurrentRegisterIndex, children[0]->GetAllocationBlock().GetRegisterIndex ());
		}

		InstructionType instructionType = GetInstructionTypeFromCode (node->GetAllocationBlock().GetCode());

		if (rightType != ASTNodeType::BinaryOperationNode)
		{
			EmitInstruction (instructionType, m_CurrentRegisterIndex, children[1]);
		}
		else
		{
			EmitInstruction (instructionType, m_CurrentRegisterIndex, children[1]->GetAllocationBlock().GetRegisterIndex ());
		}

		++m_CurrentRegisterIndex;
	}

	//--------------------------------------------------------------------------------

	void agaCodeGenerator::EmitInstruction (InstructionType instruction, int dstRegisterIndex, int srcRegisterIndex)
	{
		std::string codeLine = std::string (instructions[instruction].word) + " #";
		codeLine += ToString (dstRegisterIndex) + ", ";
		codeLine += "#" + ToString (srcRegisterIndex);

		AddCodeLine (codeLine);
	}

	//--------------------------------------------------------------------------------

	void agaCodeGenerator::EmitInstruction (InstructionType instruction, int registerIndex, agaASTNode *node)
	{
		std::string codeLine = std::string (instructions[instruction].word) + " #";
		codeLine += ToString (registerIndex) + ", ";
		codeLine += node->GetAllocationBlock().GetCode();

		AddCodeLine (codeLine);
	}

	//--------------------------------------------------------------------------------

	void agaCodeGenerator::EmitInstruction (InstructionType instruction, agaASTNode *node, int registerIndex)
	{
		std::string codeLine = std::string (instructions[instruction].word);
		codeLine += " " + node->GetAllocationBlock().GetCode();
		codeLine +=  ", #" + ToString (registerIndex);


		AddCodeLine (codeLine);
	}

	//--------------------------------------------------------------------------------

	InstructionType agaCodeGenerator::GetInstructionTypeFromCode (const std::string &code)
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
