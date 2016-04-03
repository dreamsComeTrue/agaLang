#include <cctype>
#include <fstream>
#include <string>

#include "agaCompiler.h"
#include "agaConstants.h"
#include "agaLogger.h"

using namespace aga;

//--------------------------------------------------------------------------------

std::string GetSourceFromFile (const std::string &filePath)
{
    std::ifstream stream;
    std::string source = "";

    stream.open (filePath);
    stream.unsetf (std::ios_base::skipws);

    if (!stream)
    {
        agaLogger::log (FILE_OPEN_ERROR);

        return source;
    }

    stream.seekg (0, std::ios::end);
    source.reserve (stream.tellg ());
    stream.seekg (0, std::ios::beg);

    source.assign ((std::istreambuf_iterator<char> (stream)), std::istreambuf_iterator<char> ());
    stream.close ();

    return source;
}

//--------------------------------------------------------------------------------

std::string GetFileName (const std::string &strPath)
{
    size_t iLastSeparator = 0;
    return strPath.substr ((iLastSeparator = strPath.find_last_of ("\\")) != std::string::npos ? iLastSeparator + 1 : 0,
                           strPath.size () - strPath.find_last_of ("."));
}

//--------------------------------------------------------------------------------

/*
 * Program Entry point
 */
int main (int argc, char **argv)
{
    if (argc == 1)
    {
        agaLogger::log (AGALANG_USAGE);

        return -1;
    }

    std::string filePath (argv[1]);
    std::string source = GetSourceFromFile (filePath);

    if (source != "")
    {
        agaCompiler compiler;

        std::shared_ptr<agaASTProgram> program = compiler.CompileSource (GetFileName (filePath), source);
        compiler.GenerateCode (program);
    }

    return 0;
}
