#ifndef _AGA_ASTBINARYOPERATOR_H_
#define _AGA_ASTBINARYOPERATOR_H_

#include "agaASTExpression.h"
#include "agaASTConstant.h"
#include "agaInstructions.h"

namespace aga
{
    class agaASTBinaryOperator : public agaASTExpression
    {
    public:
        agaASTBinaryOperator (agaToken token, agaASTNode *left, agaASTNode *right) :
            agaASTExpression (BinaryOperationNode, BinaryOperation, token),
            m_Operator (token.GetLiteral().at (0)), m_Left (left), m_Right (right)
        {
        }

        virtual void Evaluate ()
        {
            long leftLong;
            double leftDouble;
            /*	ExpressionType leftType = VariableExpression;

                long rightLong;
                double rightDouble;
                ExpressionType rightType = VariableExpression;

                if (m_Left->GetType() == IntegerExpression)
                {
                    //	leftLong = ( (agaASTConstant*) m_Left)->GetLongValue();
                    leftType = IntegerExpression;
                }
                else
                    if (m_Left->GetType() == FloatExpression)
                    {
                        //		leftDouble = ( (agaASTConstant*) m_Left)->GetDoubleValue();
                        leftType = FloatExpression;
                    }

                if (m_Right->GetType() == IntegerExpression)
                {
                    //	rightLong = ( (agaASTConstant*) m_Right)->GetLongValue();
                //	rightType = IntegerExpression;
                }
                else
                    if (m_Right->GetType() == FloatExpression)
                    {
                        //		rightDouble = ( (agaASTConstant*) m_Right)->GetDoubleValue();
                        //rightType = Fl;
                    }
            */
            std::string code = "";

            switch (m_Operator)
            {
                case '*':
                    code = instructions[InstructionType::MUL].word;
                    break;

                case '+':
                    code = instructions[InstructionType::ADD].word;
                    break;

                case '-':
                    code = instructions[InstructionType::SUB].word;
                    break;

                case '/':
                    code = instructions[InstructionType::DIV].word;
                    break;

                default
                :
                    break;
            }

            m_AllocationBlock.SetCode (code);

            m_Left->Evaluate ();
            m_Right->Evaluate ();
        }

        agaASTNode* GetLeft ()
        {
            return m_Left;
        }

        agaASTNode* GetRight ()
        {
            return m_Right;
        }

        virtual const std::string ToString ()
        {
            return "(" + m_Left->ToString() + " " + m_Operator + " " +
                    m_Right->ToString() + ")";
        }

    private:
        char		m_Operator;
        agaASTNode	*m_Left;
        agaASTNode	*m_Right;
    };
}

#endif	//	_AGA_ASTBINARYOPERATOR_H_
