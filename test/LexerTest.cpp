#include <cmm/Types.h>
#include <cmm/Lexer.h>
#include <cmm/Token.h>
#include <cmm/container/StringView.h>

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

TEST(LexerTest, LexSemiColon)
{
    const std::string input = " ; ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_SEMI_COLON);
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

TEST(LexerTest, LexPosDouble1Period)
{
    const f64 value = 1.;
    const std::string input = " 1. ";
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

TEST(LexerTest, LexNegDouble1Period)
{
    const f64 value = -1.;
    const std::string input = " -1. ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::DOUBLE);
    ASSERT_EQ(token.asDouble(), value);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexPosDoubleEError)
{
    const std::string input = " 1.234E ";
    Lexer lexer(input);
    Token token('\0', false);

    std::string errorMessage;
    ASSERT_FALSE(lexer.nextToken(token, &errorMessage));
    ASSERT_FALSE(errorMessage.empty());
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexPosDoubleDoubleEError)
{
    const std::string input = " 1.234EE ";
    Lexer lexer(input);
    Token token('\0', false);

    std::string errorMessage;
    ASSERT_FALSE(lexer.nextToken(token, &errorMessage));
    ASSERT_FALSE(errorMessage.empty());

    // Trailing second 'E'??  TODO: Does this make sense.
    ASSERT_FALSE(lexer.completedOrWhitespaceOnly());
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

TEST(LexerTest, LexNull)
{
    const std::string input = " NULL ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::NULL_T);
    ASSERT_TRUE(token.isNull());
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexString)
{
    const std::string input = "\"\\\"Hello, world!\\\"\"";
    const StringView<char> strView(input.c_str(), input.size() - 2);
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
    ASSERT_EQ(value, outValue.string());
}

TEST(LexerTest, LexSymbol)
{
    const std::string input = " { ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_LCURLY_BRACKET);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexSymbolPlus)
{
    const std::string input = " + ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_PLUS);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexSymbolPlusPlus)
{
    const std::string input = " ++ ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::SYMBOL);
    ASSERT_EQ(token.asStringSymbol(), "++");
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexSymbolMineMinus)
{
    const std::string input = " -- ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::SYMBOL);
    ASSERT_EQ(token.asStringSymbol(), "--");
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexSymbolPlusWithInvalidEError)
{
    const std::string input = " +e";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_PLUS);
    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexSymbolPlusWithInvalidFError)
{
    const std::string input = " +F";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_PLUS);
    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexSymbolPlusWithInvalidPeriodButRecoverAndSeperateTokens)
{
    const std::string input = " +. ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_PLUS);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_PERIOD);
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexSymbolMinus)
{
    const std::string input = " - ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_MINUS);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexSymbolMinusWithInvalidEError)
{
    const std::string input = " -E";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_MINUS);
    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexSymbolMinusWithInvalidFError)
{
    const std::string input = " -f";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_MINUS);
    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexSymbolMinusWithInvalidPeriodButRecoverAndSeperateTokens)
{
    const std::string input = " -. ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_MINUS);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_PERIOD);
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexSymbolForwardSlash)
{
    const std::string input = " / ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_FORWARD_SLASH);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexVariable)
{
    const char* output = "awesomeVariable123_";
    const std::string input = " awesomeVariable123_ ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::SYMBOL);
    ASSERT_EQ(token.asStringSymbol(), output);
    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexFunctionEmptyArgs)
{
    const char* output = "awesomeFunction";
    const std::string input = " awesomeFunction() ";
    Lexer lexer(input);
    Token token('\0', false);

    // name
    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::SYMBOL);
    ASSERT_EQ(token.asStringSymbol(), output);

    // Open paren
    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_LPAREN);

    // Close paren
    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_RPAREN);

    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexFunctionSnakeCaseEmptyArgs)
{
    const char* output = "awesome_snake_case_function";
    const std::string input = " awesome_snake_case_function() ";
    Lexer lexer(input);
    Token token('\0', false);

    // name
    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::SYMBOL);
    ASSERT_EQ(token.asStringSymbol(), output);

    // Open paren
    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_LPAREN);

    // Close paren
    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), CHAR_RPAREN);

    ASSERT_FALSE(lexer.nextToken(token));
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexComment)
{
    const f64 eps   = 0.000001;
    const f64 value = 3.14195;
    const std::string input = " // Hello, world!\n 3.14195 ";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::DOUBLE);
    ASSERT_NEAR(token.asDouble(), value, eps);
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexGreaterThanCharSymbol)
{
    const std::string input = ">";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), '>');
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexLessThanCharSymbol)
{
    const std::string input = "<";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), '<');
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexDotCharSymbol)
{
    const std::string input = ".";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::CHAR_SYMBOL);
    ASSERT_EQ(token.asCharSymbol(), '.');
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

TEST(LexerTest, LexArrowStringSymbol)
{
    const std::string input = "->";
    Lexer lexer(input);
    Token token('\0', false);

    ASSERT_TRUE(lexer.nextToken(token));
    ASSERT_EQ(token.getType(), TokenType::SYMBOL);
    ASSERT_EQ(token.asStringSymbol(), input);
    ASSERT_TRUE(lexer.completedOrWhitespaceOnly());
}

s32 main(s32 argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

