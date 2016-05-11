#ifndef _AGA_ASTPROGRAM_H_
#define _AGA_ASTPROGRAM_H_

#include <vector>

#include "agaASTBlock.h"

namespace aga
{
    class agaASTProgram : public agaASTNode
    {
      public:
        agaASTProgram (std::shared_ptr<agaASTNode> parentNode) : agaASTNode (ProgramNode, parentNode) {}
        virtual ~agaASTProgram () {}

        void AddBlock (std::shared_ptr<agaASTBlock> &blockNode) { m_Blocks.push_back (blockNode); }

        const std::vector<std::shared_ptr<agaASTBlock>> &GetBlocks () { return m_Blocks; }

        virtual llvm::Value *Evaluate (agaCodeGenerator *codeGenerator)
        {
            for (const std::shared_ptr<agaASTBlock> &block : m_Blocks)
            {
                block->Evaluate (codeGenerator);
            }
        }

        virtual void SemanticCheck (std::shared_ptr<agaSemanticAnalyzer> analyzer) {}

        virtual const std::string ToString () { return "PROGRAM"; }

      private:
        std::vector<std::shared_ptr<agaASTBlock>> m_Blocks;
    };
}

#endif //	_AGA_ASTPROGRAM_H_
