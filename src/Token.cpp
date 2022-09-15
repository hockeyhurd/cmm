/**
 * @author hockeyhurd
 * @version 2022-05-13
 */

// Our includes
#include <cmm/Token.h>

// std includes
#include <functional>
#include <stdexcept>
#include <type_traits>

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
        if ((isCStringOrStringSymbol()) && value.str != nullptr)
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
        else if (isCStringOrStringSymbol())
        {
            // Other is also a string
            if (other.isCStringOrStringSymbol())
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
        if ((isCStringOrStringSymbol()) && value.str != nullptr)
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
        conditionallyCleanString();

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
        conditionallyCleanString();

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
        conditionallyCleanString();

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
        conditionallyCleanString();

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
        conditionallyCleanString();

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
        conditionallyCleanString();

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
        conditionallyCleanString();

        type = TokenType::INT64;
        value.int64Value = int64Value;
    }

    bool Token::isNull() const CMM_NOEXCEPT
    {
        return type == TokenType::NULL_T;
    }

    void Token::setNull() CMM_NOEXCEPT
    {
        conditionallyCleanString();

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
        conditionallyCleanString();

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
        if (isCStringOrStringSymbol())
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
        if (isCStringOrStringSymbol())
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

    bool Token::operator== (const Token& other) const
    {
        if (type != other.type)
        {
            return false;
        }

        switch (type)
        {
        case TokenType::BOOL:
            return value.b == other.value.b;
        case TokenType::CHAR:
        case TokenType::CHAR_SYMBOL:
            return value.ch == other.value.ch;
        case TokenType::DOUBLE:
            return *reinterpret_cast<const u64*>(&value.doubleValue) ==
                   *reinterpret_cast<const u64*>(&other.value.doubleValue);
        case TokenType::FLOAT:
            return *reinterpret_cast<const u32*>(&value.floatValue) ==
                   *reinterpret_cast<const u32*>(&other.value.floatValue);
        case TokenType::INT16:
            return value.int16Value == other.value.int16Value;
        case TokenType::INT32:
            return value.int32Value == other.value.int32Value;
        case TokenType::INT64:
            return value.int64Value == other.value.int64Value;
        case TokenType::NULL_T:
            return true;
        case TokenType::SYMBOL:
        case TokenType::STRING:
            {
                // Compare pointers
                if (value.str == other.value.str)
                {
                    return true;
                }

                // Compare each character
                return *value.str == *other.value.str;
            }
            return false;
        default:
            return false;
        }

        return false;
    }

    bool Token::operator!= (const Token& other) const
    {
        return !(*this == other);
    }

    void Token::conditionallyCleanString() CMM_NOEXCEPT
    {
        if (isCStringOrStringSymbol() && value.str != nullptr)
        {
            delete value.str;
            value.str = nullptr;
        }
    }

    bool Token::isCStringOrStringSymbol() const CMM_NOEXCEPT
    {
        return type == TokenType::STRING || type == TokenType::SYMBOL;
    }

    std::size_t TokenHasher::operator() (const Token& token) const
    {
        std::size_t result = 0;

        switch (token.getType())
        {
        case TokenType::BOOL:
            result = *reinterpret_cast<const std::size_t*>(&token.value.b);
            break;
        case TokenType::CHAR_SYMBOL:
        case TokenType::CHAR:
            result = static_cast<std::size_t>(token.value.ch);
            break;
        case TokenType::DOUBLE:
            result = *reinterpret_cast<const std::size_t*>(&token.value.doubleValue);
            break;
        case TokenType::FLOAT:
            result = *reinterpret_cast<const std::size_t*>(&token.value.floatValue);
            break;
        case TokenType::INT16:
            result = static_cast<std::size_t>(token.value.int16Value);
            break;
        case TokenType::INT32:
            result = static_cast<std::size_t>(token.value.int32Value);
            break;
        case TokenType::INT64:
            result = static_cast<std::size_t>(token.value.int64Value);
            break;
        case TokenType::NULL_T:
            result = 0;
            break;
        case TokenType::SYMBOL:
        case TokenType::STRING:
            result = std::hash<std::string>()(*token.value.str);
            break;
        default:
            throw std::runtime_error("Unexpected token type");
        }

        return result;
    }

    std::size_t TokenTypeHasher::operator() (const TokenType& type) const
    {
        const auto result = *reinterpret_cast<const std::size_t*>(&type);
        return result;
        // return static_cast<std::underlying_type_t<TokenType>>(type);
    }
}

