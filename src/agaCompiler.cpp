#include <memory>

#include "agaLang.h"
#include "agaCompiler.h"
#include "agaCodeGenerator.h"
#include "agaParser.h"
#include "agaLogger.h"
#include "agaASTProgram.h"
#include "agaException.h"

namespace aga
{
    //--------------------------------------------------------------------------------

    agaCompiler::agaCompiler() :
        m_Parser (nullptr),
        m_CodeGenerator (nullptr)
    {
    }

    //--------------------------------------------------------------------------------

    agaCompiler::~agaCompiler()
    {
    }

    //--------------------------------------------------------------------------------

    agaASTProgram* agaCompiler::CompileSource (const std::string &code)
    {
        m_Parser = std::unique_ptr<agaParser> (new agaParser (code));

        agaASTProgram *programNode = nullptr;

        try
        {
            programNode = m_Parser->ParseProgram ();
        }
        catch (agaException& e)
        {
            agaLogger::log (e.what ());
        }

        return programNode;
    }

    //--------------------------------------------------------------------------------

    void agaCompiler::GenerateCode(agaASTProgram *programNode)
    {
        if (programNode != nullptr)
        {
            m_CodeGenerator = std::unique_ptr<agaCodeGenerator> (new agaCodeGenerator ());

            std::vector<std::string> generatedCode = m_CodeGenerator.get()->GenerateCode (programNode);

            agaLogger::log("Generated code:");

            for (int i = 0; i < generatedCode.size(); ++i)
            {
                const std::string& lineOfCode = generatedCode[i];

                agaLogger::PrintMemory (i, lineOfCode);
            }
        }
    }

    //--------------------------------------------------------------------------------
}
