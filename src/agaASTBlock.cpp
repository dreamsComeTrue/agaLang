#include "agaASTBlock.h"

namespace aga
{
    //--------------------------------------------------------------------------------

    std::shared_ptr<agaSymbol> agaASTBlock::GetSymbol (const std::string &name)
    {
        for (std::shared_ptr<agaSymbol> &symbol : m_Symbols)
        {
            if (symbol->GetName () == name)
            {
                return symbol;
            }
        }

        return nullptr;
    }

    //--------------------------------------------------------------------------------

    void agaASTBlock::PutSymbol (const std::string &name, llvm::AllocaInst *value, agaASTNode *parent)
    {
        if (GetSymbol (name) == nullptr)
        {
            m_Symbols.push_back (std::make_shared<agaSymbol> (name, value, parent));
        }
        else
        {
            for (std::shared_ptr<agaSymbol> &symbol : m_Symbols)
            {
                if (symbol->GetName () == name)
                {
                    symbol->SetValue (value);
                    symbol->SetParent (parent);

                    break;
                }
            }
        }
    }

    //--------------------------------------------------------------------------------

    llvm::Value *agaASTBlock::Evaluate (agaCodeGenerator *codeGenerator)
    {
        llvm::FunctionType *protoFuncType = llvm::FunctionType::get (llvm::Type::getVoidTy (llvm::getGlobalContext ()), false);
        llvm::Function *protoFunction = llvm::Function::Create (protoFuncType, llvm::Function::LinkageTypes::CommonLinkage);

        // Set names for all arguments.
        unsigned idx = 0;
        for (auto &arg : protoFunction->args ())
        {
            arg.setName (m_Parameters[idx++]);
        }

        llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create (llvm::getGlobalContext (), "block", protoFunction);
        llvm::IRBuilder<> &builder = codeGenerator->GetBuilder ();

        builder.SetInsertPoint (basicBlock);

        llvm::Value *retVal = nullptr;

        for (std::unique_ptr<agaASTNode> &statement : m_Statements)
        {
            retVal = statement->Evaluate (codeGenerator);
        }

        if (m_ReturnExpr.get () != nullptr)
        {
            retVal = m_ReturnExpr->Evaluate (codeGenerator);
        }

        builder.CreateRet (retVal);

        llvm::FunctionType *funcType = llvm::FunctionType::get (retVal->getType (), false);
        llvm::Function *function = llvm::Function::Create (funcType, llvm::Function::LinkageTypes::CommonLinkage, m_Name,
                                                           codeGenerator->GetModule ().get ());

        function->getBasicBlockList ().splice (function->begin (), protoFunction->getBasicBlockList ());

        return function;
    }

    //--------------------------------------------------------------------------------

    const std::string agaASTBlock::ToString ()
    {
        std::string result;

        if (m_Name != "")
        {
            result = m_Name;

            for (const std::string &parameter : m_Parameters)
            {
                result += " " + parameter;
            }
        }
        else
        {
            result = "[none]";
        }

        result += ":";

        return result;
    }

    //--------------------------------------------------------------------------------
}
