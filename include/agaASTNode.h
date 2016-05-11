#ifndef _AGA_ASTNODE_H_
#define _AGA_ASTNODE_H_

#include <memory>
#include <string>
#include <vector>

#include "llvm/IR/Value.h"

#include "agaAllocationBlock.h"
#include "agaCodeGenerator.h"
#include "agaException.h"
#include "agaSemanticAnalyzer.h"
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

    class agaASTBlock;

    class agaASTNode
    {
      public:
        agaASTNode (ASTNodeType type, std::shared_ptr<agaASTNode> parentNode) : m_Type (type), m_Parent (parentNode) {}
        virtual ~agaASTNode () {}

        agaASTNode (ASTNodeType type, agaToken token, std::shared_ptr<agaASTNode> parentNode)
            : m_Type (type), m_Token (token), m_Parent (parentNode)
        {
        }

        const ASTNodeType GetType () const { return m_Type; }

        agaAllocationBlock &GetAllocationBlock () { return m_AllocationBlock; }

        const agaToken &GetToken () const { return m_Token; }

        const std::shared_ptr<agaASTBlock> GetParent () { return std::static_pointer_cast<agaASTBlock> (m_Parent); }

        void SetIRType (llvm::Type *type) { m_IRType = type; }

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer) = 0;

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator) = 0;

        virtual const std::string ToString () = 0;

      protected:
        std::shared_ptr<agaASTNode> m_Parent;
        ASTNodeType m_Type;
        agaToken m_Token;
        agaAllocationBlock m_AllocationBlock;
        llvm::Type *m_IRType;
    };
}

#endif //	_AGA_ASTNODE_H_
