#ifndef _AGA_ASTNODE_H_
#define _AGA_ASTNODE_H_

#include <string>
#include <vector>

#include "agaAllocationBlock.h"
#include "agaToken.h"

namespace aga
{
	enum ASTNodeType
	{
	    ProgramNode,
	    ConstantNode,
	    VariableNode,
		AssignmentNode,
	    ExpressionNode,
	    BinaryOperationNode,
		BooleanRelationNode,
		LogicalRelationNode
	};

	class agaASTNode
	{
	public:
		agaASTNode (ASTNodeType type) :
			m_Type (type) { }

		agaASTNode (ASTNodeType type, agaToken token) :
			m_Type (type), m_Token (token) { }

		std::vector<agaASTNode *> GetChildren ()
		{
			return m_Children;
		}

		void AddChild (agaASTNode *child)
		{
			m_Children.push_back (child);
		}

		const ASTNodeType GetType () const
		{
			return m_Type;
		}

		agaAllocationBlock &GetAllocationBlock ()
		{
			return m_AllocationBlock;
		}

		const agaToken &GetToken () const
		{
			return m_Token;
		}

		virtual void Evaluate () = 0;

	protected:
		ASTNodeType					m_Type;
		agaToken					m_Token;
		agaAllocationBlock			m_AllocationBlock;
		std::vector<agaASTNode *> 	m_Children;
	};
}

#endif	//	_AGA_ASTNODE_H_
