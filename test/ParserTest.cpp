#include <cmm/NodeList.h>
#include <cmm/Parser.h>
#include <cmm/Reporter.h>

#include <gtest/gtest.h>

#include <string>

using namespace cmm;

static Reporter& reporter = Reporter::instance();

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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::LITTERAL);

    auto* boolPtr = static_cast<LitteralNode*>(expressionStatement->getExpression());
    ASSERT_EQ(boolPtr->getDatatype().type, EnumCType::BOOL);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::LITTERAL);

    auto* boolPtr = static_cast<LitteralNode*>(expressionStatement->getExpression());
    ASSERT_EQ(boolPtr->getDatatype().type, EnumCType::BOOL);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::LITTERAL);

    auto* boolPtr = static_cast<LitteralNode*>(expressionStatement->getExpression());
    ASSERT_EQ(boolPtr->getDatatype().type, EnumCType::NULL_T);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::LITTERAL);

    auto* rootIntPtr = static_cast<LitteralNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootIntPtr->getDatatype().type, EnumCType::INT32);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rootSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootSumPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rootSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootSumPtr->getRight()->getType(), EnumNodeType::LITTERAL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rootSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(leftIntPtr->getValue().valueS32, 10);

    auto* rightIntPtr = static_cast<LitteralNode*>(rootSumPtr->getRight());
    ASSERT_EQ(rightIntPtr->getDatatype().type, EnumCType::INT32);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rootSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootSumPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rootSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootSumPtr->getRight()->getType(), EnumNodeType::BIN_OP);

    auto* leftIntPtr = static_cast<LitteralNode*>(rootSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(leftIntPtr->getValue().valueS32, 10);

    auto* rightSumPtr = static_cast<BinOpNode*>(rootSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), EnumNodeType::LITTERAL);

    leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(leftIntPtr->getValue().valueS32, 31);

    auto* rightIntPtr = static_cast<LitteralNode*>(rightSumPtr->getRight());
    ASSERT_EQ(rightIntPtr->getDatatype().type, EnumCType::INT32);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootSubPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootSubPtr->getTypeof(), EnumBinOpNodeType::SUBTRACT);
    ASSERT_NE(rootSubPtr->getLeft(), nullptr);
    ASSERT_EQ(rootSubPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rootSubPtr->getRight(), nullptr);
    ASSERT_EQ(rootSubPtr->getRight()->getType(), EnumNodeType::LITTERAL);

    auto* leftFloatPtr = static_cast<LitteralNode*>(rootSubPtr->getLeft());
    ASSERT_EQ(leftFloatPtr->getDatatype().type, EnumCType::FLOAT);
    ASSERT_EQ(leftFloatPtr->getValue().valueF32, 10.0F);

    auto* rightFloatPtr = static_cast<LitteralNode*>(rootSubPtr->getRight());
    ASSERT_EQ(rightFloatPtr->getDatatype().type, EnumCType::FLOAT);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootMultPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootMultPtr->getTypeof(), EnumBinOpNodeType::MULTIPLY);
    ASSERT_NE(rootMultPtr->getLeft(), nullptr);
    ASSERT_EQ(rootMultPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rootMultPtr->getRight(), nullptr);
    ASSERT_EQ(rootMultPtr->getRight()->getType(), EnumNodeType::LITTERAL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rootMultPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(leftIntPtr->getValue().valueS32, 123);

    auto* rightIntPtr = static_cast<LitteralNode*>(rootMultPtr->getRight());
    ASSERT_EQ(rightIntPtr->getDatatype().type, EnumCType::INT32);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootDivPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootDivPtr->getTypeof(), EnumBinOpNodeType::DIVIDE);
    ASSERT_NE(rootDivPtr->getLeft(), nullptr);
    ASSERT_EQ(rootDivPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rootDivPtr->getRight(), nullptr);
    ASSERT_EQ(rootDivPtr->getRight()->getType(), EnumNodeType::LITTERAL);

    auto* leftDoublePtr = static_cast<LitteralNode*>(rootDivPtr->getLeft());
    ASSERT_EQ(leftDoublePtr->getDatatype().type, EnumCType::DOUBLE);
    ASSERT_EQ(leftDoublePtr->getValue().valueF64, 123.0);

    auto* rightDoublePtr = static_cast<LitteralNode*>(rootDivPtr->getRight());
    ASSERT_EQ(rightDoublePtr->getDatatype().type, EnumCType::DOUBLE);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), EnumNodeType::LITTERAL);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightIntPtr = static_cast<LitteralNode*>(rootAssignPtr->getRight());
    ASSERT_EQ(rightIntPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(rightIntPtr->getValue().valueS32, 10);
}

TEST(ParserTest, ParseCompilationNodeIntAssignmentViaPointer)
{
    const std::string input = "*a = 10;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), EnumNodeType::DEREF);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), EnumNodeType::LITTERAL);

    auto* leftDerefPtr = static_cast<DerefNode*>(rootAssignPtr->getLeft());
    ASSERT_TRUE(leftDerefPtr->hasExpression());
    ASSERT_NE(leftDerefPtr->getExpression(), nullptr);
    ASSERT_EQ(leftDerefPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* leftVariablePtr = static_cast<VariableNode*>(leftDerefPtr->getExpression());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightIntPtr = static_cast<LitteralNode*>(rootAssignPtr->getRight());
    ASSERT_EQ(rightIntPtr->getDatatype().type, EnumCType::INT32);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), EnumNodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), EnumNodeType::LITTERAL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    auto* rightIntPtr = static_cast<LitteralNode*>(rightSumPtr->getRight());
    ASSERT_EQ(rightIntPtr->getDatatype().type, EnumCType::DOUBLE);
    ASSERT_EQ(rightIntPtr->getValue().valueF64, 32.0);
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignToParenAddExpression)
{
    const std::string input = "a = (10 + 32);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignAndParenSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndParenSumPtr ->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndParenSumPtr ->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndParenSumPtr ->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignAndParenSumPtr ->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndParenSumPtr ->getRight()->getType(), EnumNodeType::PAREN_EXPRESSION);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndParenSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightParenPtr = static_cast<ParenExpressionNode*>(rootAssignAndParenSumPtr->getRight());
    ASSERT_NE(rightParenPtr->getExpression(), nullptr);
    ASSERT_EQ(rightParenPtr->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* sumExpressionPtr = static_cast<BinOpNode*>(rightParenPtr->getExpression());
    ASSERT_EQ(sumExpressionPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(sumExpressionPtr->getLeft(), nullptr);
    ASSERT_EQ(sumExpressionPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(sumExpressionPtr->getRight(), nullptr);
    ASSERT_EQ(sumExpressionPtr->getRight()->getType(), EnumNodeType::LITTERAL);

    auto* leftIntPtr = static_cast<LitteralNode*>(sumExpressionPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(leftIntPtr->getValue().valueS32, 10);

    auto* rightIntPtr = static_cast<LitteralNode*>(sumExpressionPtr->getRight());
    ASSERT_EQ(rightIntPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(rightIntPtr->getValue().valueS32, 32);
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignToNestedParenAddExpression)
{
    const std::string input = "a = (11 + (32 - 1));";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignAndParenSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndParenSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndParenSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndParenSumPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignAndParenSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndParenSumPtr->getRight()->getType(), EnumNodeType::PAREN_EXPRESSION);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndParenSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightParenPtr = static_cast<ParenExpressionNode*>(rootAssignAndParenSumPtr->getRight());
    ASSERT_NE(rightParenPtr->getExpression(), nullptr);
    ASSERT_EQ(rightParenPtr->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* firstBinOpPtr = static_cast<BinOpNode*>(rightParenPtr->getExpression());
    ASSERT_EQ(firstBinOpPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(firstBinOpPtr->getLeft(), nullptr);
    ASSERT_EQ(firstBinOpPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(firstBinOpPtr->getRight(), nullptr);
    ASSERT_EQ(firstBinOpPtr->getRight()->getType(), EnumNodeType::PAREN_EXPRESSION);

    auto* nestedParenExpressionPtr = static_cast<ParenExpressionNode*>(firstBinOpPtr->getRight());
    ASSERT_NE(nestedParenExpressionPtr->getExpression(), nullptr);
    ASSERT_EQ(nestedParenExpressionPtr->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* nestedBinOpPtr = static_cast<BinOpNode*>(nestedParenExpressionPtr->getExpression());
    ASSERT_EQ(nestedBinOpPtr->getTypeof(), EnumBinOpNodeType::SUBTRACT);
    ASSERT_NE(nestedBinOpPtr->getLeft(), nullptr);
    ASSERT_EQ(nestedBinOpPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(nestedBinOpPtr->getRight(), nullptr);
    ASSERT_EQ(nestedBinOpPtr->getRight()->getType(), EnumNodeType::LITTERAL);

    auto* nestedLeftIntPtr = static_cast<LitteralNode*>(nestedBinOpPtr->getLeft());
    ASSERT_EQ(nestedLeftIntPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(nestedLeftIntPtr->getValue().valueS32, 32);

    auto* nestedRightIntPtr = static_cast<LitteralNode*>(nestedBinOpPtr->getRight());
    ASSERT_EQ(nestedRightIntPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(nestedRightIntPtr->getValue().valueS32, 1);
}

TEST(ParserTest, ParseCompilationNodeParenAddExpressionThenMult)
{
    const std::string input = "a = 2 + (3 * 2);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignAndBinOpPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndBinOpPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndBinOpPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndBinOpPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignAndBinOpPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndBinOpPtr->getRight()->getType(), EnumNodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndBinOpPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightBinOpPtr = static_cast<BinOpNode*>(rootAssignAndBinOpPtr->getRight());
    ASSERT_EQ(rightBinOpPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightBinOpPtr->getLeft(), nullptr);
    ASSERT_EQ(rightBinOpPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rightBinOpPtr->getRight(), nullptr);
    ASSERT_EQ(rightBinOpPtr->getRight()->getType(), EnumNodeType::PAREN_EXPRESSION);

    auto* leftLitteralTwoPtr = static_cast<LitteralNode*>(rightBinOpPtr->getLeft());
    ASSERT_EQ(leftLitteralTwoPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(leftLitteralTwoPtr->getDatatype().pointers, 0);
    ASSERT_EQ(leftLitteralTwoPtr->getValue().valueS32, 2);

    auto* rightParenPtr = static_cast<ParenExpressionNode*>(rightBinOpPtr->getRight());
    ASSERT_NE(rightParenPtr->getExpression(), nullptr);
    ASSERT_EQ(rightParenPtr->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* innerBinOpPtr = static_cast<BinOpNode*>(rightParenPtr->getExpression());
    ASSERT_EQ(innerBinOpPtr->getTypeof(), EnumBinOpNodeType::MULTIPLY);
    ASSERT_NE(innerBinOpPtr->getLeft(), nullptr);
    ASSERT_EQ(innerBinOpPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(innerBinOpPtr->getRight(), nullptr);
    ASSERT_EQ(innerBinOpPtr->getRight()->getType(), EnumNodeType::LITTERAL);

    auto* innerLeftLitteralPtr = static_cast<LitteralNode*>(innerBinOpPtr->getLeft());
    ASSERT_EQ(innerLeftLitteralPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(innerLeftLitteralPtr->getDatatype().pointers, 0);
    ASSERT_EQ(innerLeftLitteralPtr->getValue().valueS32, 3);

    auto* innerRightLitteralPtr = static_cast<LitteralNode*>(innerBinOpPtr->getRight());
    ASSERT_EQ(innerRightLitteralPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(innerRightLitteralPtr->getDatatype().pointers, 0);
    ASSERT_EQ(innerRightLitteralPtr->getValue().valueS32, 2);
}

TEST(ParserTest, ParseCompilationNodeParenMultExpressionThenAdd)
{
    const std::string input = "a = (2 * 3) + 2;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignAndBinOpPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndBinOpPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndBinOpPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndBinOpPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignAndBinOpPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndBinOpPtr->getRight()->getType(), EnumNodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndBinOpPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightBinOpPtr = static_cast<BinOpNode*>(rootAssignAndBinOpPtr->getRight());
    ASSERT_EQ(rightBinOpPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightBinOpPtr->getLeft(), nullptr);
    ASSERT_EQ(rightBinOpPtr->getLeft()->getType(), EnumNodeType::PAREN_EXPRESSION);
    ASSERT_NE(rightBinOpPtr->getRight(), nullptr);
    ASSERT_EQ(rightBinOpPtr->getRight()->getType(), EnumNodeType::LITTERAL);

    auto* rightLitteralTwoPtr = static_cast<LitteralNode*>(rightBinOpPtr->getRight());
    ASSERT_EQ(rightLitteralTwoPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(rightLitteralTwoPtr->getDatatype().pointers, 0);
    ASSERT_EQ(rightLitteralTwoPtr->getValue().valueS32, 2);

    auto* leftParenPtr = static_cast<ParenExpressionNode*>(rightBinOpPtr->getLeft());
    ASSERT_NE(leftParenPtr->getExpression(), nullptr);
    ASSERT_EQ(leftParenPtr->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* innerBinOpPtr = static_cast<BinOpNode*>(leftParenPtr->getExpression());
    ASSERT_EQ(innerBinOpPtr->getTypeof(), EnumBinOpNodeType::MULTIPLY);
    ASSERT_NE(innerBinOpPtr->getLeft(), nullptr);
    ASSERT_EQ(innerBinOpPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(innerBinOpPtr->getRight(), nullptr);
    ASSERT_EQ(innerBinOpPtr->getRight()->getType(), EnumNodeType::LITTERAL);

    auto* innerRightLitteralPtr = static_cast<LitteralNode*>(innerBinOpPtr->getRight());
    ASSERT_EQ(innerRightLitteralPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(innerRightLitteralPtr->getDatatype().pointers, 0);
    ASSERT_EQ(innerRightLitteralPtr->getValue().valueS32, 3);

    auto* innerLeftLitteralPtr = static_cast<LitteralNode*>(innerBinOpPtr->getLeft());
    ASSERT_EQ(innerLeftLitteralPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(innerLeftLitteralPtr->getDatatype().pointers, 0);
    ASSERT_EQ(innerLeftLitteralPtr->getValue().valueS32, 2);
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithNoArgsAndAssignment)
{
    const std::string input = "a = 123.0 + func();";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), EnumNodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    ASSERT_EQ(rightFunctionCallPtr->getName(), "func");
    ASSERT_TRUE(rightFunctionCallPtr->empty());
    ASSERT_EQ(rightFunctionCallPtr->size(), 0);

    const auto iter = rightFunctionCallPtr->cbegin();
    ASSERT_EQ(iter, rightFunctionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithASingleVariableArgAndAssignment)
{
    const std::string input = "a = 123.0 + func(x);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), EnumNodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    ASSERT_EQ(rightFunctionCallPtr->getName(), "func");
    ASSERT_FALSE(rightFunctionCallPtr->empty());
    ASSERT_EQ(rightFunctionCallPtr->size(), 1);

    auto iter = rightFunctionCallPtr->cbegin();
    ASSERT_NE(iter, rightFunctionCallPtr->cend());

    const auto* expressionNodePtr = iter->getExpression();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::VARIABLE);

    const auto* variableNodePtr = static_cast<const VariableNode*>(expressionNodePtr);
    ASSERT_EQ(variableNodePtr->getName(), "x");

    ++iter;
    ASSERT_EQ(iter, rightFunctionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithASingleIntArgAndAssignment)
{
    const std::string input = "a = 123.0 + func(42);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), EnumNodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    ASSERT_EQ(rightFunctionCallPtr->getName(), "func");
    ASSERT_FALSE(rightFunctionCallPtr->empty());
    ASSERT_EQ(rightFunctionCallPtr->size(), 1);

    auto iter = rightFunctionCallPtr->cbegin();
    ASSERT_NE(iter, rightFunctionCallPtr->cend());

    const auto* expressionNodePtr = iter->getExpression();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(litteralNodePtr->getValue().valueS32, 42);

    ++iter;
    ASSERT_EQ(iter, rightFunctionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithASingleCharArgAndAssignment)
{
    const std::string input = "a = 123.0 + func('c');";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), EnumNodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    ASSERT_EQ(rightFunctionCallPtr->getName(), "func");
    ASSERT_FALSE(rightFunctionCallPtr->empty());
    ASSERT_EQ(rightFunctionCallPtr->size(), 1);

    auto iter = rightFunctionCallPtr->cbegin();
    ASSERT_NE(iter, rightFunctionCallPtr->cend());

    const auto* expressionNodePtr = iter->getExpression();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::CHAR);
    ASSERT_EQ(litteralNodePtr->getValue().valueChar, 'c');

    ++iter;
    ASSERT_EQ(iter, rightFunctionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithASingleDoubleArgAndAssignment)
{
    const std::string input = "a = 123.0 + func(2.5);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), EnumNodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    ASSERT_EQ(rightFunctionCallPtr->getName(), "func");
    ASSERT_FALSE(rightFunctionCallPtr->empty());
    ASSERT_EQ(rightFunctionCallPtr->size(), 1);

    auto iter = rightFunctionCallPtr->cbegin();
    ASSERT_NE(iter, rightFunctionCallPtr->cend());

    const auto* expressionNodePtr = iter->getExpression();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::DOUBLE);
    ASSERT_EQ(litteralNodePtr->getValue().valueF64, 2.5);

    ++iter;
    ASSERT_EQ(iter, rightFunctionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithASingleBoolArgAndAssignment)
{
    const std::string input = "a = 123.0 + func(true);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), EnumNodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    ASSERT_EQ(rightFunctionCallPtr->getName(), "func");
    ASSERT_FALSE(rightFunctionCallPtr->empty());
    ASSERT_EQ(rightFunctionCallPtr->size(), 1);

    auto iter = rightFunctionCallPtr->cbegin();
    ASSERT_NE(iter, rightFunctionCallPtr->cend());

    const auto* expressionNodePtr = iter->getExpression();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::BOOL);
    ASSERT_EQ(litteralNodePtr->getValue().valueBool, true);

    ++iter;
    ASSERT_EQ(iter, rightFunctionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithASingleNullArgAndAssignment)
{
    const std::string input = "a = 123.0 + func(NULL);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), EnumNodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    ASSERT_EQ(rightFunctionCallPtr->getName(), "func");
    ASSERT_FALSE(rightFunctionCallPtr->empty());
    ASSERT_EQ(rightFunctionCallPtr->size(), 1);

    auto iter = rightFunctionCallPtr->cbegin();
    ASSERT_NE(iter, rightFunctionCallPtr->cend());

    const auto* expressionNodePtr = iter->getExpression();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::NULL_T);
    ASSERT_EQ(litteralNodePtr->getValue().valueVoidPtr, nullptr);

    ++iter;
    ASSERT_EQ(iter, rightFunctionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithASingleStringArgAndAssignment)
{
    const std::string input = "a = 123.0 + func(\"Hello, world!\");";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), EnumNodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightSumPtr->getRight());
    ASSERT_EQ(rightFunctionCallPtr->getName(), "func");
    ASSERT_FALSE(rightFunctionCallPtr->empty());
    ASSERT_EQ(rightFunctionCallPtr->size(), 1);

    auto iter = rightFunctionCallPtr->cbegin();
    ASSERT_NE(iter, rightFunctionCallPtr->cend());

    const auto* expressionNodePtr = iter->getExpression();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_TRUE(litteralNodePtr->getDatatype().isString());
    ASSERT_EQ(std::string(litteralNodePtr->getValue().valueString), "Hello, world!");

    ++iter;
    ASSERT_EQ(iter, rightFunctionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeDoubleAssignAndSumViaFunctionCallWithNoArgsAndUnaryOpAssignment)
{
    const std::string input = "a = 123.0 + +func();";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignAndSumPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignAndSumPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignAndSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignAndSumPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignAndSumPtr->getRight()->getType(), EnumNodeType::BIN_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignAndSumPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightSumPtr = static_cast<BinOpNode*>(rootAssignAndSumPtr->getRight());
    ASSERT_EQ(rightSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
    ASSERT_NE(rightSumPtr->getLeft(), nullptr);
    ASSERT_EQ(rightSumPtr->getLeft()->getType(), EnumNodeType::LITTERAL);
    ASSERT_NE(rightSumPtr->getRight(), nullptr);
    ASSERT_EQ(rightSumPtr->getRight()->getType(), EnumNodeType::UNARY_OP);

    auto* leftIntPtr = static_cast<LitteralNode*>(rightSumPtr->getLeft());
    ASSERT_EQ(leftIntPtr->getDatatype().type, EnumCType::DOUBLE);
    ASSERT_EQ(leftIntPtr->getValue().valueF64, 123.0);

    auto* rightUnaryOpPtr = static_cast<UnaryOpNode*>(rightSumPtr->getRight());
    ASSERT_EQ(rightUnaryOpPtr->getOpType(), EnumUnaryOpType::POSITIVE);
    ASSERT_TRUE(rightUnaryOpPtr->hasExpression());
    ASSERT_EQ(rightUnaryOpPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* rightFunctionCallPtr = static_cast<FunctionCallNode*>(rightUnaryOpPtr->getExpression());
    ASSERT_EQ(rightFunctionCallPtr->getName(), "func");
    ASSERT_TRUE(rightFunctionCallPtr->empty());
    ASSERT_EQ(rightFunctionCallPtr->size(), 0);

    const auto iter = rightFunctionCallPtr->cbegin();
    ASSERT_EQ(iter, rightFunctionCallPtr->cend());
}

TEST(ParserTest, ParseCompilationNodeVarAssignmentViaNegativeVar)
{
    const std::string input = "a = -b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), EnumNodeType::UNARY_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightUnaryOpPtr = static_cast<UnaryOpNode*>(rootAssignPtr->getRight());
    ASSERT_EQ(rightUnaryOpPtr->getOpType(), EnumUnaryOpType::NEGATIVE);
    ASSERT_TRUE(rightUnaryOpPtr->hasExpression());
    ASSERT_EQ(rightUnaryOpPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* rightVariablePtr = static_cast<VariableNode*>(rightUnaryOpPtr->getExpression());
    ASSERT_EQ(rightVariablePtr->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeVarAssignmentViaNegativeDerferencedVariable)
{
    const std::string input = "a = -*b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), EnumNodeType::UNARY_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightUnaryOpPtr = static_cast<UnaryOpNode*>(rootAssignPtr->getRight());
    ASSERT_EQ(rightUnaryOpPtr->getOpType(), EnumUnaryOpType::NEGATIVE);
    ASSERT_TRUE(rightUnaryOpPtr->hasExpression());
    ASSERT_EQ(rightUnaryOpPtr->getExpression()->getType(), EnumNodeType::DEREF);

    auto* rightDerefPtr = static_cast<DerefNode*>(rightUnaryOpPtr->getExpression());
    ASSERT_TRUE(rightDerefPtr->hasExpression());
    ASSERT_EQ(rightDerefPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* rightVariablePtr = static_cast<VariableNode*>(rightDerefPtr->getExpression());
    ASSERT_EQ(rightVariablePtr->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeVarAssignmentViaDerferencedPositiveVariableError)
{
    const std::string input = "a = *+b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(ParserTest, ParseCompilationNodeVarAssignmentViaDerferencedNegativeVariableError)
{
    const std::string input = "a = *-b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(ParserTest, ParseCompilationNodeVarAssignmentViaAddressOf)
{
    const std::string input = "a = &b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), EnumNodeType::UNARY_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightAddressOfPtr = static_cast<UnaryOpNode*>(rootAssignPtr->getRight());
    ASSERT_TRUE(rightAddressOfPtr->hasExpression());
    ASSERT_EQ(rightAddressOfPtr->getOpType(), EnumUnaryOpType::ADDRESS_OF);
    ASSERT_EQ(rightAddressOfPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* rightAddressOfExpression = rightAddressOfPtr->getExpression();
    ASSERT_EQ(rightAddressOfExpression->getType(), EnumNodeType::VARIABLE);

    auto* rightVariablePtr = static_cast<VariableNode*>(rightAddressOfPtr->getExpression());
    ASSERT_EQ(rightVariablePtr->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeVarAssignmentViaAddressOfFuncCausesError)
{
    const std::string input = "a = &b();";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(ParserTest, ParseCompilationNodeVarAssignmentViaAddressOfIntError)
{
    const std::string input = "a = &42;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(ParserTest, ParseCompilationNodeVarDerefAssignmentViaAddressOfFuncCausesError)
{
    const std::string input = "**a = &b();";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);
    ASSERT_GT(reporter.getErrorCount(), 0);
}

TEST(ParserTest, ParseCompilationNodeVarAssignmentViaPointer)
{
    const std::string input = "a = *b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), EnumNodeType::DEREF);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightDerefPtr = static_cast<DerefNode*>(rootAssignPtr->getRight());
    ASSERT_NE(rightDerefPtr->getExpression(), nullptr);
    ASSERT_EQ(rightDerefPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* rightVariablePtr = static_cast<VariableNode*>(rightDerefPtr->getExpression());
    ASSERT_EQ(rightVariablePtr->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeVarPointerAssignmentViaAddressOf)
{
    const std::string input = "*a = &b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), EnumNodeType::DEREF);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), EnumNodeType::UNARY_OP);

    auto* leftDerefPtr = static_cast<DerefNode*>(rootAssignPtr->getLeft());
    ASSERT_NE(leftDerefPtr->getExpression(), nullptr);
    ASSERT_EQ(leftDerefPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* leftVariablePtr = static_cast<VariableNode*>(leftDerefPtr->getExpression());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightAddressOfPtr = static_cast<UnaryOpNode*>(rootAssignPtr->getRight());
    ASSERT_TRUE(rightAddressOfPtr->hasExpression());
    ASSERT_EQ(rightAddressOfPtr->getOpType(), EnumUnaryOpType::ADDRESS_OF);
    ASSERT_EQ(rightAddressOfPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* rightVariablePtr = static_cast<VariableNode*>(rightAddressOfPtr->getExpression());
    ASSERT_EQ(rightVariablePtr->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeVarPointerAssignmentViaPointer)
{
    const std::string input = "*a = *b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), EnumNodeType::DEREF);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), EnumNodeType::DEREF);

    auto* leftDerefPtr = static_cast<DerefNode*>(rootAssignPtr->getLeft());
    ASSERT_NE(leftDerefPtr->getExpression(), nullptr);
    ASSERT_EQ(leftDerefPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* leftVariablePtr = static_cast<VariableNode*>(leftDerefPtr->getExpression());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightDerefPtr = static_cast<DerefNode*>(rootAssignPtr->getRight());
    ASSERT_NE(rightDerefPtr->getExpression(), nullptr);
    ASSERT_EQ(rightDerefPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* rightVariablePtr = static_cast<VariableNode*>(rightDerefPtr->getExpression());
    ASSERT_EQ(rightVariablePtr->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeVarPointerAssignmentViaDoublePointer)
{
    const std::string input = "*a = **b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), EnumNodeType::DEREF);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), EnumNodeType::DEREF);

    auto* leftDerefPtr = static_cast<DerefNode*>(rootAssignPtr->getLeft());
    ASSERT_NE(leftDerefPtr->getExpression(), nullptr);
    ASSERT_EQ(leftDerefPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* leftVariablePtr = static_cast<VariableNode*>(leftDerefPtr->getExpression());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightDerefPtr = static_cast<DerefNode*>(rootAssignPtr->getRight());
    ASSERT_NE(rightDerefPtr->getExpression(), nullptr);
    ASSERT_EQ(rightDerefPtr->getExpression()->getType(), EnumNodeType::DEREF);

    auto* rightDerefPtr2 = static_cast<DerefNode*>(rightDerefPtr->getExpression());
    ASSERT_NE(rightDerefPtr2->getExpression(), nullptr);
    ASSERT_EQ(rightDerefPtr2->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* rightVariablePtr = static_cast<VariableNode*>(rightDerefPtr2->getExpression());
    ASSERT_EQ(rightVariablePtr->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeVarDoublePointerAssignmentViaDoublePointer)
{
    const std::string input = "**a = **b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), EnumNodeType::DEREF);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), EnumNodeType::DEREF);

    auto* leftDerefPtr = static_cast<DerefNode*>(rootAssignPtr->getLeft());
    ASSERT_NE(leftDerefPtr->getExpression(), nullptr);
    ASSERT_EQ(leftDerefPtr->getExpression()->getType(), EnumNodeType::DEREF);

    auto* leftDerefPtr2 = static_cast<DerefNode*>(leftDerefPtr->getExpression());
    ASSERT_NE(leftDerefPtr2->getExpression(), nullptr);
    ASSERT_EQ(leftDerefPtr2->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* leftVariablePtr = static_cast<VariableNode*>(leftDerefPtr2->getExpression());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightDerefPtr = static_cast<DerefNode*>(rootAssignPtr->getRight());
    ASSERT_NE(rightDerefPtr->getExpression(), nullptr);
    ASSERT_EQ(rightDerefPtr->getExpression()->getType(), EnumNodeType::DEREF);

    auto* rightDerefPtr2 = static_cast<DerefNode*>(rightDerefPtr->getExpression());
    ASSERT_NE(rightDerefPtr2->getExpression(), nullptr);
    ASSERT_EQ(rightDerefPtr2->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* rightVariablePtr = static_cast<VariableNode*>(rightDerefPtr2->getExpression());
    ASSERT_EQ(rightVariablePtr->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeVarAssignmentViaPrefixIncrementB)
{
    const std::string input = "a = ++b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), EnumNodeType::UNARY_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightUnaryOpPtr = static_cast<UnaryOpNode*>(rootAssignPtr->getRight());
    ASSERT_TRUE(rightUnaryOpPtr->hasExpression());
    ASSERT_EQ(rightUnaryOpPtr->getOpType(), EnumUnaryOpType::INCREMENT);
    ASSERT_TRUE(rightUnaryOpPtr->isPrefix());
    ASSERT_EQ(rightUnaryOpPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* rightVariablePtr = static_cast<VariableNode*>(rightUnaryOpPtr->getExpression());
    ASSERT_EQ(rightVariablePtr->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeVarAssignmentViaPrefixDecrementB)
{
    const std::string input = "a = --b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), EnumNodeType::UNARY_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightUnaryOpPtr = static_cast<UnaryOpNode*>(rootAssignPtr->getRight());
    ASSERT_TRUE(rightUnaryOpPtr->hasExpression());
    ASSERT_EQ(rightUnaryOpPtr->getOpType(), EnumUnaryOpType::DECREMENT);
    ASSERT_TRUE(rightUnaryOpPtr->isPrefix());
    ASSERT_EQ(rightUnaryOpPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* rightVariablePtr = static_cast<VariableNode*>(rightUnaryOpPtr->getExpression());
    ASSERT_EQ(rightVariablePtr->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeVarAssignmentViaPostfixIncrementB)
{
    const std::string input = "a = b++;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), EnumNodeType::UNARY_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightUnaryOpPtr = static_cast<UnaryOpNode*>(rootAssignPtr->getRight());
    ASSERT_TRUE(rightUnaryOpPtr->hasExpression());
    ASSERT_EQ(rightUnaryOpPtr->getOpType(), EnumUnaryOpType::INCREMENT);
    ASSERT_TRUE(rightUnaryOpPtr->isPostfix());
    ASSERT_EQ(rightUnaryOpPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* rightVariablePtr = static_cast<VariableNode*>(rightUnaryOpPtr->getExpression());
    ASSERT_EQ(rightVariablePtr->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeVarAssignmentViaPostfixDecrementB)
{
    const std::string input = "a = --b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::BIN_OP);

    auto* rootAssignPtr = static_cast<BinOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(rootAssignPtr->getTypeof(), EnumBinOpNodeType::ASSIGNMENT);
    ASSERT_NE(rootAssignPtr->getLeft(), nullptr);
    ASSERT_EQ(rootAssignPtr->getLeft()->getType(), EnumNodeType::VARIABLE);
    ASSERT_NE(rootAssignPtr->getRight(), nullptr);
    ASSERT_EQ(rootAssignPtr->getRight()->getType(), EnumNodeType::UNARY_OP);

    auto* leftVariablePtr = static_cast<VariableNode*>(rootAssignPtr->getLeft());
    ASSERT_EQ(leftVariablePtr->getName(), "a");

    auto* rightUnaryOpPtr = static_cast<UnaryOpNode*>(rootAssignPtr->getRight());
    ASSERT_TRUE(rightUnaryOpPtr->hasExpression());
    ASSERT_EQ(rightUnaryOpPtr->getOpType(), EnumUnaryOpType::DECREMENT);
    ASSERT_EQ(rightUnaryOpPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* rightVariablePtr = static_cast<VariableNode*>(rightUnaryOpPtr->getExpression());
    ASSERT_EQ(rightVariablePtr->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeDerefThenPrefixIncrementB)
{
    const std::string input = "*++b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::DEREF);

    auto* derefPtr = static_cast<DerefNode*>(expressionStatement->getExpression());
    ASSERT_TRUE(derefPtr->hasExpression());
    ASSERT_NE(derefPtr->getExpression(), nullptr);
    ASSERT_EQ(derefPtr->getExpression()->getType(), EnumNodeType::UNARY_OP);

    auto* unaryOpPtr = static_cast<UnaryOpNode*>(derefPtr->getExpression());
    ASSERT_EQ(unaryOpPtr->getOpType(), EnumUnaryOpType::INCREMENT);
    ASSERT_TRUE(unaryOpPtr->isPrefix());
    ASSERT_TRUE(unaryOpPtr->hasExpression());
    ASSERT_EQ(unaryOpPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* variablePtr = static_cast<VariableNode*>(unaryOpPtr->getExpression());
    ASSERT_EQ(variablePtr->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodePrefixIncrementThenDerefB)
{
    const std::string input = "++*b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::UNARY_OP);

    auto* unaryOpPtr = static_cast<UnaryOpNode*>(expressionStatement->getExpression());
    ASSERT_EQ(unaryOpPtr->getOpType(), EnumUnaryOpType::INCREMENT);
    ASSERT_TRUE(unaryOpPtr->isPrefix());
    ASSERT_TRUE(unaryOpPtr->hasExpression());
    ASSERT_EQ(unaryOpPtr->getExpression()->getType(), EnumNodeType::DEREF);

    auto* derefPtr = static_cast<DerefNode*>(unaryOpPtr->getExpression());
    ASSERT_TRUE(derefPtr->hasExpression());
    ASSERT_NE(derefPtr->getExpression(), nullptr);
    ASSERT_EQ(derefPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* variablePtr = static_cast<VariableNode*>(derefPtr->getExpression());
    ASSERT_EQ(variablePtr->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeDerefThenPostfixIncrementB)
{
    const std::string input = "*b++;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatement->getExpression(), nullptr);
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::DEREF);

    auto* derefPtr = static_cast<DerefNode*>(expressionStatement->getExpression());
    ASSERT_TRUE(derefPtr->hasExpression());
    ASSERT_NE(derefPtr->getExpression(), nullptr);
    ASSERT_EQ(derefPtr->getExpression()->getType(), EnumNodeType::UNARY_OP);

    auto* unaryOpPtr = static_cast<UnaryOpNode*>(derefPtr->getExpression());
    ASSERT_EQ(unaryOpPtr->getOpType(), EnumUnaryOpType::INCREMENT);
    ASSERT_TRUE(unaryOpPtr->isPostfix());
    ASSERT_TRUE(unaryOpPtr->hasExpression());
    ASSERT_EQ(unaryOpPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

    auto* variablePtr = static_cast<VariableNode*>(unaryOpPtr->getExpression());
    ASSERT_EQ(variablePtr->getName(), "b");
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::VARIABLE_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<VariableDeclarationStatementNode*>(firstStatement.get());
    const auto& datatype = rootDeclarationStatementPtr->getDatatype();
    ASSERT_EQ(datatype.type, EnumCType::INT32);
    ASSERT_EQ(datatype.pointers, 0);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, name);
}

TEST(ParserTest, ParseCompilationNodeIntPointerDeclarationStatement)
{
    const std::string input = "int* x;";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::VARIABLE_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<VariableDeclarationStatementNode*>(firstStatement.get());
    const auto& datatype = rootDeclarationStatementPtr->getDatatype();
    ASSERT_EQ(datatype.type, EnumCType::INT32);
    ASSERT_EQ(datatype.pointers, 1);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, name);
}

TEST(ParserTest, ParseCompilationNodeIntDoublePointerDeclarationStatement)
{
    const std::string input = "int** x;";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::VARIABLE_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<VariableDeclarationStatementNode*>(firstStatement.get());
    const auto& datatype = rootDeclarationStatementPtr->getDatatype();
    ASSERT_EQ(datatype.type, EnumCType::INT32);
    ASSERT_EQ(datatype.pointers, 2);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, name);
}

TEST(ParserTest, ParseCompilationNodeIntQuadPointerDeclarationStatement)
{
    const std::string input = "int* * * * x;";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::VARIABLE_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<VariableDeclarationStatementNode*>(firstStatement.get());
    const auto& datatype = rootDeclarationStatementPtr->getDatatype();
    ASSERT_EQ(datatype.type, EnumCType::INT32);
    ASSERT_EQ(datatype.pointers, 4);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, name);
}

TEST(ParserTest, ParseCompilationNodeStructForwardDeclarationStatement)
{
    const std::string input = "struct Vec2;";
    const std::string name = "Vec2";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::STRUCT_FWD_DECLARATION);

    auto* structFwdDeclPtr = static_cast<StructFwdDeclarationStatementNode*>(firstStatement.get());
    const auto& datatype = structFwdDeclPtr->getDatatype();
    ASSERT_EQ(datatype.type, EnumCType::STRUCT);
    ASSERT_EQ(datatype.pointers, 0);
    ASSERT_TRUE(datatype.optTypeName.has_value());
    ASSERT_EQ(datatype.optTypeName.value(), name);
}

TEST(ParserTest, ParseCompilationNodeStructDeclarationStatement)
{
    const std::string input = "struct Vec2 x;";
    const std::string structName = "Vec2";
    const std::string varName = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::VARIABLE_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<VariableDeclarationStatementNode*>(firstStatement.get());
    const auto& datatype = rootDeclarationStatementPtr->getDatatype();
    ASSERT_EQ(datatype.type, EnumCType::STRUCT);
    ASSERT_EQ(datatype.pointers, 0);
    ASSERT_TRUE(datatype.optTypeName.has_value());
    ASSERT_EQ(datatype.optTypeName.value(), structName);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, varName);
}

TEST(ParserTest, ParseCompilationNodeDoublePointerToStructDeclarationStatement)
{
    const std::string input = "struct Vec2** x;";
    const std::string structName = "Vec2";
    const std::string varName = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::VARIABLE_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<VariableDeclarationStatementNode*>(firstStatement.get());
    const auto& datatype = rootDeclarationStatementPtr->getDatatype();
    ASSERT_EQ(datatype.type, EnumCType::STRUCT);
    ASSERT_EQ(datatype.pointers, 2);
    ASSERT_TRUE(datatype.optTypeName.has_value());
    ASSERT_EQ(datatype.optTypeName.value(), structName);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, varName);
}

TEST(ParserTest, ParseCompilationNodeDoublePointerToStructDeclarationStatementError)
{
    const std::string input = "struct Vec2**;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);
}

TEST(ParserTest, ParseCompilationNodeStructEmptyDefinitionStatement)
{
    const std::string input = "struct A {};";
    const std::string name = "A";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::STRUCT_DEFINITION);

    auto* rootDeclarationStatementPtr = static_cast<StructDefinitionStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDeclarationStatementPtr->getName(), name);
    ASSERT_TRUE(rootDeclarationStatementPtr->empty());
    ASSERT_EQ(rootDeclarationStatementPtr->size(), 0);
}

TEST(ParserTest, ParseCompilationNodeStructDefinitionStatementWithSingleIntField)
{
    const std::string input = "struct A { int x; };";
    const std::string name = "A";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::STRUCT_DEFINITION);

    auto* rootDeclarationStatementPtr = static_cast<StructDefinitionStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDeclarationStatementPtr->getName(), name);
    ASSERT_FALSE(rootDeclarationStatementPtr->empty());
    ASSERT_EQ(rootDeclarationStatementPtr->size(), 1);
}

TEST(ParserTest, ParseCompilationNodeStructDefinitionStatementWithThreeDoubleFields)
{
    const std::string input = "struct Vec3 { double x; double y; double z; };";
    const std::string name = "Vec3";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::STRUCT_DEFINITION);

    auto* rootDeclarationStatementPtr = static_cast<StructDefinitionStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDeclarationStatementPtr->getName(), name);
    ASSERT_FALSE(rootDeclarationStatementPtr->empty());
    ASSERT_EQ(rootDeclarationStatementPtr->size(), 3);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<FunctionDeclarationStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype().type, EnumCType::INT32);

    const auto& outName = rootDeclarationStatementPtr->getName();
    ASSERT_EQ(outName, name);
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDeclarationStatementEmptyBlockWithStructDefinitionParmeterError)
{
    reporter.reset();

    const std::string input = "int func(struct A {}; a);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype().type, EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_TRUE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 0);

    const auto iter = blockNode.cbegin();
    ASSERT_EQ(iter, blockNode.cend());

    ASSERT_FALSE(rootDefinitionStatementPtr->hasParams());
    ASSERT_EQ(rootDefinitionStatementPtr->getReturnStatement(), nullptr);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype().type, EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, funcName);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_FALSE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 1);

    auto iter = blockNode.cbegin();
    ASSERT_NE(iter, blockNode.cend());

    const auto& declPtr = *iter;
    ASSERT_EQ(declPtr->getType(), EnumNodeType::VARIABLE_DECLARATION_STATEMENT);

    const auto* varDeclPtr = static_cast<VariableDeclarationStatementNode*>(iter->get());
    ASSERT_EQ(varDeclPtr->getDatatype().type, EnumCType::FLOAT);
    ASSERT_EQ(varDeclPtr->getName(), varName);

    ++iter;
    ASSERT_EQ(iter, blockNode.cend());

    ASSERT_FALSE(rootDefinitionStatementPtr->hasParams());
    ASSERT_EQ(rootDefinitionStatementPtr->getReturnStatement(), nullptr);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype().type, EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, funcName);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_FALSE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 2);

    auto iter = blockNode.cbegin();

    {
        ASSERT_NE(iter, blockNode.cend());

        const auto& declPtr = *iter;
        ASSERT_EQ(declPtr->getType(), EnumNodeType::VARIABLE_DECLARATION_STATEMENT);

        const auto* varDeclPtr = static_cast<VariableDeclarationStatementNode*>(iter->get());
        ASSERT_EQ(varDeclPtr->getDatatype().type, EnumCType::FLOAT);
        ASSERT_EQ(varDeclPtr->getName(), var1Name);
    }

    ++iter;
    ASSERT_NE(iter, blockNode.cend());

    {
        const auto& declPtr = *iter;
        ASSERT_EQ(declPtr->getType(), EnumNodeType::VARIABLE_DECLARATION_STATEMENT);

        const auto* varDeclPtr = static_cast<VariableDeclarationStatementNode*>(iter->get());
        ASSERT_EQ(varDeclPtr->getDatatype().type, EnumCType::CHAR);
        ASSERT_EQ(varDeclPtr->getName(), var2Name);
    }

    ++iter;
    ASSERT_EQ(iter, blockNode.cend());

    ASSERT_FALSE(rootDefinitionStatementPtr->hasParams());
    ASSERT_EQ(rootDefinitionStatementPtr->getReturnStatement(), nullptr);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<FunctionDeclarationStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype().type, EnumCType::INT32);

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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<FunctionDeclarationStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype().type, EnumCType::INT32);

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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<FunctionDeclarationStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype().type, EnumCType::INT32);

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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<FunctionDeclarationStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype().type, EnumCType::INT32);

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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<FunctionDeclarationStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype().type, EnumCType::INT32);

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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DECLARATION_STATEMENT);

    auto* rootDeclarationStatementPtr = static_cast<FunctionDeclarationStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDeclarationStatementPtr->getDatatype().type, EnumCType::INT32);

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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype().type, EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_TRUE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 0);

    const auto iter = blockNode.cbegin();
    ASSERT_EQ(iter, blockNode.cend());

    ASSERT_TRUE(rootDefinitionStatementPtr->hasParams());
    ASSERT_EQ(rootDefinitionStatementPtr->paramCount(), 1);
    ASSERT_EQ(rootDefinitionStatementPtr->getReturnStatement(), nullptr);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype().type, EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_TRUE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 0);

    const auto iter = blockNode.cbegin();
    ASSERT_EQ(iter, blockNode.cend());

    ASSERT_TRUE(rootDefinitionStatementPtr->hasParams());
    ASSERT_EQ(rootDefinitionStatementPtr->paramCount(), 1);
    ASSERT_EQ(rootDefinitionStatementPtr->getReturnStatement(), nullptr);
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDefinitionStatementEmptyBlockWithSingleParamAndReturnStatement)
{
    const std::string input = "int x(int y) { return 42; }";
    const std::string name = "x";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype().type, EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_FALSE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 1);

    auto iter = blockNode.cbegin();
    ASSERT_NE(iter, blockNode.cend());

    const auto& statementPtr = *iter;
    ASSERT_EQ(statementPtr->getType(), EnumNodeType::RETURN_STATEMENT);

    const auto* returnStatementPtr = static_cast<const ReturnStatementNode*>(statementPtr.get());
    ASSERT_TRUE(returnStatementPtr->hasExpression());
    ASSERT_EQ(returnStatementPtr->getDatatype()->type, EnumCType::INT32);

    const auto* expressionPtr = returnStatementPtr->getExpression();
    ASSERT_NE(expressionPtr, nullptr);
    ASSERT_EQ(expressionPtr->getType(), EnumNodeType::LITTERAL);

    const auto* intLitteralPtr = static_cast<const LitteralNode*>(expressionPtr);
    ASSERT_EQ(intLitteralPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(intLitteralPtr->getValue().valueS32, 42);

    ++iter;
    ASSERT_EQ(iter, blockNode.cend());

    ASSERT_TRUE(rootDefinitionStatementPtr->hasParams());
    ASSERT_EQ(rootDefinitionStatementPtr->paramCount(), 1);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype().type, EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_TRUE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 0);

    const auto iter = blockNode.cbegin();
    ASSERT_EQ(iter, blockNode.cend());

    ASSERT_TRUE(rootDefinitionStatementPtr->hasParams());
    ASSERT_EQ(rootDefinitionStatementPtr->paramCount(), 2);
    ASSERT_EQ(rootDefinitionStatementPtr->getReturnStatement(), nullptr);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype().type, EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_TRUE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 0);

    const auto iter = blockNode.cbegin();
    ASSERT_EQ(iter, blockNode.cend());

    ASSERT_TRUE(rootDefinitionStatementPtr->hasParams());
    ASSERT_EQ(rootDefinitionStatementPtr->paramCount(), 2);
    ASSERT_EQ(rootDefinitionStatementPtr->getReturnStatement(), nullptr);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype().type, EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_TRUE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 0);

    const auto iter = blockNode.cbegin();
    ASSERT_EQ(iter, blockNode.cend());

    ASSERT_TRUE(rootDefinitionStatementPtr->hasParams());
    ASSERT_EQ(rootDefinitionStatementPtr->paramCount(), 3);
    ASSERT_EQ(rootDefinitionStatementPtr->getReturnStatement(), nullptr);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::FUNCTION_DEFINITION_STATEMENT);

    auto* rootDefinitionStatementPtr = static_cast<FunctionDefinitionStatementNode*>(firstStatement.get());
    ASSERT_EQ(rootDefinitionStatementPtr->getDatatype().type, EnumCType::INT32);

    const auto& outName = rootDefinitionStatementPtr->getName();
    ASSERT_EQ(outName, name);

    const auto& blockNode = rootDefinitionStatementPtr->getBlock();
    ASSERT_TRUE(blockNode.empty());
    ASSERT_EQ(blockNode.size(), 0);

    const auto iter = blockNode.cbegin();
    ASSERT_EQ(iter, blockNode.cend());

    ASSERT_TRUE(rootDefinitionStatementPtr->hasParams());
    ASSERT_EQ(rootDefinitionStatementPtr->paramCount(), 3);
    ASSERT_EQ(rootDefinitionStatementPtr->getReturnStatement(), nullptr);
}

TEST(ParserTest, ParseCompilationNodeIntFunctionDefinitionStatementEmptyBlockWithStructDefinitionParmeterError)
{
    reporter.reset();

    const std::string input = "int func(struct A {}; a) {}";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);
}

TEST(ParserTest, ParseCompilationNodeFunctionCallStatementNoArgs)
{
    const std::string input = "func();";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    const auto* expressionNodePtr = argListIter->getExpression();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::VARIABLE);

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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    const auto* expressionNodePtr = argListIter->getExpression();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::BOOL);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    const auto* expressionNodePtr = argListIter->getExpression();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::CHAR);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    const auto* expressionNodePtr = argListIter->getExpression();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::DOUBLE);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    const auto* expressionNodePtr = argListIter->getExpression();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::INT32);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    const auto* expressionNodePtr = argListIter->getExpression();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::NULL_T);

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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    const auto* expressionNodePtr = argListIter->getExpression();
    ASSERT_NE(expressionNodePtr, nullptr);
    ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

    const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
    ASSERT_TRUE(litteralNodePtr->getDatatype().isString());
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getExpression();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::VARIABLE);

        const auto* variableNodePtr = static_cast<const VariableNode*>(expressionNodePtr);
        ASSERT_EQ(variableNodePtr->getName(), "x");
    }

    ++argListIter;
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getExpression();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::VARIABLE);

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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getExpression();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::BOOL);
        ASSERT_EQ(litteralNodePtr->getValue().valueBool, false);
    }

    ++argListIter;
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getExpression();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::BOOL);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getExpression();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::CHAR);
        ASSERT_EQ(litteralNodePtr->getValue().valueChar, 'A');
    }

    ++argListIter;
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getExpression();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::CHAR);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    CMM_CONSTEXPR f64 eps = 0.00001;

    {
        const auto* expressionNodePtr = argListIter->getExpression();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::DOUBLE);
        ASSERT_NEAR(litteralNodePtr->getValue().valueF64, 123.45, eps);
    }

    ++argListIter;
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getExpression();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::DOUBLE);
        ASSERT_NEAR(litteralNodePtr->getValue().valueF64, -100.001, eps);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getExpression();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::INT32);
        ASSERT_EQ(litteralNodePtr->getValue().valueS32, 42);
    }

    ++argListIter;
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getExpression();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::INT32);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getExpression();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::NULL_T);
    }

    ++argListIter;
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getExpression();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_EQ(litteralNodePtr->getDatatype().type, EnumCType::NULL_T);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::FUNCTION_CALL);

    auto* functionCallPtr = static_cast<FunctionCallNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(functionCallPtr, nullptr);

    auto argListIter = functionCallPtr->cbegin();
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getExpression();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_TRUE(litteralNodePtr->getDatatype().isString());
        ASSERT_EQ(strncmp(litteralNodePtr->getValue().valueString, "Hello, world!", 16), 0);
    }

    ++argListIter;
    ASSERT_NE(argListIter, functionCallPtr->cend());

    {
        const auto* expressionNodePtr = argListIter->getExpression();
        ASSERT_NE(expressionNodePtr, nullptr);
        ASSERT_EQ(expressionNodePtr->getType(), EnumNodeType::LITTERAL);

        const auto* litteralNodePtr = static_cast<const LitteralNode*>(expressionNodePtr);
        ASSERT_TRUE(litteralNodePtr->getDatatype().isString());
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::PAREN_EXPRESSION);

    auto* parenExpressionPtr = static_cast<ParenExpressionNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(parenExpressionPtr->getExpression(), nullptr);
    ASSERT_EQ(parenExpressionPtr->getExpression()->getType(), EnumNodeType::LITTERAL);

    auto* intPtr = static_cast<LitteralNode*>(parenExpressionPtr->getExpression());
    ASSERT_EQ(intPtr->getDatatype().type, EnumCType::INT32);
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

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* rootExpressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(rootExpressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(rootExpressionStatementPtr->getExpression()->getType(), EnumNodeType::PAREN_EXPRESSION);

    auto* parenExpressionPtr = static_cast<ParenExpressionNode*>(rootExpressionStatementPtr->getExpression());
    ASSERT_NE(parenExpressionPtr->getExpression(), nullptr);
    ASSERT_EQ(parenExpressionPtr->getExpression()->getType(), EnumNodeType::PAREN_EXPRESSION);

    parenExpressionPtr = static_cast<ParenExpressionNode*>(parenExpressionPtr->getExpression());
    ASSERT_NE(parenExpressionPtr->getExpression(), nullptr);
    ASSERT_EQ(parenExpressionPtr->getExpression()->getType(), EnumNodeType::VARIABLE);

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

TEST(ParserTest, ParseCompilationNodeWithCastNode)
{
    const std::string input = "(int) b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    const auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_TRUE(expressionStatement->hasExpression());
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::CAST);

    const auto* castNode = static_cast<const CastNode*>(expressionStatement->getExpression());
    ASSERT_TRUE(castNode->hasExpression());
    ASSERT_NE(castNode->getExpression(), nullptr);
    ASSERT_EQ(castNode->getExpression()->getType(), EnumNodeType::VARIABLE);
    ASSERT_EQ(castNode->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(castNode->getDatatype().pointers, 0);

    const auto* variableNode = static_cast<const VariableNode*>(castNode->getExpression());
    ASSERT_EQ(variableNode->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeWithDoubleCastNode)
{
    const std::string input = "(double) (int) b;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    const auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_TRUE(expressionStatement->hasExpression());
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::CAST);

    const auto* castNode = static_cast<const CastNode*>(expressionStatement->getExpression());
    ASSERT_TRUE(castNode->hasExpression());
    ASSERT_NE(castNode->getExpression(), nullptr);
    ASSERT_EQ(castNode->getExpression()->getType(), EnumNodeType::CAST);
    ASSERT_EQ(castNode->getDatatype().type, EnumCType::DOUBLE);
    ASSERT_EQ(castNode->getDatatype().pointers, 0);

    const auto* secondCastNode = static_cast<const CastNode*>(castNode->getExpression());
    ASSERT_EQ(secondCastNode->getExpression()->getType(), EnumNodeType::VARIABLE);
    ASSERT_EQ(secondCastNode->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(secondCastNode->getDatatype().pointers, 0);

    const auto* variableNode = static_cast<const VariableNode*>(secondCastNode->getExpression());
    ASSERT_EQ(variableNode->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeWithCastNodeWrapperInParenNode)
{
    const std::string input = "((int) b);";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    const auto* expressionStatement = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_TRUE(expressionStatement->hasExpression());
    ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::PAREN_EXPRESSION);

    const auto* parenExpr = static_cast<const ParenExpressionNode*>(expressionStatement->getExpression());
    ASSERT_TRUE(parenExpr->hasExpression());
    ASSERT_EQ(parenExpr->getExpression()->getType(), EnumNodeType::CAST);

    const auto* castNode = static_cast<const CastNode*>(parenExpr->getExpression());
    ASSERT_TRUE(castNode->hasExpression());
    ASSERT_NE(castNode->getExpression(), nullptr);
    ASSERT_EQ(castNode->getExpression()->getType(), EnumNodeType::VARIABLE);
    ASSERT_EQ(castNode->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(castNode->getDatatype().pointers, 0);

    const auto* variableNode = static_cast<const VariableNode*>(castNode->getExpression());
    ASSERT_EQ(variableNode->getName(), "b");
}

TEST(ParserTest, ParseCompilationNodeReturnStatementWithNoExpression)
{
    const std::string input = "return;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::RETURN_STATEMENT);

    const auto* returnStatementPtr = static_cast<ReturnStatementNode*>(firstStatement.get());
    ASSERT_FALSE(returnStatementPtr->hasExpression());
    ASSERT_EQ(returnStatementPtr->getDatatype(), nullptr);

    const auto* expression = returnStatementPtr->getExpression();
    ASSERT_EQ(expression, nullptr);
}

TEST(ParserTest, ParseCompilationNodeReturnStatementWithIntExpression)
{
    const std::string input = "return 42;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::RETURN_STATEMENT);

    auto* returnStatementPtr = static_cast<ReturnStatementNode*>(firstStatement.get());
    ASSERT_TRUE(returnStatementPtr->hasExpression());
    ASSERT_EQ(returnStatementPtr->getDatatype()->type, EnumCType::INT32);

    const auto* expression = returnStatementPtr->getExpression();
    ASSERT_NE(expression, nullptr);
    ASSERT_EQ(expression->getType(), EnumNodeType::LITTERAL);

    const auto* intLitteralPtr = static_cast<const LitteralNode*>(expression);
    ASSERT_EQ(intLitteralPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(intLitteralPtr->getValue().valueS32, 42);
}

TEST(ParserTest, ParseCompilationNodeReturnStatementWithBoolExpression)
{
    const std::string input = "return true;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::RETURN_STATEMENT);

    auto* returnStatementPtr = static_cast<ReturnStatementNode*>(firstStatement.get());
    ASSERT_TRUE(returnStatementPtr->hasExpression());
    ASSERT_EQ(returnStatementPtr->getDatatype()->type, EnumCType::BOOL);

    const auto* expression = returnStatementPtr->getExpression();
    ASSERT_NE(expression, nullptr);
    ASSERT_EQ(expression->getType(), EnumNodeType::LITTERAL);

    const auto* boolLitteralPtr = static_cast<const LitteralNode*>(expression);
    ASSERT_EQ(boolLitteralPtr->getDatatype().type, EnumCType::BOOL);
    ASSERT_EQ(boolLitteralPtr->getValue().valueBool, true);
}

TEST(ParserTest, ParseCompilationNodeIfElseStatementWithEmptyBlockNodeAndNoElse)
{
    const std::string input = "if (true) {}";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::IF_ELSE_STATEMENT);

    auto* ifElseStatementPtr = static_cast<IfElseStatementNode*>(firstStatement.get());
    const auto* ifConditonalExpression = ifElseStatementPtr->getIfConditional();
    ASSERT_NE(ifConditonalExpression , nullptr);
    ASSERT_EQ(ifConditonalExpression ->getType(), EnumNodeType::LITTERAL);

    const auto* boolLitteralPtr = static_cast<const LitteralNode*>(ifConditonalExpression);
    ASSERT_EQ(boolLitteralPtr->getDatatype().type, EnumCType::BOOL);
    ASSERT_EQ(boolLitteralPtr->getValue().valueBool, true);

    ASSERT_FALSE(ifElseStatementPtr->hasElseStatement());
}

TEST(ParserTest, ParseCompilationNodeIfElseStatementWithBlockNodeAndElse)
{
    const std::string input = "if (1) { x = 42; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::IF_ELSE_STATEMENT);

    auto* ifElseStatementPtr = static_cast<IfElseStatementNode*>(firstStatement.get());
    const auto* ifConditonalExpression = ifElseStatementPtr->getIfConditional();
    ASSERT_NE(ifConditonalExpression , nullptr);
    ASSERT_EQ(ifConditonalExpression ->getType(), EnumNodeType::LITTERAL);

    const auto* intLitteralPtr = static_cast<const LitteralNode*>(ifConditonalExpression);
    ASSERT_EQ(intLitteralPtr->getDatatype().type, EnumCType::INT32);
    ASSERT_EQ(intLitteralPtr->getValue().valueS32, 1);

    ASSERT_FALSE(ifElseStatementPtr->hasElseStatement());
}

TEST(ParserTest, ParseCompilationNodeIfElseStatementWithEmptyBlockNodeAndElseWithEmptyBlockNode)
{
    const std::string input = "if (true) {} else {}";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::IF_ELSE_STATEMENT);

    auto* ifElseStatementPtr = static_cast<IfElseStatementNode*>(firstStatement.get());
    const auto* ifConditonalExpression = ifElseStatementPtr->getIfConditional();
    ASSERT_NE(ifConditonalExpression , nullptr);
    ASSERT_EQ(ifConditonalExpression ->getType(), EnumNodeType::LITTERAL);

    const auto* boolLitteralPtr = static_cast<const LitteralNode*>(ifConditonalExpression);
    ASSERT_EQ(boolLitteralPtr->getDatatype().type, EnumCType::BOOL);
    ASSERT_EQ(boolLitteralPtr->getValue().valueBool, true);

    ASSERT_TRUE(ifElseStatementPtr->hasElseStatement());
}

TEST(ParserTest, ParseCompilationNodeIfElseStatementWithBlockNodeAndElseWithBlockNode)
{
    const std::string input = "if ('c') { x = 42; } else { x = -123; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::IF_ELSE_STATEMENT);

    auto* ifElseStatementPtr = static_cast<IfElseStatementNode*>(firstStatement.get());
    const auto* ifConditonalExpression = ifElseStatementPtr->getIfConditional();
    ASSERT_NE(ifConditonalExpression, nullptr);
    ASSERT_EQ(ifConditonalExpression->getType(), EnumNodeType::LITTERAL);

    const auto* charLitteralPtr = static_cast<const LitteralNode*>(ifConditonalExpression);
    ASSERT_EQ(charLitteralPtr->getDatatype().type, EnumCType::CHAR);
    ASSERT_EQ(charLitteralPtr->getValue().valueChar, 'c');

    ASSERT_TRUE(ifElseStatementPtr->hasElseStatement());
}

TEST(ParserTest, ParseCompilationNodeWhileStatementWithEmptyBlockNode)
{
    const std::string input = "while (true) {}";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::WHILE_STATEMENT);

    auto* whileStatementPtr = static_cast<WhileStatementNode*>(firstStatement.get());
    auto* conditionalPtr = whileStatementPtr->getConditional();
    ASSERT_NE(conditionalPtr, nullptr);
    ASSERT_EQ(conditionalPtr->getType(), EnumNodeType::LITTERAL);

    auto* boolPtr = static_cast<LitteralNode*>(conditionalPtr);
    ASSERT_EQ(boolPtr->getDatatype().type, EnumCType::BOOL);
    ASSERT_TRUE(boolPtr->getValue().valueBool);

    auto* statementPtr = whileStatementPtr->getStatement();
    ASSERT_NE(statementPtr, nullptr);
    ASSERT_EQ(statementPtr->getType(), EnumNodeType::BLOCK);

    auto* blockNodePtr = static_cast<BlockNode*>(statementPtr);
    ASSERT_TRUE(blockNodePtr->empty());
    ASSERT_EQ(blockNodePtr->size(), 0);
    ASSERT_EQ(blockNodePtr->cbegin(), blockNodePtr->cend());
}

TEST(ParserTest, ParseCompilationNodeWhileStatementWithBlockNode)
{
    const std::string input = "while (true) { i = i * 2; }";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::WHILE_STATEMENT);

    auto* whileStatementPtr = static_cast<WhileStatementNode*>(firstStatement.get());
    auto* conditionalPtr = whileStatementPtr->getConditional();
    ASSERT_NE(conditionalPtr, nullptr);
    ASSERT_EQ(conditionalPtr->getType(), EnumNodeType::LITTERAL);

    auto* boolPtr = static_cast<LitteralNode*>(conditionalPtr);
    ASSERT_EQ(boolPtr->getDatatype().type, EnumCType::BOOL);
    ASSERT_TRUE(boolPtr->getValue().valueBool);

    auto* statementPtr = whileStatementPtr->getStatement();
    ASSERT_NE(statementPtr, nullptr);
    ASSERT_EQ(statementPtr->getType(), EnumNodeType::BLOCK);

    auto* blockNodePtr = static_cast<BlockNode*>(statementPtr);
    ASSERT_FALSE(blockNodePtr->empty());
    ASSERT_EQ(blockNodePtr->size(), 1);

    auto iter = blockNodePtr->cbegin();
    ASSERT_NE(iter, blockNodePtr->cend());

    ++iter;
    ASSERT_EQ(iter, blockNodePtr->cend());
}

TEST(ParserTest, ParseCompilationNodeMultipleStatements)
{
    const std::string input = "true; false;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto statementIter = translationUnit.begin();
    ASSERT_NE(statementIter, translationUnit.end());
    ASSERT_EQ((*statementIter)->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    {
        auto* expressionStatement  = static_cast<ExpressionStatementNode*>(statementIter->get());
        ASSERT_NE(expressionStatement->getExpression(), nullptr);
        ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::LITTERAL);

        auto* boolPtr = static_cast<LitteralNode*>(expressionStatement->getExpression());
        ASSERT_EQ(boolPtr->getDatatype().type, EnumCType::BOOL);
        ASSERT_TRUE(boolPtr->getValue().valueBool);
    }

    ++statementIter;
    ASSERT_NE(statementIter, translationUnit.end());

    {
        auto* expressionStatement  = static_cast<ExpressionStatementNode*>(statementIter->get());
        ASSERT_NE(expressionStatement->getExpression(), nullptr);
        ASSERT_EQ(expressionStatement->getExpression()->getType(), EnumNodeType::LITTERAL);

        auto* boolPtr = static_cast<LitteralNode*>(expressionStatement->getExpression());
        ASSERT_EQ(boolPtr->getDatatype().type, EnumCType::BOOL);
        ASSERT_FALSE(boolPtr->getValue().valueBool);
    }

    ++statementIter;
    ASSERT_EQ(statementIter, translationUnit.end());
}

TEST(ParserTest, ParseCompilationNodeFieldAccessNodeViaDot)
{
    const std::string input = "x.y;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatementPtr, nullptr);
    ASSERT_NE(expressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(expressionStatementPtr->getExpression()->getType(), EnumNodeType::FIELD_ACCESS);

    auto* fieldAccessNodePtr = static_cast<FieldAccessNode*>(expressionStatementPtr->getExpression());
    ASSERT_EQ(fieldAccessNodePtr->getFieldAccessType(), EnumFieldAccessType::DOT);
    ASSERT_NE(fieldAccessNodePtr->getExpression(), nullptr);
    ASSERT_EQ(fieldAccessNodePtr->getExpression()->getType(), EnumNodeType::VARIABLE);
    ASSERT_EQ(fieldAccessNodePtr->getName(), "y");

    auto* variableNodePtr = static_cast<VariableNode*>(fieldAccessNodePtr->getExpression());
    ASSERT_EQ(variableNodePtr->getName(), "x");
}

TEST(ParserTest, ParseCompilationNodeFieldAccessNodeViaArrow)
{
    const std::string input = "x->y;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatementPtr, nullptr);
    ASSERT_NE(expressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(expressionStatementPtr->getExpression()->getType(), EnumNodeType::FIELD_ACCESS);

    auto* fieldAccessNodePtr = static_cast<FieldAccessNode*>(expressionStatementPtr->getExpression());
    ASSERT_EQ(fieldAccessNodePtr->getFieldAccessType(), EnumFieldAccessType::ARROW);
    ASSERT_NE(fieldAccessNodePtr->getExpression(), nullptr);
    ASSERT_EQ(fieldAccessNodePtr->getExpression()->getType(), EnumNodeType::VARIABLE);
    ASSERT_EQ(fieldAccessNodePtr->getName(), "y");

    auto* variableNodePtr = static_cast<VariableNode*>(fieldAccessNodePtr->getExpression());
    ASSERT_EQ(variableNodePtr->getName(), "x");
}

TEST(ParserTest, ParseCompilationNodeFieldAccessNodeViaArrowThenDot)
{
    const std::string input = "x->y.z;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::EXPRESSION_STATEMENT);

    auto* expressionStatementPtr = static_cast<ExpressionStatementNode*>(firstStatement.get());
    ASSERT_NE(expressionStatementPtr, nullptr);
    ASSERT_NE(expressionStatementPtr->getExpression(), nullptr);
    ASSERT_EQ(expressionStatementPtr->getExpression()->getType(), EnumNodeType::FIELD_ACCESS);

    auto* fieldAccessNodePtr = static_cast<FieldAccessNode*>(expressionStatementPtr->getExpression());
    ASSERT_EQ(fieldAccessNodePtr->getFieldAccessType(), EnumFieldAccessType::DOT);
    ASSERT_NE(fieldAccessNodePtr->getExpression(), nullptr);
    ASSERT_EQ(fieldAccessNodePtr->getExpression()->getType(), EnumNodeType::FIELD_ACCESS);
    ASSERT_EQ(fieldAccessNodePtr->getName(), "z");

    auto* fieldAccessNodePtr2 = static_cast<FieldAccessNode*>(fieldAccessNodePtr->getExpression());
    ASSERT_EQ(fieldAccessNodePtr2->getFieldAccessType(), EnumFieldAccessType::ARROW);
    ASSERT_NE(fieldAccessNodePtr2->getExpression(), nullptr);
    ASSERT_EQ(fieldAccessNodePtr2->getExpression()->getType(), EnumNodeType::VARIABLE);
    ASSERT_EQ(fieldAccessNodePtr2->getName(), "y");

    auto* variableNodePtr = static_cast<VariableNode*>(fieldAccessNodePtr2->getExpression());
    ASSERT_EQ(variableNodePtr->getName(), "x");
}

TEST(ParserTest, ParseCompilationNodeEnumDefinitionStatementNodeEmpty)
{
    const std::string input = "enum A {};";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::ENUM_DEFINITION);

    auto* enumDefStatePtr = static_cast<EnumDefinitionStatementNode*>(firstStatement.get());
    ASSERT_NE(enumDefStatePtr, nullptr);
    ASSERT_EQ(enumDefStatePtr->getName(), "A");
    ASSERT_TRUE(enumDefStatePtr->empty());
    ASSERT_EQ(enumDefStatePtr->size(), 0);
}

TEST(ParserTest, ParseCompilationNodeEnumDefinitionStatementNodeSingle)
{
    const std::string input = "enum A { X };";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::ENUM_DEFINITION);

    auto* enumDefStatePtr = static_cast<EnumDefinitionStatementNode*>(firstStatement.get());
    ASSERT_NE(enumDefStatePtr, nullptr);
    ASSERT_EQ(enumDefStatePtr->getName(), "A");
    ASSERT_FALSE(enumDefStatePtr->empty());
    ASSERT_EQ(enumDefStatePtr->size(), 1);

    const auto& enumeratorMap = enumDefStatePtr->getEnumData()->enumeratorMap;
    const auto endIter = enumeratorMap.cend();
    auto findResult = enumeratorMap.find("X");

    ASSERT_NE(findResult, endIter);
    ASSERT_EQ(findResult->second.getIndex(), 0);
}

TEST(ParserTest, ParseCompilationNodeEnumDefinitionStatementNodeMulti)
{
    const std::string input = "enum A { X, Y, Z, };";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::ENUM_DEFINITION);

    auto* enumDefStatePtr = static_cast<EnumDefinitionStatementNode*>(firstStatement.get());
    ASSERT_NE(enumDefStatePtr, nullptr);
    ASSERT_EQ(enumDefStatePtr->getName(), "A");
    ASSERT_FALSE(enumDefStatePtr->empty());
    ASSERT_EQ(enumDefStatePtr->size(), 3);

    const auto& enumeratorMap = enumDefStatePtr->getEnumData()->enumeratorMap;
    const auto endIter = enumeratorMap.cend();
    auto findResult = enumeratorMap.find("X");

    ASSERT_NE(findResult, endIter);
    ASSERT_EQ(findResult->second.getIndex(), 0);

    findResult = enumeratorMap.find("Y");
    ASSERT_NE(findResult, endIter);
    ASSERT_EQ(findResult->second.getIndex(), 1);

    findResult = enumeratorMap.find("Z");
    ASSERT_NE(findResult, endIter);
    ASSERT_EQ(findResult->second.getIndex(), 2);
}

TEST(ParserTest, ParseCompilationNodeEnumDefinitionStatementNodeDuplicateEnumeratorError)
{
    const std::string input = "enum A { X, Y, X };";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_FALSE(errorMessage.empty());
    ASSERT_EQ(compUnitPtr, nullptr);
}

TEST(ParserTest, ParseCompilationNodeEnumDefinitionStatementNodeWithAssigment)
{
    const std::string input = "enum A { X = 42 };";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::ENUM_DEFINITION);

    auto* enumDefStatePtr = static_cast<EnumDefinitionStatementNode*>(firstStatement.get());
    ASSERT_NE(enumDefStatePtr, nullptr);
    ASSERT_EQ(enumDefStatePtr->getName(), "A");
    ASSERT_FALSE(enumDefStatePtr->empty());
    ASSERT_EQ(enumDefStatePtr->size(), 1);

    const auto& enumeratorMap = enumDefStatePtr->getEnumData()->enumeratorMap;
    const auto endIter = enumeratorMap.cend();
    auto findResult = enumeratorMap.find("X");

    ASSERT_NE(findResult, endIter);
    ASSERT_EQ(findResult->second.getIndex(), 0);
    ASSERT_EQ(findResult->second.getValue(), 42);
}

TEST(ParserTest, ParseCompilationNodeEnumDefinitionStatementNodeWithMultiAssignment)
{
    const std::string input = "enum A { X = 10, Y, Z = 32, };";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);

    auto& translationUnit = compUnitPtr->getRoot();
    auto& firstStatement = *translationUnit.begin();
    ASSERT_EQ(firstStatement->getType(), EnumNodeType::ENUM_DEFINITION);

    auto* enumDefStatePtr = static_cast<EnumDefinitionStatementNode*>(firstStatement.get());
    ASSERT_NE(enumDefStatePtr, nullptr);
    ASSERT_EQ(enumDefStatePtr->getName(), "A");
    ASSERT_FALSE(enumDefStatePtr->empty());
    ASSERT_EQ(enumDefStatePtr->size(), 3);

    const auto& enumeratorMap = enumDefStatePtr->getEnumData()->enumeratorMap;
    const auto endIter = enumeratorMap.cend();
    auto findResult = enumeratorMap.find("X");

    ASSERT_NE(findResult, endIter);
    ASSERT_EQ(findResult->second.getIndex(), 0);
    ASSERT_EQ(findResult->second.getValue(), 10);

    findResult = enumeratorMap.find("Y");
    ASSERT_NE(findResult, endIter);
    ASSERT_EQ(findResult->second.getIndex(), 1);
    ASSERT_EQ(findResult->second.getValue(), 1);

    findResult = enumeratorMap.find("Z");
    ASSERT_NE(findResult, endIter);
    ASSERT_EQ(findResult->second.getIndex(), 2);
    ASSERT_EQ(findResult->second.getValue(), 32);
}

s32 main(s32 argc, char* argv[])
{
    reporter.setEnablePrint(false);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

