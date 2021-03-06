#ifndef _AGA_SYMBOL_H_
#define _AGA_SYMBOL_H_

#include "agaASTNode.h"

namespace aga
{
    class agaSymbol
    {
      public:
        agaSymbol (const std::string &name, agaASTNode *node, llvm::AllocaInst *value, agaASTNode *parent)
            : m_Name (name), m_Node (node), m_Value (value), m_Parent (parent)
        {
        }

        const std::string &GetName () const { return m_Name; }

        void SetNode (agaASTNode *node) { m_Node = node; }
        agaASTNode *GetNode () { return m_Node; }

        void SetParent (agaASTNode *parent) { m_Parent = parent; }
        const agaASTNode *GetParent () const { return m_Parent; }

        void SetValue (llvm::AllocaInst *value) { m_Value = value; }
        llvm::AllocaInst *GetValue () const { return m_Value; }

      private:
        std::string m_Name;
        agaASTNode *m_Node;
        agaASTNode *m_Parent;
        llvm::AllocaInst *m_Value;
    };
}

#endif //	_AGA_SYMBOL_H_
