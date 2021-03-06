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
		JMP,
	    JE,
	    JNE,
	    JG,
	    JGE,
	    JL,
	    JLE,
        SETG,
        SETL,
        SETLE,
        SETGE,
        SETE,
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
		{"JMP"       , JMP},
		{"JE"        , JE},
		{"JNE"       , JNE},
		{"JG"        , JG},
		{"JGE"       , JGE},
		{"JL"        , JL},
		{"JLE"       , JLE},
        {"SETG"      , SETG},
        {"SETL"      , SETL},
        {"SETLE"     , SETLE},
        {"SETGE"     , SETGE},
        {"SETE"      , SETE},
	};

	int const instructionsCount = sizeof (instructions) / sizeof (Instruction);
}

#endif	//	_AGA_INSTRUCTIONS_H_
