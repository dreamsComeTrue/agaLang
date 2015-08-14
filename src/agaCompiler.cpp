#include <memory>

#include "agaLang.h"
#include "agaCompiler.h"
#include "agaCodeGenerator.h"
#include "agaParser.h"
#include "agaLogger.h"

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

	void agaCompiler::CompileSource (const std::string &code)
	{
		m_Parser = std::unique_ptr<agaParser> (new agaParser (code));

		agaASTExpression *expression = m_Parser->Parse ();

		m_CodeGenerator = std::unique_ptr<agaCodeGenerator> (new agaCodeGenerator ());
		
		std::string generatedCode = m_CodeGenerator.get()->GenerateCode(expression);

		agaLogger::log (generatedCode);
	}

	//--------------------------------------------------------------------------------
}
