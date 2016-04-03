#ifndef _AGA_TYPEINFO_H_
#define _AGA_TYPEINFO_H_

#include <string>

namespace aga
{
    struct agaTypeInfo
    {
      public:
        enum Type
        {
            INT,
            FLOAT,
            STRING
        };

      public:
        agaTypeInfo (long value) : m_LongValue (value), m_Type (INT) {}
        agaTypeInfo (double value) : m_DoubleValue (value), m_Type (FLOAT) {}
        agaTypeInfo (char *value) : m_StringValue (value), m_Type (STRING) {}

        Type GetType () { return m_Type; }

        long GetLong () { return m_LongValue; }
        double GetDouble () { return m_DoubleValue; }
        char *GetString () { return m_StringValue; }

      private:
        union {
            long m_LongValue;
            double m_DoubleValue;
            char *m_StringValue;
        };

        Type m_Type;
    };
}

#endif //	_AGA_TYPEINFO_H_
