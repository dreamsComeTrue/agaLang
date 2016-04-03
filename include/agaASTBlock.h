#ifndef _AGA_ASTBLOCK_H_
#define _AGA_ASTBLOCK_H_

#include <vector>

#include "agaASTNode.h"

namespace aga
{
    class agaASTBlock : public agaASTNode
    {
      public:
        agaASTBlock (std::shared_ptr<agaASTBlock> parentBlock) : agaASTNode (BlockNode), m_Parent (parentBlock) {}

        void AddStatement (std::shared_ptr<agaASTNode> node) { m_Statements.push_back (node); }

        const std::vector<std::shared_ptr<agaASTNode>> &GetStatements () { return m_Statements; }

        const std::shared_ptr<agaASTBlock> &GetParent () { return m_Parent; }

        void AddParameter (const std::string &parameter) { m_Parameters.push_back (parameter); }

        const std::vector<std::string> &GetParameters () { return m_Parameters; }

        void SetName (const std::string &name) { m_Name = name; }

        const std::string &GetName () const { return m_Name; }

        void SetReturnExpr (std::shared_ptr<agaASTNode> returnExpr) { m_ReturnExpr = returnExpr; }

        std::shared_ptr<agaASTNode> GetReturnExpr () { return m_ReturnExpr; }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator)
        {
            llvm::FunctionType *funcType = llvm::FunctionType::get (llvm::Type::getVoidTy (llvm::getGlobalContext ()), false);
            llvm::Function *function = llvm::Function::Create (funcType, llvm::Function::LinkageTypes::InternalLinkage, m_Name,
                                                               codeGenerator->GetModule ().get ());

            // Set names for all arguments.
            unsigned idx = 0;
            for (auto &arg : function->args ())
            {
                arg.setName (m_Parameters[idx++]);
            }

            llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create (llvm::getGlobalContext (), "block", function);
            codeGenerator->GetBuilder ().SetInsertPoint (basicBlock);

            for (std::shared_ptr<agaASTNode> &statement : m_Statements)
            {
                statement->Evaluate (codeGenerator);
            }

            return function;
        }

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
        std::shared_ptr<agaASTBlock> m_Parent;
        std::string m_Name;
        std::vector<std::string> m_Parameters;
        std::vector<std::shared_ptr<agaASTNode>> m_Statements;
        std::shared_ptr<agaASTNode> m_ReturnExpr;
    };
}

#endif //	_AGA_ASTBLOCK_H_
