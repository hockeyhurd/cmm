#include <cmm/BinOpNode.h>
#include <cmm/CompilationUnitNode.h>
#include <cmm/ExpressionNode.h>
#include <cmm/ExpressionStatementNode.h>
#include <cmm/FunctionCallNode.h>
#include <cmm/FunctionDeclarationStatementNode.h>
#include <cmm/FunctionDefinitionStatementNode.h>
#include <cmm/Lexer.h>
#include <cmm/LitteralNode.h>
#include <cmm/ParenExpressionNode.h>
#include <cmm/Parser.h>
#include <cmm/StatementNode.h>
#include <cmm/Token.h>
#include <cmm/Types.h>
#include <cmm/VariableDeclarationStatementNode.h>
#include <cmm/VariableNode.h>

#include <gtest/gtest.h>

#include <string>

using namespace cmm;

TEST(ParserTest, ParseCompilationNodeEmpty)
{
    const std::string input = "";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);
}

TEST(ParserTest, ParseCompilationNodeBoolTrue)
{
    const std::string input = "true;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::LITTERAL);

    auto* boolPtr = static_cast<LitteralNode*>(expressionStatement->getExpression());
    ASSERT_EQ(boolPtr->getValueType(), EnumCType::BOOL);
    ASSERT_TRUE(boolPtr->getValue().valueBool);
}

TEST(ParserTest, ParseCompilationNodeBoolFalse)
{
    const std::string input = "false ;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::LITTERAL);

    auto* boolPtr = static_cast<LitteralNode*>(expressionStatement->getExpression());
    ASSERT_EQ(boolPtr->getValueType(), EnumCType::BOOL);
    ASSERT_FALSE(boolPtr->getValue().valueBool);
}

TEST(ParserTest, ParseCompilationNodeNullFalse)
{
    const std::string input = "NULL ;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::LITTERAL);

    auto* boolPtr = static_cast<LitteralNode*>(expressionStatement->getExpression());
    ASSERT_EQ(boolPtr->getValueType(), EnumCType::NULL_T);
    ASSERT_EQ(boolPtr->getValue().valueVoidPtr, nullptr);
}

TEST(ParserTest, ParseCompilationNodeInt)
{
    const std::string input = "32;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::LITTERAL);

    auto* rootIntPtr = static_cast<LitteralNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootIntPtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(rootIntPtr->getValue().valueS32, 32);
}

TEST(ParserTest, ParseCompilationNodeIntSum2)
{
    const std::string input = "10 + 32;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rootSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rootSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootSumPtr->getRight()->getType(), NodeType::LITTERAL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rootSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(leftIntPtr->getValue().valueS32, 10);

    auto* rightIntPtr = static_cast<LitteralNode*>(rootSumPtr->getRight());
    ASSERT_EQ(rightIntPtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(rightIntPtr->getValue().valueS32, 32);
}

TEST(ParserTest, ParseCompilationNodeIntSum3)
{
    const std::string input = "10 + 31 + 1;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rootSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rootSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootSumPtr->getRight()->getType(), NodeType::BIN_OP);

    auto* leftIntPtr = static_cast<LitteralNode*>(rootSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(leftIntPtr->getValue().valueS32, 10);

    auto* rightSumPtr = static_cast<BinOpNode*>(rootSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), NodeType::LITTERAL);

    leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(leftIntPtr->getValue().valueS32, 31);

    auto* rightIntPtr = static_cast<LitteralNode*>(rightSumPtr->getRight());
    ASSERT_EQ(rightIntPtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(rightIntPtr->getValue().valueS32, 1);
}

TEST(ParserTest, ParseCompilationNodeFloatSubtract2)
{
    const std::string input = "10.0F - 32.0F;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootSubPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootSubPtr->getTypeof(), EnumBinOpNodeType::SUBTRACT);
    ASSERT_NE(rootSubPtr->getLeft(), nullptr);
    ASSERT_EQ(rootSubPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rootSubPtr->getRight(), nullptr);
    ASSERT_EQ(rootSubPtr->getRight()->getType(), NodeType::LITTERAL);

    auto* leftFloatPtr = static_cast<LitteralNode*>(rootSubPtr->getLeft());
    ASSERT_EQ(leftFloatPtr->getValueType(), EnumCType::FLOAT);
    ASSERT_EQ(leftFloatPtr->getValue().valueF32, 10.0F);

    auto* rightFloatPtr = static_cast<LitteralNode*>(rootSubPtr->getRight());
    ASSERT_EQ(rightFloatPtr->getValueType(), EnumCType::FLOAT);
    ASSERT_EQ(rightFloatPtr->getValue().valueF32, 32.0F);
}

TEST(ParserTest, ParseCompilationNodeIntMultiply2)
{
    const std::string input = "123 * 456789;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootMultPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootMultPtr->getTypeof(), EnumBinOpNodeType::MULTIPLY);
    ASSERT_NE(rootMultPtr->getLeft(), nullptr);
    ASSERT_EQ(rootMultPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rootMultPtr->getRight(), nullptr);
    ASSERT_EQ(rootMultPtr->getRight()->getType(), NodeType::LITTERAL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rootMultPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(leftIntPtr->getValue().valueS32, 123);

    auto* rightIntPtr = static_cast<LitteralNode*>(rootMultPtr->getRight());
    ASSERT_EQ(rightIntPtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(rightIntPtr->getValue().valueS32, 456789);
}

TEST(ParserTest, ParseCompilationNodeFloatDivide2)
{
    const std::string input = "123.0 / 456789.0;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootDivPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootDivPtr->getTypeof(), EnumBinOpNodeType::DIVIDE);
    ASSERT_NE(rootDivPtr->getLeft(), nullptr);
    ASSERT_EQ(rootDivPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rootDivPtr->getRight(), nullptr);
    ASSERT_EQ(rootDivPtr->getRight()->getType(), NodeType::LITTERAL);

    auto* leftDoublePtr = static_cast<LitteralNode*>(rootDivPtr->getLeft());
    ASSERT_EQ(leftDoublePtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(leftDoublePtr->getValue().valueF64, 123.0);

    auto* rightDoublePtr = static_cast<LitteralNode*>(rootDivPtr->getRight());
    ASSERT_EQ(rightDoublePtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(rightDoublePtr->getValue().valueF64, 456789.0);
}

TEST(ParserTest, ParseCompilationNodeIntAssignment)
{
    const std::string input = "a = 10;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), NodeType::VARIABLE);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), NodeType::LITTERAL);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightIntPtr = static_cast<LitteralNode*>(rootAssignPtr->getRight());
    ASSERT_EQ(rightIntPtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(rightIntPtr->getValue().valueS32, 10);
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumAndAssignment)
{
    const std::string input = "a = 123.0 + 32.0;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), NodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), NodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), NodeType::LITTERAL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    auto* rightIntPtr = static_cast<LitteralNode*>(rightSumPtr->getRight());
    ASSERT_EQ(rightIntPtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(rightIntPtr->getValue().valueF64, 32.0);
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithNoArgsAndAssignment)
{
    const std::string input = "a = 123.0 + func();";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), NodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), NodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), NodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    // TODO: Restore this code...
    [[maybe_unused]]
    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    // ASSERT_EQ(rightIntPtr->getValueType(), EnumCType::DOUBLE);
    // ASSERT_EQ(rightIntPtr->getValue().valueF64, 32.0);
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithASingleVariableArgAndAssignment)
{
    const std::string input = "a = 123.0 + func(x);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), NodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), NodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), NodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    // TODO: Restore this code...
    [[maybe_unused]]
    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    // ASSERT_EQ(rightIntPtr->getValueType(), EnumCType::DOUBLE);
    // ASSERT_EQ(rightIntPtr->getValue().valueF64, 32.0);
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithASingleIntArgAndAssignment)
{
    const std::string input = "a = 123.0 + func(42);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), NodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), NodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), NodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    // TODO: Restore this code...
    [[maybe_unused]]
    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    // ASSERT_EQ(rightIntPtr->getValueType(), EnumCType::DOUBLE);
    // ASSERT_EQ(rightIntPtr->getValue().valueF64, 32.0);
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithASingleCharArgAndAssignment)
{
    const std::string input = "a = 123.0 + func('c');";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), NodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), NodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), NodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    // TODO: Restore this code...
    [[maybe_unused]]
    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    // ASSERT_EQ(rightIntPtr->getValueType(), EnumCType::DOUBLE);
    // ASSERT_EQ(rightIntPtr->getValue().valueF64, 32.0);
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithASingleDoubleArgAndAssignment)
{
    const std::string input = "a = 123.0 + func(2.5);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), NodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), NodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), NodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    // TODO: Restore this code...
    [[maybe_unused]]
    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    // ASSERT_EQ(rightIntPtr->getValueType(), EnumCType::DOUBLE);
    // ASSERT_EQ(rightIntPtr->getValue().valueF64, 32.0);
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithASingleBoolArgAndAssignment)
{
    const std::string input = "a = 123.0 + func(true);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), NodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), NodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), NodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    // TODO: Restore this code...
    [[maybe_unused]]
    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    // ASSERT_EQ(rightIntPtr->getValueType(), EnumCType::DOUBLE);
    // ASSERT_EQ(rightIntPtr->getValue().valueF64, 32.0);
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithASingleNullArgAndAssignment)
{
    const std::string input = "a = 123.0 + func(NULL);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), NodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), NodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), NodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    // TODO: Restore this code...
    [[maybe_unused]]
    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    // ASSERT_EQ(rightIntPtr->getValueType(), EnumCType::DOUBLE);
    // ASSERT_EQ(rightIntPtr->getValue().valueF64, 32.0);
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithASingleStringArgAndAssignment)
{
    const std::string input = "a = 123.0 + func(\"Hello, world!\");";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), NodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), NodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), NodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    // TODO: Restore this code...
    [[maybe_unused]]
    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    // ASSERT_EQ(rightIntPtr->getValueType(), EnumCType::DOUBLE);
    // ASSERT_EQ(rightIntPtr->getValue().valueF64, 32.0);
}

TEST(ParserTest, ParseCompilationNodeIntDeclarationStatement)
{
    const std::string input = "int x;";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::VARIABLE_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<VariableDeclarationStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, name);
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDeclarationStatement)
{
    const std::string input = "int x();";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<FunctionDeclarationStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, name);
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDefinitionStatementEmptyBlock)
{
    const std::string input = "int x() {}";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_TRUE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 0);

    const auto iter = blockNode.cbegin();
    ASSERT_EQ(iter, blockNode.cend());
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDefinitionStatementWithSingleStatementInBlock)
{
    const std::string input = "int x() { float y; }";
    const std::string funcName = "x";
    const std::string varName = "y";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, funcName);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_FALSE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 1);

    auto iter = blockNode.cbegin();
    ASSERT_NE(iter, blockNode.cend());

    const auto& declPtr = *iter;
    ASSERT_EQ(declPtr->getType(), NodeType::VARIABLE_DECLARATION_STATEMENT);

    const auto* varDeclPtr = static_cast<VariableDeclarationStatementNode*>(iter->get());
    ASSERT_EQ(varDeclPtr->getDatatype(), EnumCType::FLOAT);
    ASSERT_EQ(varDeclPtr->getName(), varName);

    ++iter;
    ASSERT_EQ(iter, blockNode.cend());
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDefinitionStatementWithDoubleStatementInBlock)
{
    const std::string input = "int x() { float y; char z; }";
    const std::string funcName = "x";
    const std::string var1Name = "y";
    const std::string var2Name = "z";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, funcName);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_FALSE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 2);

    auto iter = blockNode.cbegin();

    {
        ASSERT_NE(iter, blockNode.cend());

        const auto& declPtr = *iter;
        ASSERT_EQ(declPtr->getType(), NodeType::VARIABLE_DECLARATION_STATEMENT);

        const auto* varDeclPtr = static_cast<VariableDeclarationStatementNode*>(iter->get());
        ASSERT_EQ(varDeclPtr->getDatatype(), EnumCType::FLOAT);
        ASSERT_EQ(varDeclPtr->getName(), var1Name);
    }

    ++iter;
    ASSERT_NE(iter, blockNode.cend());

    {
        const auto& declPtr = *iter;
        ASSERT_EQ(declPtr->getType(), NodeType::VARIABLE_DECLARATION_STATEMENT);

        const auto* varDeclPtr = static_cast<VariableDeclarationStatementNode*>(iter->get());
        ASSERT_EQ(varDeclPtr->getDatatype(), EnumCType::CHAR);
        ASSERT_EQ(varDeclPtr->getName(), var2Name);
    }

    ++iter;
    ASSERT_EQ(iter, blockNode.cend());
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDeclarationStatementWithSingleParam)
{
    const std::string input = "int x(int y);";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<FunctionDeclarationStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, name);
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDeclarationStatementWithSingleParamButNoName)
{
    const std::string input = "int x(int);";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<FunctionDeclarationStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, name);
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDeclarationStatementWithTwoParams)
{
    const std::string input = "int x(int y, double z);";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<FunctionDeclarationStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, name);
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDeclarationStatementWithTwoParamsButNoNames)
{
    const std::string input = "int x(int y, double z);";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<FunctionDeclarationStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, name);
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDeclarationStatementWithMultipleParams)
{
    const std::string input = "int x(char a, double b, long c);";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<FunctionDeclarationStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, name);
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDeclarationStatementWithMultipleParamsButNoNames)
{
    const std::string input = "int x(char, double, long);";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<FunctionDeclarationStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, name);
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDefinitionStatementEmptyBlockWithSingleParam)
{
    const std::string input = "int x(int y) {}";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_TRUE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 0);

    const auto iter = blockNode.cbegin();
    ASSERT_EQ(iter, blockNode.cend());
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDefinitionStatementEmptyBlockWithSingleParamButNoName)
{
    const std::string input = "int x(int) {}";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_TRUE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 0);

    const auto iter = blockNode.cbegin();
    ASSERT_EQ(iter, blockNode.cend());
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDefinitionStatementEmptyBlockWithTwoParams)
{
    const std::string input = "int x(int y, double z) {}";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_TRUE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 0);

    const auto iter = blockNode.cbegin();
    ASSERT_EQ(iter, blockNode.cend());
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDefinitionStatementEmptyBlockWithTwoParamsButNoNames)
{
    const std::string input = "int x(int, double) {}";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_TRUE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 0);

    const auto iter = blockNode.cbegin();
    ASSERT_EQ(iter, blockNode.cend());
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDefinitionStatementEmptyBlockWithMultipleParams)
{
    const std::string input = "int x(int a, char b, float c) {}";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_TRUE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 0);

    const auto iter = blockNode.cbegin();
    ASSERT_EQ(iter, blockNode.cend());
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDefinitionStatementEmptyBlockWithMultipleParamsButNoNames)
{
    const std::string input = "int x(int, char, float) {}";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype(), EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_TRUE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 0);

    const auto iter = blockNode.cbegin();
    ASSERT_EQ(iter, blockNode.cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementNoArgs)
{
    const std::string input = "func();";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementWithASingleVariableArg)
{
    const std::string input = "func(x);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    // TODO: Finish checks here:
    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    const auto* expressionNodePtr = argListIter->getValue();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), NodeType::VARIABLE);

    const auto* variableNodePtr = static_cast<const VariableNode*>(expressionNodePtr);
    ASSERT_EQ(variableNodePtr->getName(), "x");

    ++argListIter;
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementWithASingleBoolArg)
{
    const std::string input = "func(false);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    // TODO: Finish checks here:
    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    const auto* expressionNodePtr = argListIter->getValue();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::BOOL);
    ASSERT_EQ(litteralNodePtr->getValue().valueBool, false);

    ++argListIter;
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementWithASingleCharArg)
{
    const std::string input = "func('A');";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    // TODO: Finish checks here:
    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    const auto* expressionNodePtr = argListIter->getValue();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::CHAR);
    ASSERT_EQ(litteralNodePtr->getValue().valueChar, 'A');

    ++argListIter;
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementWithASingleDoubleArg)
{
    const std::string input = "func(2.5);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    // TODO: Finish checks here:
    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    const auto* expressionNodePtr = argListIter->getValue();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(litteralNodePtr->getValue().valueF64, 2.5);

    ++argListIter;
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementWithASingleIntArg)
{
    const std::string input = "func(42);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    // TODO: Finish checks here:
    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    const auto* expressionNodePtr = argListIter->getValue();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(litteralNodePtr->getValue().valueS32, 42);

    ++argListIter;
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementWithASingleNullArg)
{
    const std::string input = "func(NULL);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    // TODO: Finish checks here:
    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    const auto* expressionNodePtr = argListIter->getValue();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

    // TODO: This fails... resolve this issue.
    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::NULL_T);

    ++argListIter;
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementWithASingleStringArg)
{
    const std::string input = "func(\"Hello, world!\");";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    // TODO: This fails here:
    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    const auto* expressionNodePtr = argListIter->getValue();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::STRING);
    ASSERT_EQ(strncmp(litteralNodePtr->getValue().valueString, "Hello, world!", 16), 0);

    ++argListIter;
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementWithTwoVariablesArgs)
{
    const std::string input = "func(x, y);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getValue();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), NodeType::VARIABLE);

        const auto* variableNodePtr = static_cast<const VariableNode*>(expressionNodePtr);
        ASSERT_EQ(variableNodePtr->getName(), "x");
    }

    ++argListIter;
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getValue();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), NodeType::VARIABLE);

        const auto* variableNodePtr = static_cast<const VariableNode*>(expressionNodePtr);
        ASSERT_EQ(variableNodePtr->getName(), "y");
    }

    ++argListIter;
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementWithTwoBoolArgs)
{
    const std::string input = "func(false, true);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    // TODO: Finish checks here:
    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getValue();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::BOOL);
        ASSERT_EQ(litteralNodePtr->getValue().valueBool, false);
    }

    ++argListIter;
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getValue();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::BOOL);
        ASSERT_EQ(litteralNodePtr->getValue().valueBool, true);
    }

    ++argListIter;
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementWithTwoCharArgs)
{
    const std::string input = "func('A', 'B');";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    // TODO: Finish checks here:
    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getValue();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::CHAR);
        ASSERT_EQ(litteralNodePtr->getValue().valueChar, 'A');
    }

    ++argListIter;
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getValue();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::CHAR);
        ASSERT_EQ(litteralNodePtr->getValue().valueChar, 'B');
    }

    ++argListIter;
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementWithTwoDoubleArgs)
{
    const std::string input = "func(123.45, -100.001);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    // TODO: Finish checks here:
    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getValue();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::DOUBLE);
        ASSERT_EQ(litteralNodePtr->getValue().valueF64, 123.45);
    }

    ++argListIter;
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getValue();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::DOUBLE);
        ASSERT_EQ(litteralNodePtr->getValue().valueF64, -100.001);
    }

    ++argListIter;
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementWithTwoIntArgs)
{
    const std::string input = "func(42, -1);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    // TODO: Finish checks here:
    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getValue();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::INT32);
        ASSERT_EQ(litteralNodePtr->getValue().valueS32, 42);
    }

    ++argListIter;
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getValue();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::INT32);
        ASSERT_EQ(litteralNodePtr->getValue().valueS32, -1);
    }

    ++argListIter;
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementWithTwoNullArgs)
{
    const std::string input = "func(NULL, NULL);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    // TODO: Finish checks here:
    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getValue();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

        // TODO: This fails... resolve this issue.
        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::NULL_T);
    }

    ++argListIter;
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getValue();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

        // TODO: This fails... resolve this issue.
        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::NULL_T);
    }

    ++argListIter;
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementWithTwoStringArgs)
{
    const std::string input = "func(\"Hello, world!\", \"My name is... the REAL SLIM SHADY!!\");";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getValue();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::STRING);
        ASSERT_EQ(strncmp(litteralNodePtr->getValue().valueString, "Hello, world!", 16), 0);
    }

    ++argListIter;
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getValue();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), NodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getValueType(), EnumCType::STRING);
        ASSERT_EQ(strncmp(litteralNodePtr->getValue().valueString, "My name is... the REAL SLIM SHADY!!", 48), 0);
    }

    ++argListIter;
    ASSERT_EQ(argListIter, functionCallPtr->cend());
}


TEST(ParserTest, ParseCompilationNodeSingleParenWrappedIntLitteral)
{
    const std::string input = "(42);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::PAREN_EXPRESSION);

    auto* parenExpressionPtr = static_cast<ParenExpressionNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(parenExpressionPtr->getExpression(), nullptr);
    ASSERT_EQ(parenExpressionPtr->getExpression()->getType(), NodeType::LITTERAL);

    auto* intPtr = static_cast<LitteralNode*>(parenExpressionPtr->getExpression());
    ASSERT_EQ(intPtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(intPtr->getValue().valueS32, 42);
}

TEST(ParserTest, ParseCompilationNodeMultipleParenWrappedVariable)
{
    const std::string input = "((x));";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::PAREN_EXPRESSION);

    auto* parenExpressionPtr = static_cast<ParenExpressionNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(parenExpressionPtr->getExpression(), nullptr);
    ASSERT_EQ(parenExpressionPtr->getExpression()->getType(), NodeType::PAREN_EXPRESSION);

    parenExpressionPtr = static_cast<ParenExpressionNode*>(parenExpressionPtr->getExpression());
    ASSERT_NE(parenExpressionPtr->getExpression(), nullptr);
    ASSERT_EQ(parenExpressionPtr->getExpression()->getType(), NodeType::VARIABLE);

    auto variableNodePtr = static_cast<VariableNode*>(parenExpressionPtr->getExpression());
    ASSERT_EQ(variableNodePtr->getName(), "x");
}

TEST(ParserTest, ParseCompilationNodeSingleMissingSingleClosingParen)
{
    const std::string input = "(x;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);
}

TEST(ParserTest, ParseCompilationNodeMultipleMissingSingleClosingParen)
{
    const std::string input = "((x);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);
}

TEST(ParserTest, ParseCompilationNodeMultipleMissingMultipleClosingParen)
{
    const std::string input = "((x);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);
}

s32 main(s32 argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

