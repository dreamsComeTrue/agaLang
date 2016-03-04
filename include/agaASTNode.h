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
        BlockNode,
        FunctionCallNode,
        ConstantNode,
        VariableNode,
        AssignmentNode,
        ExpressionNode,
        BinaryOperationNode,
        BooleanRelationNode,
        LogicalRelationNode
    };

    struct ASTNodeWord
    {
        const char   *word;
        ASTNodeType   nodeType;
    };

    ASTNodeWord const nodeWords[] =
    {
        {"ProgramNode"        , ProgramNode},
        {"BlockNode"          , BlockNode},
        {"FunctionCallNode"   , FunctionCallNode},
        {"ConstantNode"       , ConstantNode},
        {"VariableNode"       , VariableNode},
        {"AssignmentNode"     , AssignmentNode},
    };

    class agaASTNode
    {
    public:
        agaASTNode (ASTNodeType type) :
            m_Type (type) { }

        agaASTNode (ASTNodeType type, agaToken token) :
            m_Type (type), m_Token (token) { }

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

        virtual const std::string ToString () = 0;

    protected:
        ASTNodeType					m_Type;
        agaToken					m_Token;
        agaAllocationBlock			m_AllocationBlock;
    };
}

#endif	//	_AGA_ASTNODE_H_
