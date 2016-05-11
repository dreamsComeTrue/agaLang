#ifndef _AGA_TYPEINFO_H_
#define _AGA_TYPEINFO_H_

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Type.h"
#include <map>
#include <string>

namespace aga
{
    struct agaTypeInfo
    {
      public:
        enum Type
        {
            INT,
            Num8,
            Num16,
            Num32,
            Num64,
            Num128,
            Float,
            Void,
            STRING,
            None
        };

        std::map<std::string, Type> typeNames = {{"num8", Num8},     {"num16", Num16}, {"num32", Num32}, {"num64", Num64},
                                                 {"num128", Num128}, {"float", Float}, {"void", Void}};

      public:
        agaTypeInfo () {}
        agaTypeInfo (const std::string &name) : m_Name (name) { m_Type = typeNames[name]; }

        agaTypeInfo (long value) : m_LongValue (value), m_Type (INT) {}
        agaTypeInfo (double value) : m_DoubleValue (value), m_Type (Float) {}
        agaTypeInfo (char *value) : m_StringValue (value), m_Type (STRING) {}
        agaTypeInfo (const std::string &name, long value) : m_LongValue (value), m_Name (name), m_Type (INT) {}
        agaTypeInfo (const std::string &name, double value) : m_DoubleValue (value), m_Name (name), m_Type (Float) {}
        agaTypeInfo (const std::string &name, char *value) : m_StringValue (value), m_Name (name), m_Type (STRING) {}

        Type GetType () { return m_Type; }

        llvm::Type *GetLLVMType ()
        {
            llvm::LLVMContext &context = llvm::getGlobalContext ();

            switch (m_Type)
            {
            case Num8:
                return llvm::Type::getInt8Ty (context);
            case Num16:
                return llvm::Type::getInt16Ty (context);
            case Num32:
                return llvm::Type::getInt32Ty (context);
            case Num64:
                return llvm::Type::getInt64Ty (context);
            case Num128:
                return llvm::Type::getInt128Ty (context);
            case Void:
                return llvm::Type::getVoidTy (context);
            }

            return llvm::Type::getVoidTy (context);
        }

        long GetLong () { return m_LongValue; }
        double GetDouble () { return m_DoubleValue; }
        char *GetString () { return m_StringValue; }

        const std::string &GetName () const { return m_Name; }

      private:
        union {
            long m_LongValue;
            double m_DoubleValue;
            char *m_StringValue;
        };

        std::string m_Name;
        Type m_Type;
    };
}

#endif //	_AGA_TYPEINFO_H_
