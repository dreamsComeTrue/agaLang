#include "agaASTFunctionCall.h"
#include "agaASTBlock.h"

namespace aga
{
    //--------------------------------------------------------------------------------

    llvm::Value *agaASTFunctionCall::Evaluate (agaCodeGenerator *codeGenerator)
    {
        llvm::IRBuilder<> &builder = codeGenerator->GetBuilder ();

        if (llvm::Function *func = codeGenerator->GetModule ()->getFunction (m_Name))
        {
            std::vector<llvm::Value *> args;

            for (std::shared_ptr<agaASTNode> &param : m_Parameters)
            {
                llvm::Value *paramValue = param->Evaluate (codeGenerator);

                args.push_back (paramValue);
            }

            llvm::CallInst *call = builder.CreateCall (func, args, "call_" + m_Name);

            return call;
        }
        else
        {
            return nullptr;
        }
    }

    //--------------------------------------------------------------------------------
}
