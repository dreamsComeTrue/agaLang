#ifndef _AGA_CODEGENERATOR_H_
#define _AGA_CODEGENERATOR_H_

#include <string>

namespace aga
{
	class agaCodeGenerator
	{
	public:
		agaCodeGenerator ();
		
		void AddCodeLine (const std::string& codeLine);
		
		const std::string& GetCode () const;
		
	protected:
		std::string		m_Code;
	};
}

#endif	//	_AGA_CODEGENERATOR_H_