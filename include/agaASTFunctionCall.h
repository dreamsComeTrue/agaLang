#ifndef _AGA_ASTFUNCTIONCALL_H_
#define _AGA_ASTFUNCTIONCALL_H_

#include <vector>

#include "agaASTNode.h"

namespace aga
{
    class agaASTFunctionCall : public agaASTNode
    {
    public:
        agaASTFunctionCall () :
            agaASTNode (FunctionCallNode) { }

        void AddParameter (agaASTNode *node)
        {
            m_Parameters.push_back (node);
        }

        const std::vector<agaASTNode *>& GetParameters ()
        {
            return m_Parameters;
        }

        void SetName (const std::string& name)
        {
            m_Name = name;
        }

        const std::string& GetName () const
        {
            return m_Name;
        }

        virtual void Evaluate ()
        {
        }

        virtual const std::string ToString ()
        {
            std::string result = "[ " + m_Name;

            for (agaASTNode* param : m_Parameters)
            {
                result += " " + param->ToString();
            }

            result += " ]";

            return result;
        }

    private:
        std::string               m_Name;
        std::vector<agaASTNode *> m_Parameters;
    };
}

#endif	//	_AGA_ASTFUNCTIONCALL_H_
