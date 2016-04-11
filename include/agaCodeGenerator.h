#ifndef _AGA_CODEGENERATOR_H_
#define _AGA_CODEGENERATOR_H_

#include <map>
#include <string>
#include <vector>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

namespace aga
{
    class agaASTProgram;
    class agaCompiler;

    class agaCodeGenerator
    {
      public:
        agaCodeGenerator ();
        ~agaCodeGenerator ();

        std::unique_ptr<llvm::Module> &GenerateCode (const agaCompiler *compiler, std::unique_ptr<agaASTProgram> program);
        std::unique_ptr<llvm::Module> &GetModule () { return m_Module; }
        llvm::IRBuilder<> &GetBuilder () const { return (*m_IRBuilder.get ()); }        

      protected:
        std::unique_ptr<llvm::Module> m_Module;
        std::unique_ptr<llvm::IRBuilder<>> m_IRBuilder;        
    };
}

#endif //	_AGA_CODEGENERATOR_H_
