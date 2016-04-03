#ifndef _AGA_CODEGENERATOR_H_
#define _AGA_CODEGENERATOR_H_

#include <map>
#include <string>
#include <vector>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"

#include "agaInstructions.h"

namespace aga
{
    class agaASTExpression;
    class agaASTBinaryOperator;
    class agaASTBooleanRelation;
    class agaASTLogicalRelation;
    class agaASTNode;
    class agaASTProgram;
    class agaASTAssignment;
    class agaCompiler;

    class agaCodeGenerator
    {
      public:
        agaCodeGenerator ();
        ~agaCodeGenerator ();

        const std::vector<std::string> &GenerateCode (agaCompiler *compiler, std::shared_ptr<agaASTProgram> program);
        const std::unique_ptr<llvm::Module> &GetModule () const { return m_Module; }
        llvm::IRBuilder<> &GetBuilder () const { return (*m_IRBuilder.get ()); }
        std::map<std::string, llvm::AllocaInst *> &GetNamedValues () { return m_NamedValues; }

      protected:
        const std::vector<std::string> &GenerateCode (std::shared_ptr<agaASTNode> node);

        void AddCodeLine (const std::string &codeLine, int indentLevel = 0);

        void GenerateAssignment (std::shared_ptr<agaASTAssignment> node);
        void GenerateBinaryExpression (std::shared_ptr<agaASTBinaryOperator> node);
        void GenerateBooleanRelation (std::shared_ptr<agaASTBooleanRelation> node);
        void GenerateLogicalRelation (std::shared_ptr<agaASTLogicalRelation> node);

        void EmitInstruction (InstructionType instruction, const std::string &label = "");
        void EmitInstruction (InstructionType instruction, int dstRegisterIndex);
        void EmitInstruction (InstructionType instruction, int dstRegisterIndex, int srcRegisterIndex);
        void EmitInstruction (InstructionType instruction, int registerIndex, const std::string &label);
        void EmitInstruction (InstructionType instruction, const std::string &label, int registerIndex);
        const std::string GenerateLabel ();
        const std::string EmitLabel (const std::string &overrideLabel = "");
        const std::string GetTrueLiteral ();
        const std::string GetFalseLiteral ();

        InstructionType GetInstructionTypeFromCode (const std::string &code);

        std::vector<std::string> m_Code;
        int m_CurrentRegisterIndex;
        int m_CurrentLabelIndex;
        std::unique_ptr<llvm::Module> m_Module;
        std::unique_ptr<llvm::IRBuilder<>> m_IRBuilder;
        std::map<std::string, llvm::AllocaInst *> m_NamedValues;
    };
}

#endif //	_AGA_CODEGENERATOR_H_
