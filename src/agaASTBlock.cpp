#include "agaASTBlock.h"
#include "agaASTBlockParameter.h"

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

    std::shared_ptr<agaASTBlockParameter> agaASTBlock::GetParameter (const std::string &name)
    {
        for (std::shared_ptr<agaASTBlockParameter> &param : m_Parameters)
        {
            if (param->GetName () == name)
            {
                return param;
            }
        }

        return nullptr;
    }

    //--------------------------------------------------------------------------------

    void agaASTBlock::PutSymbol (const std::string &name, agaASTNode *node, llvm::AllocaInst *value, agaASTNode *parent)
    {
        if (GetSymbol (name) == nullptr)
        {
            m_Symbols.push_back (std::make_shared<agaSymbol> (name, node, value, parent));
        }
        else
        {
            for (std::shared_ptr<agaSymbol> &symbol : m_Symbols)
            {
                if (symbol->GetName () == name)
                {
                    symbol->SetValue (value);
                    symbol->SetNode (node);
                    symbol->SetParent (parent);

                    break;
                }
            }
        }
    }

    //--------------------------------------------------------------------------------

    llvm::Value *agaASTBlock::Evaluate (agaCodeGenerator *codeGenerator)
    {
        std::vector<llvm::Type *> params;

        for (std::shared_ptr<agaASTBlockParameter> &param : m_Parameters)
        {
            llvm::Type *type = param->GetTypeInfo ().GetLLVMType ();

            params.push_back (type);
        }

        llvm::FunctionType *protoFuncType = llvm::FunctionType::get (GetReturnType ().GetLLVMType (), params, false);
        llvm::Function *protoFunction = llvm::Function::Create (protoFuncType, llvm::Function::LinkageTypes::CommonLinkage);

        llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create (llvm::getGlobalContext (), "block", protoFunction);
        llvm::IRBuilder<> &builder = codeGenerator->GetBuilder ();

        builder.SetInsertPoint (basicBlock);

        llvm::Value *retVal = nullptr;

        for (std::shared_ptr<agaASTNode> &statement : m_Statements)
        {
            retVal = statement->Evaluate (codeGenerator);
        }

        if (m_ReturnExpr.get () != nullptr)
        {
            retVal = m_ReturnExpr->Evaluate (codeGenerator);
            builder.CreateRet (retVal);
        }

        llvm::FunctionType *funcType = llvm::FunctionType::get (GetReturnType ().GetLLVMType (), params, false);
        llvm::Function *function = llvm::Function::Create (funcType, llvm::Function::LinkageTypes::CommonLinkage, m_Name,
                                                           codeGenerator->GetModule ().get ());

        function->getBasicBlockList ().splice (function->begin (), protoFunction->getBasicBlockList ());

        // Set names for all arguments.
        unsigned idx = 0;
        for (llvm::Argument &arg : function->args ())
        {
            arg.setName (m_Parameters[idx++]->GetName ());
        }

        return function;
    }

    //--------------------------------------------------------------------------------

    const std::string agaASTBlock::ToString ()
    {
        std::string result;

        if (m_Name != "")
        {
            result = m_Name;

            for (const std::shared_ptr<agaASTBlockParameter> &parameter : m_Parameters)
            {
                result += " " + parameter->GetName ();
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
