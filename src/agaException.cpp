#include <string>
#include <cstdarg>

#include "agaException.h"

namespace aga
{
	//--------------------------------------------------------------------------------

	agaException::agaException (const std::string &message) :
		m_Message (message)
	{
	}

	//--------------------------------------------------------------------------------

	agaException::agaException (const char *fmt, ...)
	{
		int size = 1024;
		char buffer[size];

		va_list args;
		va_start (args, fmt);
		vsnprintf (buffer,size,fmt, args);
		va_end (args);

		m_Message = std::string (buffer);
	}

	//--------------------------------------------------------------------------------

	const char *agaException::what () throw ()
	{
		return m_Message.c_str ();
	}

	//--------------------------------------------------------------------------------
}
