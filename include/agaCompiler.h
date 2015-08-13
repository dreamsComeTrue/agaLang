#ifndef _AGA_COMPILER_H_
#define _AGA_COMPILER_H_

#include <string>

namespace aga
{
	class agaParser;
	class agaCodeGenerator;
	
	class agaCompiler
	{
	public:
		agaCompiler ();
		
		~agaCompiler ();
		
		void CompileSource (const std::string& code);
		
	private:
		agaParser*			m_Parser;
		agaCodeGenerator*	m_CodeGenerator;
	};
}

#endif	//	_AGA_COMPILER_H_