#ifndef _AGA_INSTRUCTIONS_H_
#define _AGA_INSTRUCTIONS_H_

#include <string>

namespace aga
{
	enum InstructionType
	{
	    MOV,
	    ADD,
	    SUB,
	    MUL,
	    DIV,
		CMP,
	    JE,
	    Unknown,
	};

	struct Instruction
	{
		const char 			*word;
		InstructionType   	type;
	};

	Instruction const instructions[] =
	{
		{"MOV"       , MOV},
		{"ADD"       , ADD},
		{"SUB"       , SUB},
		{"MUL"       , MUL},
		{"DIV"       , DIV},
		{"CMP"       , CMP},
		{"JE"       , JE},
	};

	int const instructionsCount = sizeof (instructions) / sizeof (Instruction);
}

#endif	//	_AGA_INSTRUCTIONS_H_
