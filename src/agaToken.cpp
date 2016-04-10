#include <sstream>

#include "agaLogger.h"
#include "agaToken.h"

namespace aga
{
    //--------------------------------------------------------------------------------

    std::string TokenNames[] = {"Unknown",
                                "EOF",
                                "EOL",
                                "TokenInteger",
                                "TokenFloat",
                                "String",
                                "LeftParenthesis",
                                "RightParenthesis",
                                "LeftBracket",
                                "RightBracket",
                                "LeftBrace",
                                "RightBrace",
                                "Identifier",
                                "Colon",
                                "Semicolon",
                                "Comma",
                                "Plus",
                                "Minus",
                                "Multiply",
                                "Divide",
                                "Equals",
                                "Hash",
                                "SameAs",
                                "LessThan",
                                "LessEqualThan",
                                "GreaterThan",
                                "GreaterEqualThan",
                                "Or",
                                "And",
                                "Not"};

    //--------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------

    agaToken::agaToken ()
        : m_Literal (""), m_Type (TokenUnknown), m_Line (-1), m_BeginColumn (-1), m_EndColumn (-1), m_Position (-1)
    {
    }

    //--------------------------------------------------------------------------------

    agaToken::agaToken (const std::string &literal, TokenType type, long line, long beginColumn, long endColumn, long posiiton)
        : m_Literal (literal),
          m_Type (type),
          m_Line (line),
          m_BeginColumn (beginColumn),
          m_EndColumn (endColumn),
          m_Position (posiiton)
    {
    }

    //--------------------------------------------------------------------------------

    TokenType agaToken::GetType () const { return m_Type; }

    //--------------------------------------------------------------------------------

    const std::string &agaToken::GetLiteral () const { return m_Literal; }

    //--------------------------------------------------------------------------------

    void agaToken::Print () const
    {
        agaLogger::log (true, "[s: s <d, d:d>]", TokenNames[m_Type].c_str (), m_Literal.c_str (), m_Line, m_BeginColumn,
                        m_EndColumn);
    }

    //--------------------------------------------------------------------------------

    const std::string agaToken::GetInfo () const
    {
        std::stringstream sstream;

        sstream << "[" << tokenWords[m_Type].word << " <" << (m_Line) << ", " << m_BeginColumn << ":" << m_EndColumn << ">]";

        return sstream.str ();
    }

    //--------------------------------------------------------------------------------
}
