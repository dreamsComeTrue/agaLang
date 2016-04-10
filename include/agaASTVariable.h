#ifndef _AGA_ASTVARIABLE_H_
#define _AGA_ASTVARIABLE_H_

#include <string>

#include "agaASTExpression.h"
#include "agaToken.h"

namespace aga
{
    class agaASTVariable : public agaASTExpression
    {
      public:
        agaASTVariable (agaToken token, std::shared_ptr<agaASTNode> &parentNode)
            : agaASTExpression (VariableNode, Variable, token, parentNode)
        {
        }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator)
        {
            std::string line = "VAR " + m_Token.GetLiteral ();

            m_AllocationBlock.SetCode (line);

            std::map<std::string, llvm::AllocaInst *> &namedValues = codeGenerator->GetNamedValues ();

            if (namedValues.find (m_Token.GetLiteral ()) == namedValues.end ())
            {
                llvm::Type *type = llvm::Type::getInt64Ty (codeGenerator->GetModule ().get ()->getContext ());
                llvm::AllocaInst *allocValue = codeGenerator->GetBuilder ().CreateAlloca (type, nullptr, m_Token.GetLiteral ());

                namedValues[m_Token.GetLiteral ()] = allocValue;
            }

            llvm::Value *load =
                codeGenerator->GetBuilder ().CreateLoad (namedValues[m_Token.GetLiteral ()], m_Token.GetLiteral ());

            return load;
        }

        virtual const std::string ToString () { return m_Token.GetLiteral (); }

      private:
    };
}

#endif //	_AGA_ASTVARIABLE_H_
