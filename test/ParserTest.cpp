#include <cmm/Types.h>
#include <cmm/Lexer.h>
#include <cmm/Parser.h>

#include <gtest/gtest.h>

#include <string>

using namespace cmm;

TEST(ParserTest, ParseEmpty)
{
    const std::string input = "";
    Parser parser(input);
}

s32 main(s32 argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

