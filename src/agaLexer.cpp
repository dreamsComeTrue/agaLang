#include <cctype>

#include "agaConstants.h"
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

    GetNextCharacter();

    long begin = m_CurrentPosition;
    long beginColumn = m_CurrentColumn;

    if (m_CurrentCharacter == '\n')
    {
        return agaToken ("\n", TokenEndOfLine, m_CurrentLine, beginColumn, beginColumn + 1, m_CurrentPosition);
    }

    if (m_CurrentCharacter == 0)
    {
        return agaToken ("", TokenEndOfFile, m_CurrentLine, beginColumn, beginColumn, m_CurrentPosition);
    }

    //	Skip any whitespaces
    while (isWhiteSpace (m_CurrentCharacter))
    {
        GetNextCharacter();
    }

    //	Parse identifier
    if (isAlpha (m_CurrentCharacter))
    {
        begin = m_CurrentPosition;
        beginColumn = m_CurrentColumn;

        while (isAlphaNumeric (m_CurrentCharacter))
        {
            lastColumn = m_CurrentColumn;
            lastLine = m_CurrentLine;

            GetNextCharacter();
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

                GetNextCharacter();
            }

            //	Check, if we have a float number?
            if (m_CurrentCharacter == '.')
            {
                //	Skip a 'dot' separator
                GetNextCharacter();

                while (isDigit (m_CurrentCharacter))
                {
                    GetNextCharacter();
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
                    GetNextCharacter();

                    if (m_CurrentCharacter == 0)
                    {
                        throw agaException (LEXER_END_OF_TOKEN_STREAM);
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
                if (CheckNextCharacter() == '=')
                {
                    GetNextCharacter();

                    token = agaToken ("==", TokenSameAs, m_CurrentLine, m_CurrentColumn, m_CurrentColumn + 1, m_CurrentPosition);
                }
                else
                {
                    token = agaToken ("=", TokenEquals, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
                }

                break;
            }

            case '!':
            {
                token = agaToken ("!", TokenNot, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);

                break;
            }

            case '&':
            {
                if (CheckNextCharacter() == '&')
                {
                    GetNextCharacter();

                    token = agaToken ("&&", TokenAnd, m_CurrentLine, m_CurrentColumn, m_CurrentColumn + 1, m_CurrentPosition);
                }

                break;
            }

            case '|':
            {
                if (CheckNextCharacter() == '|')
                {
                    GetNextCharacter();

                    token = agaToken ("||", TokenOr, m_CurrentLine, m_CurrentColumn, m_CurrentColumn + 1, m_CurrentPosition);
                }

                break;
            }

            case '<':
            {
                if (CheckNextCharacter() == '=')
                {
                    GetNextCharacter();

                    token = agaToken ("<=", TokenLessEqualThan, m_CurrentLine, m_CurrentColumn, m_CurrentColumn + 1, m_CurrentPosition);
                }
                else
                {
                    token = agaToken ("<", TokenLessThan, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
                }

                break;
            }

            case '>':
            {
                if (CheckNextCharacter() == '=')
                {
                    GetNextCharacter();

                    token = agaToken (">=", TokenGreaterEqualThan, m_CurrentLine, m_CurrentColumn, m_CurrentColumn + 1, m_CurrentPosition);
                }
                else
                {
                    token = agaToken ("<", TokenGreaterThan, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
                }

                break;
            }

            case '#':
            {
                token = agaToken ("#", TokenHash, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
                break;
            }

            case ';':
            {
                token = agaToken (";", TokenSemicolon, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
                break;
            }

            case ',':
            {
                token = agaToken (",", TokenComma, m_CurrentLine, m_CurrentColumn, m_CurrentColumn, m_CurrentPosition);
                break;
            }

            case '.':
            {
                long begin = m_CurrentPosition;
                long beginColumn = m_CurrentColumn;

                //	Skip a 'dot' separator
                GetNextCharacter();

                while (isDigit (m_CurrentCharacter))
                {
                    lastColumn = m_CurrentColumn;
                    lastLine = m_CurrentLine;

                    GetNextCharacter();
                }

                long end = m_CurrentPosition;

                if (end - begin == 1)
                {
                    throw agaException (LEXER_MISSING_FRACTION_DIGITS);
                }

                std::string sub = m_Source.substr (begin, end - begin);

                token = agaToken (sub, TokenFloat, m_CurrentLine, beginColumn, beginColumn + sub.length() - 1, m_CurrentPosition);

                m_CurrentPosition--;
                m_CurrentColumn = lastColumn;
                m_CurrentLine = lastLine;

                break;
            }
            }

    return token;

    //	Skip any remaining whitespaces
    if (isWhiteSpace (m_Source[m_CurrentPosition + 1]))
    {
        bool backTrack = false;

        do
        {
            lastColumn = m_CurrentColumn;
            lastLine = m_CurrentLine;
            backTrack = true;

            if (m_CurrentCharacter == '\n')
            {
                return agaToken ("\n", TokenEndOfLine, m_CurrentLine, beginColumn, beginColumn + 1, m_CurrentPosition);
            }

            if (m_CurrentCharacter == 0)
            {
                return agaToken ("", TokenEndOfFile, m_CurrentLine, beginColumn, beginColumn, m_CurrentPosition);
            }

            GetNextCharacter();
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

agaToken agaLexer::CheckNextToken()
{
    long line =	m_CurrentLine;
    long column = m_CurrentColumn;
    long position =	m_CurrentPosition;
    char character = m_CurrentCharacter;

    agaToken token = GetNextToken();

    m_CurrentLine = line;
    m_CurrentColumn = column;
    m_CurrentPosition = position;
    m_CurrentCharacter = character;

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

    if (m_CurrentPosition > m_Source.length() - 1)
    {
        //m_CurrentPosition = m_Source.length() - 1;
        m_CurrentCharacter = 0;
    }
    else
    {
        m_CurrentCharacter = m_Source[m_CurrentPosition];
    }

    return m_CurrentCharacter;
}

//--------------------------------------------------------------------------------

char agaLexer::CheckNextCharacter()
{
    long pos = m_CurrentPosition;
    long col = m_CurrentColumn;
    char currentChar = m_CurrentCharacter;
    char charToReturn = GetNextCharacter();

    m_CurrentPosition = pos;
    m_CurrentColumn = col;
    m_CurrentCharacter = currentChar;

    return charToReturn;
}

//--------------------------------------------------------------------------------
}





