#include <fstream>
#include <string>
#include <cctype>

#include "agaConstants.h"
#include "agaCompiler.h"
#include "agaLogger.h"
#include "agaException.h"

using namespace aga;

/*
 * Program Entry point
 */
int main (int argc, char **argv) 
{	
	if (argc == 1)
	{
		agaLogger::log (AGALANG_USAGE);
		
		return - 1;
	}
	
	std::ifstream stream;

	stream.open (argv[1]);
	stream.unsetf (std::ios_base::skipws);

	if (!stream)
	{
		agaLogger::log (AGALANG_FILE_OPEN_ERROR); 
		
		return -1;
	}

	std::string source = "";
	
	stream.seekg (0, std::ios::end);   
	source.reserve (stream.tellg ());
	stream.seekg (0, std::ios::beg);
	source.assign ((std::istreambuf_iterator<char> (stream)), std::istreambuf_iterator<char> ());
	stream.close ();
	
	agaCompiler compiler;
  
	try
	{
		compiler.CompileSource (source);
	}
	catch (agaException& e)
	{
		agaLogger::log (e.what ());
	}

	return 0;
}
