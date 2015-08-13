#include "agaCodeGenerator.h"

namespace aga
{
	//--------------------------------------------------------------------------------
	
	agaCodeGenerator::agaCodeGenerator()
	{

	}
	
	//--------------------------------------------------------------------------------

	void agaCodeGenerator::AddCodeLine (const std::string &codeLine)
	{
		m_Code += codeLine + "\n";
	}
	
	//--------------------------------------------------------------------------------
	
	const std::string &agaCodeGenerator::GetCode() const
	{
		return m_Code;
	}
	
	//--------------------------------------------------------------------------------
}
