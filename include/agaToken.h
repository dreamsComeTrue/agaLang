#ifndef _AGA_TOKEN_H_
#define _AGA_TOKEN_H_

#include <string>

namespace aga
{
	enum TokenType
	{
	    TokenUnknown,
	    TokenEndOfFile,						//	EOF
	    TokenEndOfLine,						//	EOL
	    TokenInteger,						//	323
	    TokenFloat,							//	12.453
	    TokenString,						//	"abc"
	    TokenLeftParenthesis,				//	(
	    TokenRightParenthesis,				//	)
	    TokenLeftBracket,					//	[
	    TokenRightBracket,					//	]
	    TokenLeftBrace,						//	{
	    TokenRightBrace,					//	}
	    TokenIdentifier,					//	hello
	    TokenColon,							//	:
	    TokenSemicolon,						//	;
	    TokenComma,							//	,
        TokenDot,							//	.
        TokenPlus,							//	+
	    TokenMinus,							//	-
	    TokenMultiply,						//	*
	    TokenDivide,						//	/
	    TokenEquals,						//	=
        TokenHash,							//	#
	    TokenSameAs,						//	==
	    TokenLessThan,						//	<
	    TokenLessEqualThan,					//	<=
	    TokenGreaterThan,					//	>
	    TokenGreaterEqualThan,				//	>=
	    TokenOr,							//	||
	    TokenAnd,							//	&&
	    TokenNot,							//	!
        TokenQuestion,  					//	?
	};

	struct TokenWord
	{
		const char *word;
		TokenType   tokenType;
	};

	TokenWord const tokenWords[] =
	{
		{"Unknown"       , TokenUnknown},
		{"EOF"       , TokenEndOfFile},
		{"EOL"       , TokenEndOfLine},
		{"TokenInteger"       , TokenInteger},
		{"TokenFloat"       , TokenFloat},
		{"String"       , TokenString},
		{"LeftParenthesis"       , TokenLeftParenthesis},
		{"RightParenthesis"       , TokenRightParenthesis},
		{"LeftBracket"       , TokenLeftBracket},
		{"RightBracket"       , TokenRightBracket},
		{"LeftBrace"       , TokenLeftBrace},
		{"RightBrace"       , TokenRightBrace},
		{"Identifier"       , TokenIdentifier},
		{"Colon"       , TokenColon},
		{"Semicolon"       , TokenSemicolon},
		{"Comma"       , TokenComma},
        {"Dot"       , TokenDot},
		{"Plus"       , TokenPlus},
		{"Minus"       , TokenMinus},
		{"Multiply"       , TokenMultiply},
		{"Divide"       , TokenDivide},
		{"Equals"       , TokenEquals},
        {"Hash"       , TokenHash},
		{"SameAs"       , TokenSameAs},
		{"LessThan"       , TokenLessThan},
		{"LessEqualThan"       , TokenLessEqualThan},
		{"GreaterThan"       , TokenGreaterThan},
		{"GreaterEqualThan"       , TokenGreaterEqualThan},
		{"Or"       , TokenOr},
		{"And"       , TokenAnd},
		{"Not"       , TokenNot},
        {"Question"       , TokenQuestion},
	};

	int const numTokenWords = sizeof (tokenWords) /sizeof (TokenWord);

	class agaToken
	{
	public:
		agaToken ();
		agaToken (const std::string &literal, TokenType type, long line, long beginColumn, long endColumn, long posiiton);

		TokenType GetType () const;

		const std::string &GetLiteral () const;

        void Print () const;

        const std::string GetInfo () const;

	private:
		std::string	m_Literal;
		TokenType	m_Type;
		long		m_Line;
		long		m_BeginColumn;
		long		m_EndColumn;
		long		m_Position;
	};
}

#endif	//	_AGA_TOKEN_H_
