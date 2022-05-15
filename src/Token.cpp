/**
 * @author hockeyhurd
 * @version 2022-05-13
 */

#include <cmm/Token.h>

namespace cmm
{
    Token::Token(const bool b) noexcept : type(TokenType::BOOL)
    {
        value.b = b;
    }

    Token::Token(const char ch) noexcept : type(TokenType::CHAR)
    {
        value.ch = ch;
    }

    Token::Token(const f64 doubleValue) noexcept : type(TokenType::DOUBLE)
    {
        value.doubleValue = doubleValue;
    }

    Token::Token(StringView str) noexcept : type(TokenType::STRING)
    {
        auto* stringView = reinterpret_cast<StringView*>(value.strBuffer);
        *stringView = str;
    }

    TokenType Token::getType() const noexcept
    {
        return type;
    }

    bool Token::asBool() const noexcept
    {
        return value.b;
    }

    bool Token::isBool() const noexcept
    {
        return type == TokenType::BOOL;
    }

    void Token::setBool(const bool b) noexcept
    {
        type = TokenType::BOOL;
        value.b = b;
    }

    char Token::asChar() const noexcept
    {
        return value.ch;
    }

    bool Token::isChar() const noexcept
    {
        return type == TokenType::CHAR;
    }

    void Token::setChar(const char ch) noexcept
    {
        type = TokenType::CHAR;
        value.ch = ch;
    }

    f64 Token::asDouble() const noexcept
    {
        return value.doubleValue;
    }

    bool Token::isDouble() const noexcept
    {
        return type == TokenType::DOUBLE;
    }

    void Token::setDouble(const f64 doubleValue) noexcept
    {
        type = TokenType::DOUBLE;
        value.doubleValue = doubleValue;
    }

    s16 Token::asInt16() const noexcept
    {
        return value.int16Value;
    }

    bool Token::isInt16() const noexcept
    {
        return type == TokenType::INT16;
    }

    void Token::setInt16(const s16 int16Value) noexcept
    {
        type = TokenType::INT16;
        value.int16Value = int16Value;
    }

    s32 Token::asInt32() const noexcept
    {
        return value.int32Value;
    }

    bool Token::isInt32() const noexcept
    {
        return type == TokenType::INT32;
    }

    void Token::setInt32(const s32 int32Value) noexcept
    {
        type = TokenType::INT32;
        value.int32Value = int32Value;
    }

    s64 Token::asInt64() const noexcept
    {
        return value.int64Value;
    }

    bool Token::isInt64() const noexcept
    {
        return type == TokenType::INT32;
    }

    void Token::setInt64(const s64 int64Value) noexcept
    {
        type = TokenType::INT64;
        value.int64Value = int64Value;
    }

    bool Token::isNull() const noexcept
    {
        return type == TokenType::NULL_T;
    }

    void Token::setNull() noexcept
    {
        type = TokenType::NULL_T;
    }

    StringView Token::asCString() const noexcept
    {
        const auto* stringView = reinterpret_cast<const StringView*>(value.strBuffer);
        return *stringView;
    }

    bool Token::isCString() const noexcept
    {
        return type == TokenType::STRING;
    }

    void Token::setCString(StringView str) noexcept
    {
        type = TokenType::STRING;
        auto* stringView = reinterpret_cast<StringView*>(value.strBuffer);
        *stringView = str;
    }
}

