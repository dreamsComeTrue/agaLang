#ifndef _AGA_EXCPETION_H_
#define _AGA_EXCPETION_H_

#include <string>
#include <exception>

namespace aga
{
	class agaException: public std::exception
	{
	public:
		agaException (const std::string& message);
		
		~agaException () throw() {};
		const char* what () throw ();		
		
	private:
		std::string m_Message;
	};
}

#endif	//	_AGA_EXCPETION_H_