#include "agaCompiler.h"
#include "agaCodeGenerator.h"
#include "agaParser.h"
#include "agaLogger.h"

namespace aga
{
	//--------------------------------------------------------------------------------
		
	agaCompiler::agaCompiler()
	{
	}
	
	//--------------------------------------------------------------------------------
	
	agaCompiler::~agaCompiler()
	{
		if (m_CodeGenerator != NULL)
		{
			delete m_CodeGenerator;
			m_CodeGenerator = NULL;
		}
		
		if (m_Parser != NULL)
		{
			delete m_Parser;
			m_Parser = NULL;
		}
	}
	
	//--------------------------------------------------------------------------------
	
	void agaCompiler::CompileSource (const std::string &code)
	{
		m_Parser = new agaParser (code);
		
		agaASTExpression* expression = m_Parser->Parse ();
		
		m_CodeGenerator = new agaCodeGenerator ();
		
		expression->Evaluate (m_CodeGenerator);
		
		agaLogger::log (m_CodeGenerator->GetCode ());
	}
	
	//--------------------------------------------------------------------------------
}
