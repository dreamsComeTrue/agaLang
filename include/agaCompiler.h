#ifndef _AGA_COMPILER_H_
#define _AGA_COMPILER_H_

#include <string>
#include <memory>

namespace aga
{
	class agaParser;
	class agaCodeGenerator;
    class agaASTProgram;
	
	class agaCompiler
	{
	public:
		agaCompiler ();
		
		~agaCompiler ();
		
        agaASTProgram* CompileSource (const std::string& code);
        void GenerateCode (agaASTProgram* programNode);
		
	private:
		std::unique_ptr<agaParser>			m_Parser;
		std::unique_ptr<agaCodeGenerator>	m_CodeGenerator;
	};
}

#endif	//	_AGA_COMPILER_H_
