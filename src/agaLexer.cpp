#include <cctype>

#include "agaLexer.h"
#include "agaLogger.h"
#include "agaException.h"

namespace aga
{
	//--------------------------------------------------------------------------------

	agaLexer::agaLexer (const std::string &source) :
		m_CurrentPosition (-1),
		m_CurrentLine (1),
		m_CurrentColumn (0),
		m_CurrentCharacter (0),
		m_Source (source)
	{
	}

	//--------------------------------------------------------------------------------

	agaToken agaLexer::GetNextToken ()
	{
		agaToken token;
		int lastColumn;
		int lastLine;

		m_CurrentCharacter = GetNextCharacter();

		//	Skip any whitespaces
		while (isWhiteSpace (m_CurrentCharacter))
		{
			m_CurrentCharacter = GetNextCharacter();
		}

		//	Parse identifier
		if (isAlpha (m_CurrentCharacter))
		{
			long begin = m_CurrentPosition;
			long beginColumn = m_CurrentColumn;

			while (isAlphaNumeric (m_CurrentCharacter))
			{
				lastColumn = m_CurrentColumn;
				lastLine = m_CurrentLine;

				m_CurrentCharacter = GetNextCharacter();
			}

			long end = m_CurrentPosition;

			std::string sub = m_Source.substr (begin, end - begin);

			token = agaToken (sub, TokenIdentifier, m_CurrentLine, beginColumn, beginColumn + sub.length() - 1, m_CurrentPosition);

			m_CurrentPosition--;
			m_CurrentColumn = lastColumn;
			m_CurrentLine = lastLine;
		}
		else

			//	Parse number
			if (isDigit (m_CurrentCharacter))
			{
				long begin = m_CurrentPosition;
				long beginColumn = m_CurrentColumn;

				while (isDigit (m_CurrentCharacter))
				{
					lastColumn = m_CurrentColumn;
					lastLine = m_CurrentLine;

					m_CurrentCharacter = GetNextCharacter();
				}

				//	Check, if we have a float number?
				if (m_CurrentCharacter == '.')
				{
					//	Skip a 'dot' separator
					m_CurrentCharacter = GetNextCharacter();

					while (isDigit (m_CurrentCharacter))
					{
						m_CurrentCharacter = GetNextCharacter();
					}

					long end = m_CurrentPosition;
					std::string sub = m_Source.substr (begin, end - begin);

					token = agaToken (sub, TokenFloat, m_CurrentLine, beginColumn, beginColumn + sub.length() - 1, m_CurrentPosition);
				}
				else
				{
					long end = m_CurrentPosition;
					std::string sub = m_Source.substr (begin, end - begin);

					token = agaToken (sub, TokenInteger, m_CurrentLine, beginColumn, beginColumn + sub.length() - 1, m_CurrentPosition);
				}

				m_CurrentPosition--;
				m_CurrentColumn = lastColumn;
				m_CurrentLine = lastLine;
			}
			else
				switch (m_CurrentCharacter)
				{
					case '"':
					{
						long begin = m_CurrentPosition;
						long beginColumn = m_CurrentColumn;

						do
						{
							m_CurrentCharacter = GetNextCharacter();

							if (m_CurrentCharacter == 0)
							{
								throw agaException ("Lexing error: unexpected end of string");
							}
						}
						while (m_CurrentCharacter != '"');

						long end = m_CurrentPosition + 1;

						std::string sub = m_Source.substr (begin, end - begin);

						token = agaToken (sub, TokenString, m_CurrentLine, beginColumn, beginColumn + sub.length() - 1, m_CurrentPosition);
						break;
					}

					case ':':
					{
						token = agaToken (":", TokenColon, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
						break;
					}

					case '(':
					{
						token = agaToken ("(", TokenLeftParenthesis, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
						break;
					}

					case ')':
					{
						token = agaToken (")", TokenRightParenthesis, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
						break;
					}

					case '[':
					{
						token = agaToken ("[", TokenLeftBracket, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
						break;
					}

					case ']':
					{
						token = agaToken ("]", TokenRightBracket, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
						break;
					}

					case '{':
					{
						token = agaToken ("{", TokenLeftBrace, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
						break;
					}

					case '}':
					{
						token = agaToken ("}", TokenRightBrace, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
						break;
					}

					case '+':
					{
						token = agaToken ("+", TokenPlus, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
						break;
					}

					case '-':
					{
						token = agaToken ("-", TokenMinus, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
						break;
					}

					case '*':
					{
						token = agaToken ("*", TokenMultiply, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
						break;
					}

					case '/':
					{
						token = agaToken ("/", TokenDivide, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
						break;
					}

					case '=':
					{
						token = agaToken ("=", TokenEquals, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
						break;
					}

					case '#':
					{
						token = agaToken ("#", TokenPound, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
						break;
					}

					case '.':
					{
						long begin = m_CurrentPosition;
						long beginColumn = m_CurrentColumn;

						//	Skip a 'dot' separator
						m_CurrentCharacter = GetNextCharacter();

						while (isDigit (m_CurrentCharacter))
						{
							lastColumn = m_CurrentColumn;
							lastLine = m_CurrentLine;

							m_CurrentCharacter = GetNextCharacter();
						}

						long end = m_CurrentPosition;

						if (end - begin == 1)
						{
							throw agaException ("Lexing error: missing fraction digits");
						}

						std::string sub = m_Source.substr (begin, end - begin);

						token = agaToken (sub, TokenFloat, m_CurrentLine, beginColumn, beginColumn + sub.length() - 1, m_CurrentPosition);

						m_CurrentPosition--;
						m_CurrentColumn = lastColumn;
						m_CurrentLine = lastLine;

						break;
					}
				}

		//	Skip any remainding whitespaces
		if (isWhiteSpace (m_Source[m_CurrentPosition + 1]))
		{
			bool backTrack = false;

			do
			{
				lastColumn = m_CurrentColumn;
				lastLine = m_CurrentLine;
				backTrack = true;

				m_CurrentCharacter = GetNextCharacter();
			}
			while (isWhiteSpace (m_CurrentCharacter));

			if (backTrack)
			{
				m_CurrentPosition--;
				m_CurrentColumn = lastColumn;
				m_CurrentLine = lastLine;
			}
		}
		
		return token;
	}

	//--------------------------------------------------------------------------------

	bool agaLexer::HasMoreTokens()
	{
		return ! (m_CurrentPosition >= (long) m_Source.length() - 1);
	}

	//--------------------------------------------------------------------------------

	bool agaLexer::isWhiteSpace (char character)
	{
		return isspace (character);
	}

	//--------------------------------------------------------------------------------

	bool agaLexer::isAlpha (char character)
	{
		return isalpha (character);
	}

	//--------------------------------------------------------------------------------

	bool agaLexer::isAlphaNumeric (char character)
	{
		return isalnum (character);
	}

	//--------------------------------------------------------------------------------

	bool agaLexer::isDigit (char character)
	{
		return isdigit (character);
	}
	//--------------------------------------------------------------------------------

	char agaLexer::GetNextCharacter ()
	{
		++m_CurrentPosition;
		++m_CurrentColumn;

		if (m_Source[m_CurrentPosition] == '\n')
		{
			m_CurrentColumn = 0;
			++m_CurrentLine;
		}

		return m_Source[m_CurrentPosition];
	}

	//--------------------------------------------------------------------------------
}



