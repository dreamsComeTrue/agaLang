#ifndef _AGA_ASTPROGRAM_H_
#define _AGA_ASTPROGRAM_H_

#include <vector>

#include "agaASTBlock.h"

namespace aga
{
    class agaASTProgram : public agaASTNode
    {
    public:
        agaASTProgram () :
            agaASTNode (ProgramNode) { }

        void AddBlock (agaASTBlock *blockNode)
        {
            m_Blocks.push_back (blockNode);
        }

        const std::vector<agaASTBlock *>& GetBlocks ()
        {
            return m_Blocks;
        }

        virtual void Evaluate ()
        {
        }

        virtual const std::string ToString ()
        {
            return "KOT";
        }

    private:
        std::vector<agaASTBlock *> m_Blocks;
    };
}

#endif	//	_AGA_ASTPROGRAM_H_
