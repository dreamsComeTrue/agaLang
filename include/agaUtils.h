#ifndef _AGA_UTILS_H_
#define _AGA_UTILS_H_

#define SAFE_DELETE(x) { if (x != nullptr) { delete x; x = nullptr; }

//--------------------------------------------------------------------------------

namespace aga
{
	std::string ToString (int value)
	{
		std::stringstream sstream;

		sstream << value;

		return sstream.str();
	}
}

//--------------------------------------------------------------------------------

#endif	//	_AGA_UTILS_H_
