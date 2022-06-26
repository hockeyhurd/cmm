/**
 * @author hockeyhurd
 * @version 2022-05-13
 */

#include <cmm/Token.h>

namespace cmm
{
    Token::Token(const bool b) CMM_NOEXCEPT : type(TokenType::BOOL)
    {
        value.b = b;
    }

    Token::Token(const char ch, const bool isSymbol) CMM_NOEXCEPT
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

    Token::Token(const f64 doubleValue) CMM_NOEXCEPT : type(TokenType::DOUBLE)
    {
        value.doubleValue = doubleValue;
    }

    Token::Token(const std::string& str, const bool isSymbol) : type(TokenType::STRING)
    {
        value.str = new std::string(str);
    }

    Token::Token(std::string&& str, const bool isSymbol) : type(TokenType::STRING)
    {
        value.str = new std::string(std::move(str));
    }

    Token::Token(const Token& other) : type(other.type)
    {
        if (other.type == TokenType::STRING || other.type == TokenType::SYMBOL)
        {
            value.str = new std::string(*other.value.str);
        }

        else
        {
            value = other.value;
        }
    }

    Token::Token(Token&& other) CMM_NOEXCEPT : type(other.type)
    {
        if (other.type == TokenType::STRING || other.type == TokenType::SYMBOL)
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
        if ((type == TokenType::STRING || type == TokenType::SYMBOL) && value.str != nullptr)
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
        else if (type == TokenType::STRING || type == TokenType::SYMBOL)
        {
            // Other is also a string
            if (other.type == TokenType::STRING || type == TokenType::SYMBOL)
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
        else if (other.type == TokenType::STRING || other.type == TokenType::SYMBOL)
        {
            value.str = new std::string(*other.value.str);
        }

        type = other.type;

        return *this;
    }

    Token& Token::operator= (Token&& other) CMM_NOEXCEPT
    {
        // If we have an allocated std::string, need to clean this up before
        // aquiring a new value.
        if ((type == TokenType::STRING || type == TokenType::SYMBOL) && value.str != nullptr)
        {
            delete value.str;
            value.str = nullptr;
        }

        type = other.type;
        value = other.value;
        other.value.str = nullptr; // zero out the value

        return *this;
    }

    TokenType Token::getType() const CMM_NOEXCEPT
    {
        return type;
    }

    bool Token::asBool() const CMM_NOEXCEPT
    {
        return value.b;
    }

    bool Token::isBool() const CMM_NOEXCEPT
    {
        return type == TokenType::BOOL;
    }

    void Token::setBool(const bool b) CMM_NOEXCEPT
    {
        type = TokenType::BOOL;
        value.b = b;
    }

    char Token::asChar() const CMM_NOEXCEPT
    {
        return value.ch;
    }

    bool Token::isChar() const CMM_NOEXCEPT
    {
        return type == TokenType::CHAR;
    }

    void Token::setChar(const char ch) CMM_NOEXCEPT
    {
        type = TokenType::CHAR;
        value.ch = ch;
    }

    f64 Token::asDouble() const CMM_NOEXCEPT
    {
        return value.doubleValue;
    }

    bool Token::isDouble() const CMM_NOEXCEPT
    {
        return type == TokenType::DOUBLE;
    }

    void Token::setDouble(const f64 doubleValue) CMM_NOEXCEPT
    {
        type = TokenType::DOUBLE;
        value.doubleValue = doubleValue;
    }

    f32 Token::asFloat() const CMM_NOEXCEPT
    {
        return value.floatValue;
    }

    bool Token::isFloat() const CMM_NOEXCEPT
    {
        return type == TokenType::FLOAT;
    }

    void Token::setFloat(const f32 floatValue) CMM_NOEXCEPT
    {
        type = TokenType::FLOAT;
        value.floatValue = floatValue;
    }

    s16 Token::asInt16() const CMM_NOEXCEPT
    {
        return value.int16Value;
    }

    bool Token::isInt16() const CMM_NOEXCEPT
    {
        return type == TokenType::INT16;
    }

    void Token::setInt16(const s16 int16Value) CMM_NOEXCEPT
    {
        type = TokenType::INT16;
        value.int16Value = int16Value;
    }

    s32 Token::asInt32() const CMM_NOEXCEPT
    {
        return value.int32Value;
    }

    bool Token::isInt32() const CMM_NOEXCEPT
    {
        return type == TokenType::INT32;
    }

    void Token::setInt32(const s32 int32Value) CMM_NOEXCEPT
    {
        type = TokenType::INT32;
        value.int32Value = int32Value;
    }

    s64 Token::asInt64() const CMM_NOEXCEPT
    {
        return value.int64Value;
    }

    bool Token::isInt64() const CMM_NOEXCEPT
    {
        return type == TokenType::INT32;
    }

    void Token::setInt64(const s64 int64Value) CMM_NOEXCEPT
    {
        type = TokenType::INT64;
        value.int64Value = int64Value;
    }

    bool Token::isNull() const CMM_NOEXCEPT
    {
        return type == TokenType::NULL_T;
    }

    void Token::setNull() CMM_NOEXCEPT
    {
        type = TokenType::NULL_T;
    }

    std::string& Token::asCString() CMM_NOEXCEPT
    {
        return *value.str;
    }

    const std::string& Token::asCString() const CMM_NOEXCEPT
    {
        return *value.str;
    }

    bool Token::isCString() const CMM_NOEXCEPT
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

    char Token::asCharSymbol() const CMM_NOEXCEPT
    {
        return value.symbol;
    }

    bool Token::isCharSymbol() const CMM_NOEXCEPT
    {
        return type == TokenType::CHAR_SYMBOL;
    }

    void Token::setCharSymbol(const char symbol) CMM_NOEXCEPT
    {
        type = TokenType::CHAR_SYMBOL;
        value.symbol = symbol;
    }

    std::string& Token::asStringSymbol() CMM_NOEXCEPT
    {
        return *value.str;
    }

    const std::string& Token::asStringSymbol() const CMM_NOEXCEPT
    {
        return *value.str;
    }

    bool Token::isStringSymbol() const CMM_NOEXCEPT
    {
        return type == TokenType::SYMBOL;
    }

    void Token::setStringSymbol(const std::string& strSymbol) CMM_NOEXCEPT
    {
        // Check if already a std::string
        if (type == TokenType::STRING || type == TokenType::SYMBOL)
        {
            if (value.str == nullptr)
            {
                value.str = new std::string(strSymbol);
            }

            else
            {
                *value.str = strSymbol;
            }
        }

        else
        {
            type = TokenType::SYMBOL;
            value.str = new std::string(strSymbol);
        }
    }

    void Token::setStringSymbol(std::string&& strSymbol) CMM_NOEXCEPT
    {
        // Check if already a std::string
        if (type == TokenType::STRING || type == TokenType::SYMBOL)
        {
            if (value.str == nullptr)
            {
                value.str = new std::string(std::move(strSymbol));
            }

            else
            {
                *value.str = strSymbol;
            }
        }

        else
        {
            type = TokenType::SYMBOL;
            value.str = new std::string(std::move(strSymbol));
        }
    }
}

