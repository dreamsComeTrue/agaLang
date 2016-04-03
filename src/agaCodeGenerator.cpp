#include <sstream>

#include "llvm-c/TargetMachine.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LLVMContext.h"

#include "agaASTAssignment.h"
#include "agaASTBinaryOperator.h"
#include "agaASTBlock.h"
#include "agaASTBooleanRelation.h"
#include "agaASTExpression.h"
#include "agaASTFunctionCall.h"
#include "agaASTLogicalRelation.h"
#include "agaASTMatch.h"
#include "agaASTProgram.h"
#include "agaCodeGenerator.h"
#include "agaCompiler.h"
#include "agaUtils.h"

namespace aga
{
    //--------------------------------------------------------------------------------

    agaCodeGenerator::agaCodeGenerator () : m_Module (nullptr), m_CurrentRegisterIndex (1), m_CurrentLabelIndex (0)
    {
        m_IRBuilder = llvm::make_unique<llvm::IRBuilder<>> (llvm::getGlobalContext ());
    }

    //--------------------------------------------------------------------------------

    agaCodeGenerator::~agaCodeGenerator () {}

    //--------------------------------------------------------------------------------

    void agaCodeGenerator::AddCodeLine (const std::string &codeLine, int indentLevel)
    {
        std::string indentString = "";

        for (int i = 0; i < indentLevel; ++i)
        {
            indentString += "    ";
        }

        m_Code.push_back (indentString + codeLine);
    }

    //--------------------------------------------------------------------------------

    const std::vector<std::string> &agaCodeGenerator::GenerateCode (agaCompiler *compiler, std::shared_ptr<agaASTProgram> program)
    {
        m_Module = llvm::make_unique<llvm::Module> (compiler->GetFileName (), llvm::getGlobalContext ());

        std::vector<std::shared_ptr<agaASTBlock>> blocks = program->GetBlocks ();

        for (std::shared_ptr<agaASTBlock> &block : blocks)
        {
            block->Evaluate (this);
        }

        m_Module.get ()->dump ();

        return m_Code;
    }

    //--------------------------------------------------------------------------------

    const std::vector<std::string> &agaCodeGenerator::GenerateCode (std::shared_ptr<agaASTNode> node)
    {
        switch (node->GetType ())
        {
        case ASTNodeType::BlockNode:
        {
            std::shared_ptr<agaASTBlock> block = std::static_pointer_cast<agaASTBlock> (node);

            AddCodeLine (block->ToString ());

            std::vector<std::shared_ptr<agaASTNode>> statements = block->GetStatements ();

            for (std::shared_ptr<agaASTNode> &statement : statements)
            {
                statement->Evaluate (this);

                GenerateCode (statement);
            }

            AddCodeLine ("< " + block->GetName () + " = " + block->GetReturnExpr ()->ToString ());
        }
        break;

        case ASTNodeType::FunctionCallNode:
        {
            std::shared_ptr<agaASTFunctionCall> functionCall = std::static_pointer_cast<agaASTFunctionCall> (node);

            AddCodeLine (functionCall->ToString ());
        }
        break;

        case ASTNodeType::MatchNode:
        {
            std::shared_ptr<agaASTMatch> match = std::static_pointer_cast<agaASTMatch> (node);

            AddCodeLine (match->ToString ());

            for (const std::shared_ptr<agaASTNode> &statement : match->GetCases ())
            {
                statement->Evaluate (this);

                GenerateCode (statement);
            }
        }
        break;

        case ASTNodeType::AssignmentNode:
            GenerateAssignment (std::static_pointer_cast<agaASTAssignment> (node));
            break;

        case ASTNodeType::BinaryOperationNode:
            GenerateBinaryExpression (std::static_pointer_cast<agaASTBinaryOperator> (node));
            break;

        case ASTNodeType::BooleanRelationNode:
            GenerateBooleanRelation (std::static_pointer_cast<agaASTBooleanRelation> (node));
            break;

        case ASTNodeType::LogicalRelationNode:
            GenerateLogicalRelation (std::static_pointer_cast<agaASTLogicalRelation> (node));
            break;

        default:
            if (node->GetType () == ConstantNode || node->GetType () == VariableNode)
            {
                EmitInstruction (InstructionType::MOV, m_CurrentRegisterIndex, node->GetAllocationBlock ().GetCode ());
            }

            ++m_CurrentRegisterIndex;

            break;
        }

        return m_Code;
    }

    //--------------------------------------------------------------------------------

    void agaCodeGenerator::GenerateAssignment (std::shared_ptr<agaASTAssignment> node)
    {
        std::shared_ptr<agaASTExpression> expression = node->GetExpression ();

        expression->Evaluate (this);

        GenerateCode (expression);

        node->Evaluate (this);

        EmitInstruction (InstructionType::MOV, node->GetAllocationBlock ().GetCode (), m_CurrentRegisterIndex - 1);
    }

    //--------------------------------------------------------------------------------

    void agaCodeGenerator::GenerateBinaryExpression (std::shared_ptr<agaASTBinaryOperator> node)
    {
        node->Evaluate (this);

        std::shared_ptr<agaASTNode> left = node->GetLeft ();
        std::shared_ptr<agaASTNode> right = node->GetRight ();

        ASTNodeType leftType = left->GetType ();
        ASTNodeType rightType = right->GetType ();

        if (leftType == ASTNodeType::BinaryOperationNode)
        {
            GenerateBinaryExpression (std::static_pointer_cast<agaASTBinaryOperator> (left));
            left->GetAllocationBlock ().SetRegisterIndex (m_CurrentRegisterIndex - 1);
        }

        if (leftType == ASTNodeType::BooleanRelationNode)
        {
            GenerateBooleanRelation (std::static_pointer_cast<agaASTBooleanRelation> (left));
            left->GetAllocationBlock ().SetRegisterIndex (m_CurrentRegisterIndex - 1);
        }

        if (rightType == ASTNodeType::BinaryOperationNode)
        {
            GenerateBinaryExpression (std::static_pointer_cast<agaASTBinaryOperator> (right));
            right->GetAllocationBlock ().SetRegisterIndex (m_CurrentRegisterIndex - 1);
        }

        if (rightType == ASTNodeType::BooleanRelationNode)
        {
            GenerateBooleanRelation (std::static_pointer_cast<agaASTBooleanRelation> (right));
            right->GetAllocationBlock ().SetRegisterIndex (m_CurrentRegisterIndex - 1);
        }

        if (leftType != ASTNodeType::BinaryOperationNode && leftType != ASTNodeType::BooleanRelationNode)
        {
            EmitInstruction (InstructionType::MOV, m_CurrentRegisterIndex, left->GetAllocationBlock ().GetCode ());
        }
        else
        {
            EmitInstruction (InstructionType::MOV, m_CurrentRegisterIndex, left->GetAllocationBlock ().GetRegisterIndex ());
        }

        InstructionType instructionType = GetInstructionTypeFromCode (node->GetAllocationBlock ().GetCode ());

        if (rightType != ASTNodeType::BinaryOperationNode && rightType != ASTNodeType::BooleanRelationNode)
        {
            EmitInstruction (instructionType, m_CurrentRegisterIndex, right->GetAllocationBlock ().GetCode ());
        }
        else
        {
            EmitInstruction (instructionType, m_CurrentRegisterIndex, right->GetAllocationBlock ().GetRegisterIndex ());
        }

        ++m_CurrentRegisterIndex;
    }

    //--------------------------------------------------------------------------------

    void agaCodeGenerator::GenerateBooleanRelation (std::shared_ptr<agaASTBooleanRelation> node)
    {
        GenerateCode (node->GetLeft ());

        int firstIndex = m_CurrentRegisterIndex - 1;

        GenerateCode (node->GetRight ());

        int secondIndex = m_CurrentRegisterIndex - 1;

        EmitInstruction (InstructionType::CMP, firstIndex, secondIndex);

        switch (node->GetToken ().GetType ())
        {
        case TokenLessThan:
            EmitInstruction (InstructionType::SETL, m_CurrentRegisterIndex);
            break;

        case TokenLessEqualThan:
            EmitInstruction (InstructionType::SETLE, m_CurrentRegisterIndex);
            break;

        case TokenGreaterThan:
            EmitInstruction (InstructionType::SETG, m_CurrentRegisterIndex);
            break;

        case TokenGreaterEqualThan:
            EmitInstruction (InstructionType::SETGE, m_CurrentRegisterIndex);
            break;

        case TokenSameAs:
            EmitInstruction (InstructionType::SETE, m_CurrentRegisterIndex);
            break;
        }

        ++m_CurrentRegisterIndex;
    }

    //--------------------------------------------------------------------------------

    void agaCodeGenerator::GenerateLogicalRelation (std::shared_ptr<agaASTLogicalRelation> node)
    {
        GenerateCode (node->GetLeft ());

        int firstIndex = m_CurrentRegisterIndex - 1;

        GenerateCode (node->GetRight ());

        int secondIndex = m_CurrentRegisterIndex - 1;

        switch (node->GetToken ().GetType ())
        {
        case TokenAnd:
        {
            std::string labelJE = GenerateLabel ();
            std::string labelJMP = GenerateLabel ();

            EmitInstruction (InstructionType::CMP, firstIndex, GetFalseLiteral ());
            EmitInstruction (InstructionType::JE, labelJE);
            EmitInstruction (InstructionType::CMP, secondIndex, GetFalseLiteral ());
            EmitInstruction (InstructionType::JE, labelJE);
            EmitInstruction (InstructionType::MOV, secondIndex, GetTrueLiteral ());
            EmitInstruction (InstructionType::JMP, labelJMP);
            EmitLabel (labelJE);
            EmitInstruction (InstructionType::MOV, secondIndex, GetFalseLiteral ());
            EmitLabel (labelJMP);

            break;
        }

        case TokenOr:
        {
            std::string labelJNE = GenerateLabel ();
            std::string labelJE = GenerateLabel ();
            std::string labelJMP = GenerateLabel ();

            EmitInstruction (InstructionType::CMP, firstIndex, GetFalseLiteral ());
            EmitInstruction (InstructionType::JNE, labelJNE);
            EmitInstruction (InstructionType::CMP, secondIndex, GetFalseLiteral ());
            EmitInstruction (InstructionType::JE, labelJE);
            EmitLabel (labelJNE);
            EmitInstruction (InstructionType::MOV, secondIndex, GetTrueLiteral ());
            EmitInstruction (InstructionType::JMP, labelJMP);
            EmitLabel (labelJE);
            EmitInstruction (InstructionType::MOV, secondIndex, GetFalseLiteral ());
            EmitLabel (labelJMP);

            break;
        }
        }
    }

    //--------------------------------------------------------------------------------

    void agaCodeGenerator::EmitInstruction (InstructionType instruction, const std::string &label)
    {
        std::string codeLine = std::string (instructions[instruction].word) + " .";
        codeLine += label;

        AddCodeLine (codeLine, 1);
    }

    //--------------------------------------------------------------------------------

    void agaCodeGenerator::EmitInstruction (InstructionType instruction, int dstRegisterIndex)
    {
        std::string codeLine = std::string (instructions[instruction].word) + " #";
        codeLine += ToString (dstRegisterIndex);

        AddCodeLine (codeLine, 1);
    }

    //--------------------------------------------------------------------------------

    void agaCodeGenerator::EmitInstruction (InstructionType instruction, int dstRegisterIndex, int srcRegisterIndex)
    {
        std::string codeLine = std::string (instructions[instruction].word) + " #";
        codeLine += ToString (dstRegisterIndex) + ", ";
        codeLine += "#" + ToString (srcRegisterIndex);

        AddCodeLine (codeLine, 1);
    }

    //--------------------------------------------------------------------------------

    void agaCodeGenerator::EmitInstruction (InstructionType instruction, int registerIndex, const std::string &value)
    {
        std::string codeLine = std::string (instructions[instruction].word) + " #";
        codeLine += ToString (registerIndex) + ", ";
        codeLine += value;

        AddCodeLine (codeLine, 1);
    }

    //--------------------------------------------------------------------------------

    void agaCodeGenerator::EmitInstruction (InstructionType instruction, const std::string &value, int registerIndex)
    {
        std::string codeLine = std::string (instructions[instruction].word);
        codeLine += " " + value;
        codeLine += ", #" + ToString (registerIndex);

        AddCodeLine (codeLine, 1);
    }

    //--------------------------------------------------------------------------------

    const std::string agaCodeGenerator::GenerateLabel ()
    {
        std::string codeLine = "_l" + ToString (m_CurrentLabelIndex);

        ++m_CurrentLabelIndex;

        return codeLine;
    }

    //--------------------------------------------------------------------------------

    const std::string agaCodeGenerator::EmitLabel (const std::string &overrideLabel)
    {
        std::string codeLine;

        if (overrideLabel != "")
        {
            codeLine = overrideLabel;
        }
        else
        {
            codeLine = GenerateLabel ();
        }

        AddCodeLine (codeLine + ":");

        return codeLine;
    }

    //--------------------------------------------------------------------------------

    const std::string agaCodeGenerator::GetTrueLiteral () { return "CONST 1"; }

    //--------------------------------------------------------------------------------

    const std::string agaCodeGenerator::GetFalseLiteral () { return "CONST 0"; }

    //--------------------------------------------------------------------------------

    InstructionType agaCodeGenerator::GetInstructionTypeFromCode (const std::string &code)
    {
        for (int i = 0; i < instructionsCount; ++i)
        {
            if (instructions[i].word == code)
            {
                return instructions[i].type;
            }
        }

        return InstructionType::Unknown;
    }

    //--------------------------------------------------------------------------------
}
