#ifndef _AGA_COMPILER_H_
#define _AGA_COMPILER_H_

#include <memory>
#include <string>

#include "agaASTProgram.h"

namespace aga
{
    class agaParser;
    class agaCodeGenerator;

    class agaCompiler
    {
      public:
        agaCompiler ();

        ~agaCompiler ();

        std::unique_ptr<agaASTProgram> CompileSource (const std::string &fileName, const std::string &code);
        void GenerateCode (std::unique_ptr<agaASTProgram> programNode);
        const std::string &GetFileName () const;
        void Execute () const;

      private:
        std::string m_FileName;
        std::unique_ptr<agaParser> m_Parser;
        std::unique_ptr<agaCodeGenerator> m_CodeGenerator;
    };
}

#endif //	_AGA_COMPILER_H_
