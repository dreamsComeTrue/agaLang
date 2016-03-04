#ifndef _AGA_ASTASSIGNMENT_H_
#define _AGA_ASTASSIGNMENT_H_

#include "agaASTExpression.h"

namespace aga
{
    class agaASTAssignment : public agaASTExpression
    {
    public:
        agaASTAssignment (agaToken token, agaASTExpression *expression) :
            agaASTExpression (ASTNodeType::AssignmentNode, ExpressionType::Assignment, token),
            m_Expression (expression)
        {}

        agaASTExpression* GetExpression () const
        {
            return m_Expression;
        }

        virtual void Evaluate ()
        {
            std::string code = m_Token.GetLiteral();

            GetAllocationBlock().SetCode (code);
        }

        virtual const std::string ToString ()
        {
            return "KOT";
        }

    protected:
        agaASTExpression *m_Expression;
    };
}

#endif	//	_AGA_ASTASSIGNMENT_H_
