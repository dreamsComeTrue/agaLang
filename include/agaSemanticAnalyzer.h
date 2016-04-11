#ifndef _AGA_SEMANTICANALYZER_H_
#define _AGA_SEMANTICANALYZER_H_

#include <memory>

namespace aga
{
    enum agaSemanticPhase
    {
        VariablePhase,
        ExpressionPhase
    };

    class agaASTBlock;

    class agaSemanticAnalyzer
    {
      public:
        agaSemanticAnalyzer () {}

        void SetEnclosingBlock (std::shared_ptr<agaASTBlock> enclosingBlock) { m_EnclosingBlock = enclosingBlock; }
        const std::shared_ptr<agaASTBlock> &GetEnclosingBlock () const { return m_EnclosingBlock; }

        agaSemanticPhase GetPhase () { return m_Phase; }
        void SetPhase (agaSemanticPhase phase) { m_Phase = phase; }

      private:
        agaSemanticPhase m_Phase;
        std::shared_ptr<agaASTBlock> m_EnclosingBlock;
    };
}

#endif //	_AGA_SEMANTICANALYZER_H_
