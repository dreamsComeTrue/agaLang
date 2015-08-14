#ifndef _AGA_ASTNODE_H_
#define _AGA_ASTNODE_H_

#include <string>
#include <vector>

#include "agaAllocationBlock.h"

namespace aga
{
	enum ASTNodeType
	{
	    Program,
	    Constant,
	    Variable,
	    BinaryOperation
	};

	class agaASTNode
	{
	public:
		agaASTNode (ASTNodeType type, agaASTNode *parent, const std::string &code) :
			m_Type (type),
			m_AllocationBlock (code),
			m_Parent (parent) { }

		agaASTNode *GetParent ()
		{
			return m_Parent;
		}

		void SetParent (agaASTNode *node)
		{
			m_Parent = node;
		}

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

	protected:
		ASTNodeType					m_Type;
		agaAllocationBlock			m_AllocationBlock;
		agaASTNode 					*m_Parent;
		std::vector<agaASTNode *> 	m_Children;
	};
}

#endif	//	_AGA_ASTNODE_H_
