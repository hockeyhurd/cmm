#include <cmm/BinOpNode.h>
#include <cmm/CompilationUnitNode.h>
#include <cmm/Lexer.h>
#include <cmm/LitteralNode.h>
#include <cmm/Parser.h>
#include <cmm/Token.h>
#include <cmm/Types.h>
#include <cmm/DeclarationStatementNode.h>
#include <cmm/ExpressionNode.h>
#include <cmm/ExpressionStatementNode.h>
#include <cmm/ParenExpressionNode.h>
#include <cmm/StatementNode.h>
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

    auto expressionStatement = std::static_pointer_cast<ExpressionStatementNode>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::LITTERAL);

    auto boolPtr = std::static_pointer_cast<LitteralNode>(expressionStatement->getExpression());
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

    auto expressionStatement = std::static_pointer_cast<ExpressionStatementNode>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::LITTERAL);

    auto boolPtr = std::static_pointer_cast<LitteralNode>(expressionStatement->getExpression());
    ASSERT_EQ(boolPtr->getValueType(), EnumCType::BOOL);
    ASSERT_FALSE(boolPtr->getValue().valueBool);
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

    auto expressionStatement = std::static_pointer_cast<ExpressionStatementNode>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::LITTERAL);

    auto rootIntPtr = std::static_pointer_cast<LitteralNode>(expressionStatement->getExpression());
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

    auto expressionStatement = std::static_pointer_cast<ExpressionStatementNode>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto rootSumPtr = std::static_pointer_cast<BinOpNode>(expressionStatement->getExpression());
    ASSERT_EQ(rootSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rootSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rootSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootSumPtr->getRight()->getType(), NodeType::LITTERAL);

    auto leftIntPtr = std::static_pointer_cast<LitteralNode>(rootSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(leftIntPtr->getValue().valueS32, 10);

    auto rightIntPtr = std::static_pointer_cast<LitteralNode>(rootSumPtr->getRight());
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

    auto expressionStatement = std::static_pointer_cast<ExpressionStatementNode>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto rootSumPtr = std::static_pointer_cast<BinOpNode>(expressionStatement->getExpression());
    ASSERT_EQ(rootSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rootSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rootSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootSumPtr->getRight()->getType(), NodeType::BIN_OP);

    auto leftIntPtr = std::static_pointer_cast<LitteralNode>(rootSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(leftIntPtr->getValue().valueS32, 10);

    auto rightSumPtr = std::static_pointer_cast<BinOpNode>(rootSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), NodeType::LITTERAL);

    leftIntPtr = std::static_pointer_cast<LitteralNode>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(leftIntPtr->getValue().valueS32, 31);

    auto rightIntPtr = std::static_pointer_cast<LitteralNode>(rightSumPtr->getRight());
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

    auto expressionStatement = std::static_pointer_cast<ExpressionStatementNode>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto rootSubPtr = std::static_pointer_cast<BinOpNode>(expressionStatement->getExpression());
    ASSERT_EQ(rootSubPtr->getTypeof(), EnumBinOpNodeType::SUBTRACT);
    ASSERT_NE(rootSubPtr->getLeft(), nullptr);
    ASSERT_EQ(rootSubPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rootSubPtr->getRight(), nullptr);
    ASSERT_EQ(rootSubPtr->getRight()->getType(), NodeType::LITTERAL);

    auto leftFloatPtr = std::static_pointer_cast<LitteralNode>(rootSubPtr->getLeft());
    ASSERT_EQ(leftFloatPtr->getValueType(), EnumCType::FLOAT);
    ASSERT_EQ(leftFloatPtr->getValue().valueF32, 10.0F);

    auto rightFloatPtr = std::static_pointer_cast<LitteralNode>(rootSubPtr->getRight());
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

    auto expressionStatement = std::static_pointer_cast<ExpressionStatementNode>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto rootMultPtr = std::static_pointer_cast<BinOpNode>(expressionStatement->getExpression());
    ASSERT_EQ(rootMultPtr->getTypeof(), EnumBinOpNodeType::MULTIPLY);
    ASSERT_NE(rootMultPtr->getLeft(), nullptr);
    ASSERT_EQ(rootMultPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rootMultPtr->getRight(), nullptr);
    ASSERT_EQ(rootMultPtr->getRight()->getType(), NodeType::LITTERAL);

    auto leftIntPtr = std::static_pointer_cast<LitteralNode>(rootMultPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::INT32);
    ASSERT_EQ(leftIntPtr->getValue().valueS32, 123);

    auto rightIntPtr = std::static_pointer_cast<LitteralNode>(rootMultPtr->getRight());
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

    auto expressionStatement = std::static_pointer_cast<ExpressionStatementNode>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto rootDivPtr = std::static_pointer_cast<BinOpNode>(expressionStatement->getExpression());
    ASSERT_EQ(rootDivPtr->getTypeof(), EnumBinOpNodeType::DIVIDE);
    ASSERT_NE(rootDivPtr->getLeft(), nullptr);
    ASSERT_EQ(rootDivPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rootDivPtr->getRight(), nullptr);
    ASSERT_EQ(rootDivPtr->getRight()->getType(), NodeType::LITTERAL);

    auto leftDoublePtr = std::static_pointer_cast<LitteralNode>(rootDivPtr->getLeft());
    ASSERT_EQ(leftDoublePtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(leftDoublePtr->getValue().valueF64, 123.0);

    auto rightDoublePtr = std::static_pointer_cast<LitteralNode>(rootDivPtr->getRight());
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

    auto expressionStatement = std::static_pointer_cast<ExpressionStatementNode>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto rootAssignPtr = std::static_pointer_cast<BinOpNode>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), NodeType::VARIABLE);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), NodeType::LITTERAL);

    auto leftVariablePtr = std::static_pointer_cast<VariableNode>(rootAssignPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto rightIntPtr = std::static_pointer_cast<LitteralNode>(rootAssignPtr->getRight());
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

    auto expressionStatement = std::static_pointer_cast<ExpressionStatementNode>(compUnitPtr->getRoot());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), NodeType::BIN_OP);

    auto rootAssignAndSumPtr = std::static_pointer_cast<BinOpNode>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), NodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), NodeType::BIN_OP);

    auto leftVariablePtr = std::static_pointer_cast<VariableNode>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto rightSumPtr = std::static_pointer_cast<BinOpNode>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), NodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), NodeType::LITTERAL);

    auto leftIntPtr = std::static_pointer_cast<LitteralNode>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    auto rightIntPtr = std::static_pointer_cast<LitteralNode>(rightSumPtr->getRight());
    ASSERT_EQ(rightIntPtr->getValueType(), EnumCType::DOUBLE);
    ASSERT_EQ(rightIntPtr->getValue().valueF64, 32.0);
}

TEST(ParserTest, ParseCompilationNodeIntDeclarationStatement)
{
    const std::string input = "int x;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::DECLARATION_STATEMENT);

    auto rootDeclarationStatementPtr = std::static_pointer_cast<DeclarationStatementNode>(compUnitPtr->getRoot());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype(), EnumCType::INT32);
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

    auto rootExpressionStatementPtr = std::static_pointer_cast<ExpressionStatementNode>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::PAREN_EXPRESSION);

    auto parenExpressionPtr = std::static_pointer_cast<ParenExpressionNode>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(parenExpressionPtr->getExpression(), nullptr);
    ASSERT_EQ(parenExpressionPtr->getExpression()->getType(), NodeType::LITTERAL);

    auto intPtr = std::static_pointer_cast<LitteralNode>(parenExpressionPtr->getExpression());
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

    auto rootExpressionStatementPtr = std::static_pointer_cast<ExpressionStatementNode>(compUnitPtr->getRoot());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), NodeType::PAREN_EXPRESSION);

    auto parenExpressionPtr = std::static_pointer_cast<ParenExpressionNode>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(parenExpressionPtr->getExpression(), nullptr);
    ASSERT_EQ(parenExpressionPtr->getExpression()->getType(), NodeType::PAREN_EXPRESSION);

    parenExpressionPtr = std::static_pointer_cast<ParenExpressionNode>(parenExpressionPtr->getExpression());
    ASSERT_NE(parenExpressionPtr->getExpression(), nullptr);
    ASSERT_EQ(parenExpressionPtr->getExpression()->getType(), NodeType::VARIABLE);

    auto variableNodePtr = std::static_pointer_cast<VariableNode>(parenExpressionPtr->getExpression());
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

