#ifndef _AGA_ASTEXPRESSION_H_
#define _AGA_ASTEXPRESSION_H_

#include "agaASTNode.h"

namespace aga
{
    enum ExpressionType
    {
        Assignment,
        BinaryOperation,
        BooleanRelation,
        LogicalRelation,
        IntegerConst,
        FloatConst,
        StringConst,
        Variable
    };

    class agaASTExpression : public agaASTNode
    {
    public:
        agaASTExpression (ASTNodeType nodeType, ExpressionType expressionType, agaToken token) :
            agaASTNode (nodeType, token),
            m_Type (expressionType) { }

        ExpressionType GetExpressionType ()
        {
            return m_Type;
        }

        virtual llvm::Value* Evaluate (agaCodeGenerator* codeGenerator) = 0;

        virtual const std::string ToString ()
        {
            return "KOT";
        }

    protected:
        ExpressionType	m_Type;
    };
}

#endif	//	_AGA_ASTEXPRESSION_H_
