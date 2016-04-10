#include <memory>

#if 0
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#endif

#include "llvm/IR/LLVMContext.h"

#include "agaCodeGenerator.h"
#include "agaCompiler.h"
#include "agaException.h"
#include "agaLogger.h"
#include "agaParser.h"

namespace aga
{
    //--------------------------------------------------------------------------------

    agaCompiler::agaCompiler () : m_Parser (nullptr), m_CodeGenerator (nullptr) {}

    //--------------------------------------------------------------------------------

    agaCompiler::~agaCompiler () {}

    //--------------------------------------------------------------------------------

    std::unique_ptr<agaASTProgram> agaCompiler::CompileSource (const std::string &fileName, const std::string &code)
    {
        m_FileName = fileName;
        m_Parser = std::make_unique<agaParser> (code);

        std::unique_ptr<agaASTProgram> programNode = nullptr;

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

    void agaCompiler::GenerateCode (std::unique_ptr<agaASTProgram> programNode)
    {
        if (programNode != nullptr)
        {
            m_CodeGenerator = std::make_unique<agaCodeGenerator> ();
            m_CodeGenerator->GenerateCode (this, std::move (programNode));
            m_CodeGenerator->GetModule ()->dump ();
        }
    }

    //--------------------------------------------------------------------------------

    void agaCompiler::Execute () const
    {
#if 0
        llvm::InitializeNativeTarget ();

        std::unique_ptr<llvm::Module> &module = m_CodeGenerator->GetModule ();

        // Now we going to create JIT
        std::string errStr;
        llvm::Module *modulePtr = module.get ();
        llvm::ExecutionEngine *EE = llvm::EngineBuilder (std::move (module)).setErrorStr (&errStr).create ();

        if (!EE)
        {
            llvm::errs () << ": Failed to construct ExecutionEngine: " << errStr << "\n";
            return;
        }

        llvm::errs () << "verifying... ";

        if (llvm::verifyModule (*modulePtr, &llvm::errs ()))
        {
            llvm::errs () << ": Error constructing function!\n";
            return;
        }

        llvm::errs () << "OK\n";
        llvm::errs () << "We just constructed this LLVM module:\n\n---------\n";

        // Call the Fibonacci function with argument n:
        llvm::ArrayRef<llvm::GenericValue> Args;
        llvm::Function *func = module->getFunction ("main");
        llvm::GenericValue GV = EE->runFunction (func, Args);

        // import result of execution
        llvm::outs () << "Result: " << GV.IntVal << "\n";
#endif
    }

    //--------------------------------------------------------------------------------

    const std::string &agaCompiler::GetFileName () const { return m_FileName; }

    //--------------------------------------------------------------------------------
}
