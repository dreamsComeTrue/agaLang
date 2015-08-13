#ifndef _AGA_LOGGER_H_
#define _AGA_LOGGER_H_

#include <string>
#include <cstdarg>

namespace aga
{
	class agaLogger
	{
	private:
		agaLogger () { }
		~agaLogger () { }
		
		agaLogger (agaLogger const&);       // Don't Implement
        void operator= (agaLogger const&); 	// Don't implement
		
	public:
		static agaLogger& getInstance ();
		
		static void log (char logInfo, bool endLine = true);
		static void log (const char* logInfo, bool endLine = true);
		static void log (const std::string& logInfo, bool endLine = true);
		static void log (long logInfo, bool endLine = true);
		static void log (double logInfo, bool endLine = true);
		static void log (bool endLine, const char* fmt...);
	};
}

#endif	//	_AGA_LOGGER_H_