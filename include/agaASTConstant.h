#ifndef _AGA_ASTCONSTANT_H_
#define _AGA_ASTCONSTANT_H_

#include "agaASTExpression.h"
#include "agaToken.h"
#include "agaTypeInfo.h"

namespace aga
{
    class agaASTConstant : public agaASTExpression
    {
      public:
        agaASTConstant (agaToken token, long value, std::shared_ptr<agaASTNode> parentNode)
            : agaASTExpression (ConstantNode, IntegerConst, token, parentNode), m_TypeInfo (value)
        {
        }

        agaASTConstant (agaToken token, double value, std::shared_ptr<agaASTNode> parentNode)
            : agaASTExpression (ConstantNode, FloatConst, token, parentNode), m_TypeInfo (value)
        {
        }

        agaASTConstant (agaToken token, std::string value, std::shared_ptr<agaASTNode> &parentNode)
            : agaASTExpression (ConstantNode, StringConst, token, parentNode), m_TypeInfo (const_cast<char *> (value.c_str ()))
        {
        }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator)
        {
            std::string line = "CONST " + m_Token.GetLiteral ();

            m_AllocationBlock.SetCode (line);

            llvm::LLVMContext &context = codeGenerator->GetModule ().get ()->getContext ();
            llvm::Value *constant = nullptr;

            switch (m_TypeInfo.GetType ())
            {
            case agaTypeInfo::Type::INT:
                constant = llvm::ConstantInt::get (llvm::Type::getInt64Ty (context), m_TypeInfo.GetLong (), true);
                break;

            case agaTypeInfo::Type::FLOAT:
                constant = llvm::ConstantFP::get (llvm::Type::getDoubleTy (context), m_TypeInfo.GetDouble ());
                break;

            case agaTypeInfo::Type::STRING:
                constant = llvm::ConstantDataArray::getString (context, m_TypeInfo.GetString ());
                break;
            }

            return constant;
        }

        virtual const std::string ToString () { return m_Token.GetLiteral (); }

      private:
        agaTypeInfo m_TypeInfo;
    };
}

#endif //	_AGA_ASTCONSTANT_H_
