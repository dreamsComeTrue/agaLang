#ifndef _AGA_ASTVARIABLE_H_
#define _AGA_ASTVARIABLE_H_

#include <string>

#include "agaASTBlock.h"
#include "agaASTExpression.h"
#include "agaToken.h"

namespace aga
{
    class agaASTVariable : public agaASTExpression
    {
      public:
        agaASTVariable (agaToken token, std::shared_ptr<agaASTNode> &parentNode)
            : agaASTExpression (VariableNode, VariableExpression, token, parentNode)
        {
        }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator)
        {
            std::string name = m_Token.GetLiteral ();
            std::string line = "VAR " + name;

            m_AllocationBlock.SetCode (line);

            std::shared_ptr<agaASTBlock> block = GetBlock ();

            if (block->GetSymbol (name) == nullptr || block->GetSymbol (name)->GetValue () == nullptr)
            {
                llvm::AllocaInst *allocValue = codeGenerator->GetBuilder ().CreateAlloca (m_IRType, nullptr, name);

                block->PutSymbol (name, allocValue, this);
            }

            llvm::Value *load = codeGenerator->GetBuilder ().CreateLoad (block->GetSymbol (name)->GetValue (), name);

            return load;
        }

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer)
        {
            const std::shared_ptr<agaASTBlock> &block = analyzer->GetEnclosingBlock ();
            std::string name = m_Token.GetLiteral ();

            if (analyzer->GetPhase () == VariablePhase)
            {
                block->PutSymbol (name, nullptr, nullptr);
            }
            else
            {
                if (!block->GetSymbol (name))
                {
                    throw agaException (m_Token.GetInfo () + ": Variable '" + name + "' does not exist in current scope!");
                }
            }
        }

        virtual const std::string ToString () { return m_Token.GetLiteral (); }

      private:
    };
}

#endif //	_AGA_ASTVARIABLE_H_
