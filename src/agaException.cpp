#include "agaException.h"

namespace aga
{
	//--------------------------------------------------------------------------------
	
	agaException::agaException (const std::string& message) :
		m_Message (message)
	{		
	}
	
	//--------------------------------------------------------------------------------
	
	const char* agaException::what () throw ()
	{
		return m_Message.c_str ();
	}
	
	//--------------------------------------------------------------------------------
}
