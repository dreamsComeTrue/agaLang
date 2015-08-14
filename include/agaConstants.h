#ifndef _AGA_CONSTANTS_H_
#define _AGA_CONSTANTS_H_

#define AGALANG_USAGE "Usage: agaLang source.aga"
#define FILE_OPEN_ERROR "Unable to open the input file!"

//	Lexer
#define LEXER_END_OF_TOKEN_STREAM "Lexer error: unexpected end of string"
#define LEXER_MISSING_FRACTION_DIGITS "Lexer error: missing fraction digits"

//	Parser
#define END_OF_TOKEN_STREAM "Unexpected end of token stream!"
#define UNEXPECTED_TOKEN "Unexpected token: %s"
#define UNEXPECTED_TOKEN_EOF "Unexpected token: %s<%s> at EOF"
#define UNEXPECTED_TOKEN_EXPECTING "Unexpected token: %s, expecting: %s"

#endif	//	_AGA_CONSTANTS_H_