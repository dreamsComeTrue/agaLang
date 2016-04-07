#include <sstream>

#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"

#include "agaASTBlock.h"
#include "agaASTProgram.h"
#include "agaCodeGenerator.h"
#include "agaCompiler.h"
#include "agaUtils.h"

namespace aga
{
    //--------------------------------------------------------------------------------

    agaCodeGenerator::agaCodeGenerator () : m_Module (nullptr)
    {
        m_IRBuilder = llvm::make_unique<llvm::IRBuilder<>> (llvm::getGlobalContext ());
    }

    //--------------------------------------------------------------------------------

    agaCodeGenerator::~agaCodeGenerator () {}

    //--------------------------------------------------------------------------------

    std::unique_ptr<llvm::Module> &agaCodeGenerator::GenerateCode (const agaCompiler *compiler,
                                                                         std::unique_ptr<agaASTProgram> program)
    {
        m_Module = llvm::make_unique<llvm::Module> (compiler->GetFileName (), llvm::getGlobalContext ());

        const std::vector<std::unique_ptr<agaASTBlock>> &blocks = program->GetBlocks ();

        for (const std::unique_ptr<agaASTBlock> &block : blocks)
        {
            block->Evaluate (this);
        }

        return m_Module;
    }

    //--------------------------------------------------------------------------------
}
