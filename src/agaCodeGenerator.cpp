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
		agaASTNode *rootNode = new agaASTNode (ASTNodeType::Program, "");
		agaASTNode *resultNode = expression->Evaluate (rootNode);

		GenerateCode (resultNode);

		return m_Code;
	}

	//--------------------------------------------------------------------------------

	void agaCodeGenerator::GenerateCode (agaASTNode *node)
	{
		std::string codeLine = node->GetCode();
		std::vector<agaASTNode *> children = node->GetChildren();

		if (CheckNodesAreFinal (children))
		{
			for (int i = 0; i < children.size(); ++i)
			{
				agaASTNode *child = children[i];

				codeLine += " " + child->GetCode();
				
				if (i < children.size() - 1)
				{
					codeLine += ",";
				}
			}

			AddCodeLine (codeLine);
		}
		else
		{
			AddCodeLine (codeLine);

			for (int i = 0; i < children.size(); ++i)
			{
				agaASTNode *child = children[i];

				GenerateCode (child);
			}
		}
	}

	//--------------------------------------------------------------------------------

	bool agaCodeGenerator::CheckNodesAreFinal (std::vector<agaASTNode *> children)
	{
		for (int i = 0; i < children.size(); ++i)
		{
			agaASTNode *child = children[i];

			if (child->GetType() == ASTNodeType::BinaryOperation)
			{
				return false;
			}
		}

		return true;
	}


	//--------------------------------------------------------------------------------
}
