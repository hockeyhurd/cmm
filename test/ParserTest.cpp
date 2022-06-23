#include <cmm/BinOpNode.h>
#include <cmm/CompilationUnitNode.h>
#include <cmm/Lexer.h>
#include <cmm/LitteralNode.h>
#include <cmm/Parser.h>
#include <cmm/Token.h>
#include <cmm/Types.h>

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
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::LITTERAL);

    auto rootBoolPtr = std::static_pointer_cast<LitteralNode>(compUnitPtr->getRoot());
    ASSERT_EQ(rootBoolPtr->getTypeof(), EnumCType::BOOL);
    ASSERT_TRUE(rootBoolPtr->getValue().valueBool);
}

TEST(ParserTest, ParseCompilationNodeBoolFalse)
{
    const std::string input = "false ;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::LITTERAL);

    auto rootBoolPtr = std::static_pointer_cast<LitteralNode>(compUnitPtr->getRoot());
    ASSERT_EQ(rootBoolPtr->getTypeof(), EnumCType::BOOL);
    ASSERT_FALSE(rootBoolPtr->getValue().valueBool);
}

TEST(ParserTest, ParseCompilationNodeInt)
{
    const std::string input = "32;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::LITTERAL);

    auto rootIntPtr = std::static_pointer_cast<LitteralNode>(compUnitPtr->getRoot());
    ASSERT_EQ(rootIntPtr->getTypeof(), EnumCType::INT32);
    ASSERT_EQ(rootIntPtr->getValue().valueS32, 32);
}

TEST(ParserTest, ParseCompilationNodeIntSum)
{
    const std::string input = "10 + 32;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::BIN_OP);

    auto rootSumPtr = std::static_pointer_cast<BinOpNode>(compUnitPtr->getRoot());
    ASSERT_EQ(rootSumPtr->getTypeof(), EnumBinOpNodeType::ADD);
}

TEST(ParserTest, ParseCompilationNodeFloatSubtract)
{
    const std::string input = "10.0F - 32.0F;";
    Parser parser(input);
    std::string errorMessage;
    auto compUnitPtr = parser.parseCompilationUnit(&errorMessage);

    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(compUnitPtr, nullptr);
    ASSERT_EQ(compUnitPtr->getRootType(), NodeType::BIN_OP);

    auto rootSumPtr = std::static_pointer_cast<BinOpNode>(compUnitPtr->getRoot());
    ASSERT_EQ(rootSumPtr->getTypeof(), EnumBinOpNodeType::SUBTRACT);
}

s32 main(s32 argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

