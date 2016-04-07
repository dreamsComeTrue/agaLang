#ifndef _AGA_UTILS_H_
#define _AGA_UTILS_H_

#include <sstream>
#include <string>

//--------------------------------------------------------------------------------

namespace aga
{
    std::string ToString (int value)
    {
        std::stringstream sstream;

        sstream << value;

        return sstream.str ();
    }
}

//--------------------------------------------------------------------------------

#endif //	_AGA_UTILS_H_
