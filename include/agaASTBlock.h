#ifndef _AGA_ASTBLOCK_H_
#define _AGA_ASTBLOCK_H_

#include <vector>

#include "agaASTNode.h"

namespace aga
{
    class agaASTBlock : public agaASTNode
    {
    public:
        agaASTBlock (agaASTBlock* parentBlock) :
            agaASTNode (BlockNode), m_Parent (parentBlock) { }

        void AddStatement (agaASTNode *node)
        {
            m_Statements.push_back (node);
        }

        const std::vector<agaASTNode *>& GetStatements ()
        {
            return m_Statements;
        }

        const agaASTNode* GetParent ()
        {
            return m_Parent;
        }

        void AddParameter (const std::string& parameter)
        {
            m_Parameters.push_back(parameter);
        }

        const std::vector<std::string>& GetParameters ()
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
            return "KOT";
        }

    private:
        agaASTNode*               m_Parent;
        std::string               m_Name;
        std::vector<std::string>  m_Parameters;
        std::vector<agaASTNode *> m_Statements;
    };
}

#endif	//	_AGA_ASTBLOCK_H_
