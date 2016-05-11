#ifndef _AGA_ASTBLOCKPARAMETER_H_
#define _AGA_ASTBLOCKPARAMETER_H_

#include "agaASTNode.h"
#include "agaSymbol.h"
#include "agaTypeInfo.h"

namespace aga
{
    class agaASTBlockParameter
    {
      public:
        agaASTBlockParameter (const std::string &name, const agaTypeInfo &typeInfo) : m_Name (name), m_TypeInfo (typeInfo) {}

        const std::string &GetName () const { return m_Name; }

        agaTypeInfo &GetTypeInfo () { return m_TypeInfo; }

      private:
        std::string m_Name;
        agaTypeInfo m_TypeInfo;
    };
}

#endif //	_AGA_ASTBLOCKPARAMETER_H_
