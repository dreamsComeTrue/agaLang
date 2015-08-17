#include <iostream>
#include <sstream>
#include <iomanip>

#include "agaLogger.h"

namespace aga
{
	//--------------------------------------------------------------------------------

	agaLogger &agaLogger::getInstance ()
	{
		static agaLogger instance;

		return instance;
	}

	//--------------------------------------------------------------------------------

	void agaLogger::log (char logInfo, bool endLine)
	{
		std::cout << logInfo;

		if (endLine)
		{
			std::cout << std::endl;
		}
	}

	//--------------------------------------------------------------------------------

	void agaLogger::log (const char *logInfo, bool endLine)
	{
		std::cout << logInfo;

		if (endLine)
		{
			std::cout << std::endl;
		}
	}

	//--------------------------------------------------------------------------------

	void agaLogger::log (const std::string &logInfo, bool endLine)
	{
		std::cout << logInfo;

		if (endLine)
		{
			std::cout << std::endl;
		}
	}

	//--------------------------------------------------------------------------------

	void agaLogger::log (long logInfo, bool endLine)
	{
		std::cout << logInfo;

		if (endLine)
		{
			std::cout << std::endl;
		}
	}

	//--------------------------------------------------------------------------------

	void agaLogger::log (double logInfo, bool endLine)
	{
		std::cout << std::setiosflags (std::ios::fixed) << std::setprecision (4) << logInfo;

		if (endLine)
		{
			std::cout << std::endl;
		}
	}

	//--------------------------------------------------------------------------------

	void agaLogger::PrintMemory (int memoryAddress, const std::string &logInfo, bool endLine)
	{
		std::cout << std::hex << std::setfill ('0') << std::setw (4) << memoryAddress << "  " << logInfo;

		if (endLine)
		{
			std::cout << std::endl;
		}
	}

	//--------------------------------------------------------------------------------

	void agaLogger::log (bool endLine, const char *fmt...)
	{
		va_list args;
		va_start (args, fmt);

		while (*fmt != '\0')
		{
			if (*fmt == 'd')
			{
				int i = va_arg (args, int);
				std::cout << i;
			}
			else
				if (*fmt == 'c')
				{
					// note automatic conversion to integral type
					int c = va_arg (args, int);
					std::cout << static_cast<char> (c);
				}
				else
					if (*fmt == 'f')
					{
						double d = va_arg (args, double);
						std::cout << d;
					}
					else
						if (*fmt == 's')
						{
							char *s = va_arg (args, char *);
							std::cout << s;
						}
						else
						{
							std::cout << *fmt;
						}

			++fmt;
		}

		va_end (args);

		if (endLine)
		{
			std::cout << std::endl;
		}
	}

	//--------------------------------------------------------------------------------
}
