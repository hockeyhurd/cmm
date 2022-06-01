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

    Token::Token(const char ch, const bool isSymbol) noexcept
    {
        if (isSymbol)
        {
            type = TokenType::SYMBOL;
        }

        else
        {
            type = TokenType::CHAR;
        }

        value.ch = ch;
    }

    Token::Token(const f64 doubleValue) noexcept : type(TokenType::DOUBLE)
    {
        value.doubleValue = doubleValue;
    }

    Token::Token(const std::string& str) : type(TokenType::STRING)
    {
        value.str = new std::string(str);
    }

    Token::Token(std::string&& str) : type(TokenType::STRING)
    {
        value.str = new std::string(std::move(str));
    }

    Token::Token(const Token& other) : type(other.type)
    {
        if (other.type == TokenType::STRING)
        {
            value.str = new std::string(*other.value.str);
        }

        else
        {
            value = other.value;
        }
    }

    Token::Token(Token&& other) noexcept : type(other.type)
    {
        if (other.type == TokenType::STRING)
        {
            value.str = other.value.str;
            other.value.str = nullptr;
        }

        else
        {
            value = other.value;
            other.value.str = nullptr; // zero out other value
        }
    }

    Token::~Token()
    {
        if (type == TokenType::STRING && value.str != nullptr)
        {
            delete value.str;
            value.str = nullptr;
        }
    }

    Token& Token::operator= (const Token& other)
    {
        if (this == &other)
        {
            return *this;
        }

        // See if we are already a string
        else if (type == TokenType::STRING)
        {
            // Other is also a string
            if (other.type == TokenType::STRING)
            {
                // uninitialized string, create a new one.
                if (value.str == nullptr)
                {
                    value.str = new std::string(*other.value.str);
                }

                // Set our string's value to the other.
                else
                {
                    *value.str = *other.value.str;
                }
            }

            // Other is NOT a string, but we are.  Cleanup the allocated memory.
            else if (value.str != nullptr)
            {
                delete value.str;
                value.str = nullptr;
            }
        }

        // We are not a string, but the other value is.  Create a new string copy.
        else if (other.type == TokenType::STRING)
        {
            value.str = new std::string(*other.value.str);
        }

        type = other.type;

        return *this;
    }

    Token& Token::operator= (Token&& other) noexcept
    {
        // If we have an allocated std::string, need to clean this up before
        // aquiring a new value.
        if (type == TokenType::STRING && value.str != nullptr)
        {
            delete value.str;
            value.str = nullptr;
        }

        type = other.type;
        value = other.value;
        other.value.str = nullptr; // zero out the value

        return *this;
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

    f32 Token::asFloat() const noexcept
    {
        return value.floatValue;
    }

    bool Token::isFloat() const noexcept
    {
        return type == TokenType::FLOAT;
    }

    void Token::setFloat(const f32 floatValue) noexcept
    {
        type = TokenType::FLOAT;
        value.floatValue = floatValue;
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

    std::string& Token::asCString() noexcept
    {
        return *value.str;
    }

    const std::string& Token::asCString() const noexcept
    {
        return *value.str;
    }

    bool Token::isCString() const noexcept
    {
        return type == TokenType::STRING;
    }

    void Token::setCString(const std::string& str)
    {
        // Check if already a std::string
        if (type == TokenType::STRING)
        {
            if (value.str == nullptr)
            {
                value.str = new std::string(str);
            }

            else
            {
                *value.str = str;
            }
        }

        else
        {
            type = TokenType::STRING;
            value.str = new std::string(str);
        }
    }

    void Token::setCString(std::string&& str)
    {
        // Check if already a std::string
        if (type == TokenType::STRING)
        {
            if (value.str == nullptr)
            {
                value.str = new std::string(std::move(str));
            }

            else
            {
                *value.str = str;
            }
        }

        else
        {
            type = TokenType::STRING;
            value.str = new std::string(std::move(str));
        }
    }

    char Token::asSymbol() const noexcept
    {
        return value.symbol;
    }

    bool Token::isSymbol() const noexcept
    {
        return type == TokenType::SYMBOL;
    }

    void Token::setSymbol(const char symbol) noexcept
    {
        type = TokenType::SYMBOL;
        value.symbol = symbol;
    }
}

