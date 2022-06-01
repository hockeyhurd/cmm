#include <cmm/Types.h>
#include <cmm/Lexer.h>
#include <cmm/StringView.h>
#include <cmm/Token.h>

#include <gtest/gtest.h>

#include <string>

using namespace cmm;

TEST(LexerTest, LexEmpty)
{
    const std::string input = "";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexWhitespace)
{
    const std::string input = "  \n\t\n\n               \r";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexBoolTrue)
{
    const bool value = true;
    const std::string input = " true ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::BOOL);
    ASSERT_EQ(token.asBool(), value);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexBoolFalse)
{
    const bool value = false;
    const std::string input = " false ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::BOOL);
    ASSERT_EQ(token.asBool(), value);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexPosDouble)
{
    const f64 value = 1.234;
    const std::string input = " 1.234 ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::DOUBLE);
    ASSERT_EQ(token.asDouble(), value);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexNegDouble)
{
    const f64 value = -1.234;
    const std::string input = " -1.234 ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::DOUBLE);
    ASSERT_EQ(token.asDouble(), value);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexPosDoubleE3)
{
    const f64 value = 1.234E3;
    const std::string input = " 1.234E3 ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::DOUBLE);
    ASSERT_EQ(token.asDouble(), value);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexNegDoubleE4)
{
    const f64 value = -1.234E4;
    const std::string input = " -1.234E4 ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::DOUBLE);
    ASSERT_EQ(token.asDouble(), value);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexPosFloat)
{
    const f32 value = 1.234F;
    const std::string input = " 1.234F ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::FLOAT);
    ASSERT_EQ(token.asFloat(), value);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexNegFloat)
{
    const f64 value = -1.234f;
    const std::string input = " -1.234f ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::FLOAT);
    ASSERT_EQ(token.asFloat(), value);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexPosFloatE3)
{
    const f32 value = 1.234E3F;
    const std::string input = " 1.234E3F ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::FLOAT);
    ASSERT_EQ(token.asFloat(), value);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexNegFloatE4)
{
    const f32 value = -1.234E4f;
    const std::string input = " -1.234E4f ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::FLOAT);
    ASSERT_EQ(token.asFloat(), value);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexPosInt32)
{
    const f64 value = 1234;
    const std::string input = " 1234 ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::INT32);
    ASSERT_EQ(token.asInt32(), value);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexNegInt32)
{
    const f64 value = -1234;
    const std::string input = " -1234 ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::INT32);
    ASSERT_EQ(token.asInt32(), value);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexString)
{
    const std::string input = "\"\\\"Hello, world!\\\"\"";
    const StringView strView(input.c_str(), input.c_str() + input.size() - 2);
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::STRING);

    auto outValue = token.asCString();
    ASSERT_EQ(outValue.size(), strView.size());
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexStringUnescapedBackslashError)
{
    const std::string text = " \\ ";
    Lexer lexer(text);
    Token token('\0', false);

    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_NE(token.getType(), TokenType::STRING);
}

TEST(LexerTest, LexStringUnescapedBackslashAndDoubleQuoteError)
{
    const std::string text = " \\\" ";
    Lexer lexer(text);
    Token token('\0', false);

    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_NE(token.getType(), TokenType::STRING);
}

TEST(LexerTest, ParseStringWithEscapedQuotes)
{
    const std::string value = "\\\"hello\\\"";
    const std::string text = "\"\\\"hello\\\"\"";
    Lexer lexer(text);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));

    const auto& outValue = token.asCString();
    ASSERT_EQ(value.size(), outValue.size());
    ASSERT_EQ(value, outValue);
}

TEST(LexerTest, LexSymbol)
{
    const std::string input = " { ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::SYMBOL);
    ASSERT_EQ(token.asSymbol(), CHAR_LCURLY_BRACKET);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

s32 main(s32 argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

