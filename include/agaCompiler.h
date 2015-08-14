#ifndef _AGA_COMPILER_H_
#define _AGA_COMPILER_H_

#include <string>
#include <memory>

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
		std::unique_ptr<agaParser>			m_Parser;
		std::unique_ptr<agaCodeGenerator>	m_CodeGenerator;
	};
}

#endif	//	_AGA_COMPILER_H_