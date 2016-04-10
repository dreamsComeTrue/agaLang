#ifndef _AGA_ASTNODE_H_
#define _AGA_ASTNODE_H_

#include <string>
#include <vector>

#include "llvm/IR/Value.h"

#include "agaAllocationBlock.h"
#include "agaCodeGenerator.h"
#include "agaToken.h"

namespace aga
{
    enum ASTNodeType
    {
        ProgramNode,
        BlockNode,
        FunctionCallNode,
        MatchNode,
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
        const char *word;
        ASTNodeType nodeType;
    };

    ASTNodeWord const nodeWords[] = {
        {"ProgramNode", ProgramNode},       {"BlockNode", BlockNode},       {"FunctionCallNode", FunctionCallNode},
        {"MatchNode", MatchNode},           {"ConstantNode", ConstantNode}, {"VariableNode", VariableNode},
        {"AssignmentNode", AssignmentNode},
    };

    class agaASTNode
    {
      public:
        agaASTNode (ASTNodeType type, std::shared_ptr<agaASTNode> parentNode) : m_Type (type), m_Parent (parentNode) {}
        virtual ~agaASTNode () {}

        agaASTNode (ASTNodeType type, agaToken token, std::shared_ptr<agaASTNode> parentNode)
            : m_Type (type), m_Token (token), m_Parent (parentNode)
        {
        }

        const std::shared_ptr<agaASTNode> GetParent () const { return m_Parent; }

        const ASTNodeType GetType () const { return m_Type; }

        agaAllocationBlock &GetAllocationBlock () { return m_AllocationBlock; }

        const agaToken &GetToken () const { return m_Token; }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator) = 0;

        virtual const std::string ToString () = 0;

      protected:
        std::shared_ptr<agaASTNode> m_Parent;
        ASTNodeType m_Type;
        agaToken m_Token;
        agaAllocationBlock m_AllocationBlock;
    };
}

#endif //	_AGA_ASTNODE_H_
