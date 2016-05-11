#include "agaASTConstant.h"
#include "agaASTBlock.h"

namespace aga
{
    //--------------------------------------------------------------------------------

    llvm::Value *agaASTConstant::Evaluate (agaCodeGenerator *codeGenerator)
    {
        std::string line = "CONST " + m_Token.GetLiteral ();

        m_AllocationBlock.SetCode (line);

        llvm::LLVMContext &context = codeGenerator->GetModule ().get ()->getContext ();
        llvm::Value *constant = nullptr;

        switch (m_TypeInfo.GetType ())
        {
        case agaTypeInfo::Type::INT:
            constant = llvm::ConstantInt::get (llvm::Type::getInt128Ty(context), m_TypeInfo.GetLong (), true);
            break;

        case agaTypeInfo::Type::Float:
            constant = llvm::ConstantFP::get (llvm::Type::getDoubleTy (context), m_TypeInfo.GetDouble ());
            break;

        case agaTypeInfo::Type::STRING:
            constant = llvm::ConstantDataArray::getString (context, m_TypeInfo.GetString ());
            break;
        }

        return constant;
    }

    //--------------------------------------------------------------------------------
}
