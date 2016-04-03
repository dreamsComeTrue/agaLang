#ifndef _AGA_ASTPROGRAM_H_
#define _AGA_ASTPROGRAM_H_

#include <vector>

#include "agaASTBlock.h"

namespace aga
{
    class agaASTProgram : public agaASTNode
    {
      public:
        agaASTProgram () : agaASTNode (ProgramNode) {}

        void AddBlock (std::shared_ptr<agaASTBlock> blockNode) { m_Blocks.push_back (blockNode); }

        const std::vector<std::shared_ptr<agaASTBlock>> &GetBlocks () { return m_Blocks; }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator) { return nullptr; }

        virtual const std::string ToString () { return "KOT"; }

      private:
        std::vector<std::shared_ptr<agaASTBlock>> m_Blocks;
    };
}

#endif //	_AGA_ASTPROGRAM_H_
