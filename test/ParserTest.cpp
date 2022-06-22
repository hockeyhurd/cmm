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

TEST(ParserTest, ParseCompilationNodeBoolBool)
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

s32 main(s32 argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

