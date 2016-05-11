#ifndef _AGA_ASTFUNCTIONCALL_H_
#define _AGA_ASTFUNCTIONCALL_H_

#include <vector>

#include "agaASTNode.h"

namespace aga
{
    class agaASTFunctionCall : public agaASTNode
    {
      public:
        agaASTFunctionCall (std::shared_ptr<agaASTNode> &parentNode) : agaASTNode (FunctionCallNode, parentNode) {}

        void AddParameter (std::shared_ptr<agaASTNode> node) { m_Parameters.push_back (node); }

        const std::vector<std::shared_ptr<agaASTNode>> &GetParameters () { return m_Parameters; }

        void SetName (const std::string &name) { m_Name = name; }

        const std::string &GetName () const { return m_Name; }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator);

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer) {}

        virtual const std::string ToString ()
        {
            std::string result = "[ " + m_Name;

            for (const std::shared_ptr<agaASTNode> &param : m_Parameters)
            {
                result += " " + param->ToString ();
            }

            result += " ]";

            return result;
        }

      private:
        std::string m_Name;
        std::vector<std::shared_ptr<agaASTNode>> m_Parameters;
    };
}

#endif //	_AGA_ASTFUNCTIONCALL_H_
