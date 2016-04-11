#ifndef _AGA_ASTBLOCK_H_
#define _AGA_ASTBLOCK_H_

#include <vector>

#include "agaASTNode.h"
#include "agaSymbol.h"

namespace aga
{
    class agaASTBlock : public agaASTNode
    {
      public:
        agaASTBlock (std::shared_ptr<agaASTNode> parentNode) : agaASTNode (BlockNode, parentNode) {}

        void AddStatement (std::unique_ptr<agaASTNode> &node) { m_Statements.push_back (std::move (node)); }

        const std::vector<std::unique_ptr<agaASTNode>> &GetStatements () { return m_Statements; }

        void AddParameter (const std::string &parameter) { m_Parameters.push_back (parameter); }

        const std::vector<std::string> &GetParameters () { return m_Parameters; }

        void SetName (const std::string &name) { m_Name = name; }

        const std::string &GetName () const { return m_Name; }

        void SetReturnExpr (std::unique_ptr<agaASTNode> returnExpr) { m_ReturnExpr = std::move (returnExpr); }

        const std::unique_ptr<agaASTNode> &GetReturnExpr () { return m_ReturnExpr; }

        std::vector<std::shared_ptr<agaSymbol>> &GetSymbols () { return m_Symbols; }

        std::shared_ptr<agaSymbol> GetSymbol (const std::string &name)
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

        void PutSymbol (const std::string &name, llvm::AllocaInst *value, agaASTNode *parent)
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

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator)
        {
            llvm::FunctionType *protoFuncType =
                llvm::FunctionType::get (llvm::Type::getVoidTy (llvm::getGlobalContext ()), false);
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

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer) {}

        virtual const std::string ToString ()
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

      private:
        std::string m_Name;
        std::vector<std::string> m_Parameters;
        std::vector<std::unique_ptr<agaASTNode>> m_Statements;
        std::unique_ptr<agaASTNode> m_ReturnExpr;
        std::vector<std::shared_ptr<agaSymbol>> m_Symbols;
    };
}

#endif //	_AGA_ASTBLOCK_H_
