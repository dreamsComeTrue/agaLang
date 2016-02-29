#include <sstream>

#include "agaCodeGenerator.h"
#include "agaASTExpression.h"
#include "agaASTAssignment.h"
#include "agaASTBinaryOperator.h"
#include "agaASTBooleanRelation.h"
#include "agaASTLogicalRelation.h"
#include "agaASTProgram.h"
#include "agaUtils.h"

namespace aga
{
//--------------------------------------------------------------------------------

agaCodeGenerator::agaCodeGenerator() :
    m_CurrentRegisterIndex (1),
    m_CurrentLabelIndex (0)
{
}

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

const std::vector<std::string> &agaCodeGenerator::GenerateCode (agaASTProgram *program)
{
    std::vector<agaASTNode *> statements = program->GetStatements ();

    for (int i = 0; i < statements.size(); ++i)
    {
        agaASTNode *statement = statements[i];

        statement->Evaluate();

        GenerateCode (statement);
    }

    return m_Code;
}

//--------------------------------------------------------------------------------

const std::vector<std::string> &agaCodeGenerator::GenerateCode (agaASTNode *node)
{
    switch (node->GetType())
    {
    case  ASTNodeType::AssignmentNode:
        GenerateAssignment (static_cast<agaASTAssignment *> (node));
        break;

    case ASTNodeType::BinaryOperationNode:
        GenerateBinaryExpression (static_cast<agaASTBinaryOperator *> (node));
        break;

    case ASTNodeType::BooleanRelationNode:
        GenerateBooleanRelation (static_cast<agaASTBooleanRelation *> (node));
        break;

    case ASTNodeType::LogicalRelationNode:
        GenerateLogicalRelation (static_cast<agaASTLogicalRelation *> (node));
        break;

    default:
        std::string codeLine = node->GetAllocationBlock().GetCode();

        if (node->GetType() == ConstantNode || node->GetType() == VariableNode)
        {
            EmitInstruction (InstructionType::MOV, m_CurrentRegisterIndex,  node->GetAllocationBlock().GetCode());
        }

        ++m_CurrentRegisterIndex;

        //	AddCodeLine (codeLine);

        std::vector<agaASTNode *> children = node->GetChildren();

        for (int i = 0; i < children.size(); ++i)
        {
            GenerateCode (children[i]);
        }

        break;
    }

    return m_Code;
}

//--------------------------------------------------------------------------------

void agaCodeGenerator::GenerateAssignment (agaASTAssignment *node)
{
    agaASTExpression *expression = node->GetExpression();

    expression->Evaluate();

    GenerateCode (expression);

    node->Evaluate();

    EmitInstruction (InstructionType::MOV, node->GetAllocationBlock().GetCode(), m_CurrentRegisterIndex - 1);
}

//--------------------------------------------------------------------------------

void agaCodeGenerator::GenerateBinaryExpression (agaASTBinaryOperator *node)
{
    std::vector<agaASTNode *> children = node->GetChildren();

    node->Evaluate();
    children[0]->Evaluate ();
    children[1]->Evaluate ();

    ASTNodeType leftType = children[0]->GetType();
    ASTNodeType rightType = children[1]->GetType();

    if (leftType == ASTNodeType::BinaryOperationNode)
    {
        GenerateBinaryExpression (static_cast<agaASTBinaryOperator *> (children[0]));
        children[0]->GetAllocationBlock ().SetRegisterIndex (m_CurrentRegisterIndex - 1);
    }

    if (leftType == ASTNodeType::BooleanRelationNode)
    {
        GenerateBooleanRelation(static_cast<agaASTBooleanRelation *> (children[0]));
        children[0]->GetAllocationBlock ().SetRegisterIndex (m_CurrentRegisterIndex - 1);
    }

    if (rightType == ASTNodeType::BinaryOperationNode)
    {
        GenerateBinaryExpression (static_cast<agaASTBinaryOperator *> (children[1]));
        children[1]->GetAllocationBlock ().SetRegisterIndex (m_CurrentRegisterIndex - 1);
    }

    if (rightType == ASTNodeType::BooleanRelationNode)
    {
        GenerateBooleanRelation (static_cast<agaASTBooleanRelation *> (children[1]));
        children[1]->GetAllocationBlock ().SetRegisterIndex (m_CurrentRegisterIndex - 1);
    }

    if (leftType != ASTNodeType::BinaryOperationNode && leftType != ASTNodeType::BooleanRelationNode)
    {
        EmitInstruction (InstructionType::MOV, m_CurrentRegisterIndex, children[0]->GetAllocationBlock().GetCode());
    }
    else
    {
        EmitInstruction (InstructionType::MOV, m_CurrentRegisterIndex, children[0]->GetAllocationBlock().GetRegisterIndex ());
    }

    InstructionType instructionType = GetInstructionTypeFromCode (node->GetAllocationBlock().GetCode());

    if (rightType != ASTNodeType::BinaryOperationNode && rightType != ASTNodeType::BooleanRelationNode)
    {
        EmitInstruction (instructionType, m_CurrentRegisterIndex, children[1]->GetAllocationBlock().GetCode());
    }
    else
    {
        EmitInstruction (instructionType, m_CurrentRegisterIndex, children[1]->GetAllocationBlock().GetRegisterIndex ());
    }

    ++m_CurrentRegisterIndex;
}

//--------------------------------------------------------------------------------

void agaCodeGenerator::GenerateBooleanRelation (agaASTBooleanRelation *node)
{
    std::vector<agaASTNode *> children = node->GetChildren();

    GenerateCode (children[0]);

    int firstIndex = m_CurrentRegisterIndex - 1;

    GenerateCode (children[1]);

    int secondIndex = m_CurrentRegisterIndex - 1;

    EmitInstruction (InstructionType::CMP, firstIndex, secondIndex);

    switch (node->GetToken().GetType())
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

void agaCodeGenerator::GenerateLogicalRelation (agaASTLogicalRelation *node)
{
    std::vector<agaASTNode *> children = node->GetChildren();

    switch (node->GetToken().GetType())
    {
    case TokenAnd:
    {
        GenerateCode (children[0]);

        int firstIndex = m_CurrentRegisterIndex - 1;

        GenerateCode (children[1]);

        int secondIndex = m_CurrentRegisterIndex - 1;

        EmitInstruction (InstructionType::CMP, firstIndex, secondIndex);

        std::string labelJNE = GenerateLabel();
        std::string labelJE = GenerateLabel();

        EmitInstruction (InstructionType::JNE, labelJNE);
        EmitInstruction (InstructionType::MOV, secondIndex, GetTrueLiteral());
        EmitInstruction (InstructionType::JMP, labelJE);
        EmitLabel(labelJNE);
        EmitInstruction (InstructionType::MOV, secondIndex, GetFalseLiteral());
        EmitLabel(labelJE);

        break;
    }

    case TokenOr:
    {
        GenerateCode (children[0]);

        int firstIndex = m_CurrentRegisterIndex - 1;

        GenerateCode (children[1]);

        int secondIndex = m_CurrentRegisterIndex - 1;

        EmitInstruction (InstructionType::CMP, firstIndex, secondIndex);

        std::string labelJE = GenerateLabel();
        std::string labelJNE = GenerateLabel();

        EmitInstruction (InstructionType::JE, labelJE);
        EmitInstruction (InstructionType::MOV, secondIndex, GetTrueLiteral());
        EmitLabel(labelJE);
        EmitInstruction (InstructionType::MOV, secondIndex, GetFalseLiteral());
        EmitLabel(labelJNE);

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
    codeLine +=  ", #" + ToString (registerIndex);

    AddCodeLine (codeLine, 1);
}

//--------------------------------------------------------------------------------

const std::string agaCodeGenerator::GenerateLabel()
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
        codeLine = GenerateLabel();
    }

    AddCodeLine (codeLine + ":");

    return codeLine;
}

//--------------------------------------------------------------------------------

const std::string agaCodeGenerator::GetTrueLiteral()
{
    return "CONST 1";
}

//--------------------------------------------------------------------------------

const std::string agaCodeGenerator::GetFalseLiteral()
{
    return "CONST 0";
}

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
