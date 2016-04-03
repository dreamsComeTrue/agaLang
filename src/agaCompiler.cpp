#include <memory>

#include "agaASTProgram.h"
#include "agaCodeGenerator.h"
#include "agaCompiler.h"
#include "agaException.h"
#include "agaLang.h"
#include "agaLogger.h"
#include "agaParser.h"

namespace aga
{
    //--------------------------------------------------------------------------------

    agaCompiler::agaCompiler () : m_Parser (nullptr), m_CodeGenerator (nullptr) {}

    //--------------------------------------------------------------------------------

    agaCompiler::~agaCompiler () {}

    //--------------------------------------------------------------------------------

    std::shared_ptr<agaASTProgram> agaCompiler::CompileSource (const std::string &fileName, const std::string &code)
    {
        m_FileName = fileName;
        m_Parser = std::unique_ptr<agaParser> (new agaParser (code));

        std::shared_ptr<agaASTProgram> programNode = nullptr;

        try
        {
            programNode = m_Parser->ParseProgram ();
        }
        catch (agaException &e)
        {
            agaLogger::log (e.what ());
        }

        return programNode;
    }

    //--------------------------------------------------------------------------------

    void agaCompiler::GenerateCode (std::shared_ptr<agaASTProgram> programNode)
    {
        if (programNode != nullptr)
        {
            m_CodeGenerator = std::unique_ptr<agaCodeGenerator> (new agaCodeGenerator ());

            m_CodeGenerator.get ()->GenerateCode (this, programNode);
        }
    }

    //--------------------------------------------------------------------------------

    const std::string &agaCompiler::GetFileName () const { return m_FileName; }

    //--------------------------------------------------------------------------------
}
