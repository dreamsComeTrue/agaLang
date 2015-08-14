#ifndef _AGA_ASTNODE_H_
#define _AGA_ASTNODE_H_

#include <string>
#include <vector>

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
		agaASTNode (ASTNodeType type, const std::string &code) : m_Type (type), m_Code (code), m_Previous (nullptr) { }

		const std::string &GetCode () const
		{
			return m_Code;
		}

		agaASTNode *GetPrevious ()
		{
			return m_Previous;
		}

		void SetPrevious (agaASTNode *node)
		{
			m_Previous = node;
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

	protected:
		ASTNodeType					m_Type;
		agaASTNode 					*m_Previous;
		std::vector<agaASTNode *> 	m_Children;
		std::string					m_Code;
	};
}

#endif	//	_AGA_ASTNODE_H_
