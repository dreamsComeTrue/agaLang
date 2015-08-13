#ifndef _AGA_TOKEN_H_
#define _AGA_TOKEN_H_

#include <string>

namespace aga
{
	enum TokenType
	{
		TokenUnknown,
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
		TokenPlus,							//	+
		TokenMinus,							//	-
		TokenMultiply,						//	*
		TokenDivide,						//	/
		TokenEquals,						//	=
		TokenPound							//	#
	};
	
	extern std::string TokenNames[];
	
	class agaToken
	{
	public:
		agaToken ();
		agaToken (const std::string& literal, TokenType type, long line, long beginColumn, long endColumn, long posiiton);
		
		TokenType GetType () const;
		
		const std::string& GetLiteral () const;
		
		void Print ();
		
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