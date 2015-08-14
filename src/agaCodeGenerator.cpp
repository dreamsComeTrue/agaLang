#include <sstream>

#include "agaCodeGenerator.h"
#include "agaASTExpression.h"

namespace aga
{
	//--------------------------------------------------------------------------------

	agaCodeGenerator::agaCodeGenerator()
	{

	}

	//--------------------------------------------------------------------------------

	void agaCodeGenerator::AddCodeLine (const std::string &codeLine)
	{
		m_Code += codeLine + "\n";
	}

	//--------------------------------------------------------------------------------

	const std::string &agaCodeGenerator::GenerateCode (agaASTExpression *expression)
	{
		agaASTNode *rootNode = new agaASTNode (ASTNodeType::Program, nullptr, "");
		rootNode->SetParent (rootNode);

		agaASTNode *resultNode = expression->Evaluate (rootNode);

		GenerateCode (resultNode);

		return m_Code;
	}

	//--------------------------------------------------------------------------------

	void agaCodeGenerator::GenerateCode (agaASTNode *node)
	{
		std::vector<agaASTNode *> children = node->GetChildren();

		if (node->GetType() == ASTNodeType::BinaryOperation)
		{
			GenerateBinaryExpression (node);
		}
		else
		{
			std::string codeLine = node->GetAllocationBlock().GetCode();

			AddCodeLine (codeLine);

			for (int i = 0; i < children.size(); ++i)
			{
				GenerateCode (children[i]);
			}
		}
	}

	int currentRegisterIndex = 1;

	//--------------------------------------------------------------------------------

	std::string ToString (int value)
	{
		std::stringstream sstream;

		sstream << value;

		return sstream.str();
	}

	void agaCodeGenerator::GenerateBinaryExpression (agaASTNode *node)
	{
		std::vector<agaASTNode *> children = node->GetChildren();

		ASTNodeType leftType = children[0]->GetType();
		ASTNodeType rightType = children[1]->GetType();

		if (leftType == ASTNodeType::BinaryOperation)
		{			
			GenerateBinaryExpression (children[0]);
			children[0]->GetAllocationBlock ().SetRegisterIndex (currentRegisterIndex-1);
		}

		if (rightType == ASTNodeType::BinaryOperation)
		{
			GenerateBinaryExpression (children[1]);
			children[1]->GetAllocationBlock ().SetRegisterIndex (currentRegisterIndex-1);
		}

		std::string codeLine = "MOV #";

		codeLine += ToString (currentRegisterIndex) + ", ";

		if (leftType != ASTNodeType::BinaryOperation)
		{
			codeLine += children[0]->GetAllocationBlock().GetCode();
		}
		else
		{
			codeLine += "#" + ToString (children[0]->GetAllocationBlock().GetRegisterIndex ());
		}

		AddCodeLine (codeLine);

		codeLine = node->GetAllocationBlock().GetCode();
		codeLine += " #" + ToString (currentRegisterIndex) + ", ";

		if (rightType != ASTNodeType::BinaryOperation)
		{
			codeLine += children[1]->GetAllocationBlock().GetCode();
		}
		else
		{
			codeLine += "#" + ToString (children[1]->GetAllocationBlock().GetRegisterIndex ());
		}

		AddCodeLine (codeLine);

		++currentRegisterIndex;
	}

	//--------------------------------------------------------------------------------

}
