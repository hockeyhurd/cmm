/**
 * @author hockeyhurd
 * @version 2022-05-13
 */

#pragma once

#ifndef CMM_TOKEN_H
#define CMM_TOKEN_H

// cmm includes
#include <cmm/Types.h>
#include <cmm/StringView.h>

// std includes
#include <cstdio>

namespace cmm
{
    CMM_CONSTEXPR char CHAR_ASTERISK = '*';
    CMM_CONSTEXPR char CHAR_BACK_SLASH = '\\';
    CMM_CONSTEXPR char CHAR_CARRIAGE_RETURN = '\r';
    CMM_CONSTEXPR char CHAR_COLON = ':';
    CMM_CONSTEXPR char CHAR_COMMA = ',';
    CMM_CONSTEXPR char CHAR_DOUBLE_QOUTE = '"';
    CMM_CONSTEXPR char CHAR_EOF = EOF;
    CMM_CONSTEXPR char CHAR_EQUALS = '=';
    CMM_CONSTEXPR char CHAR_LCURLY_BRACKET = '{';
    CMM_CONSTEXPR char CHAR_LSQUARE_BRACKET = '[';
    CMM_CONSTEXPR char CHAR_LBRACKET = '[';
    CMM_CONSTEXPR char CHAR_MINUS = '-';
    CMM_CONSTEXPR char CHAR_NEWLINE = '\n';
    CMM_CONSTEXPR char CHAR_NULL = '\0';
    CMM_CONSTEXPR char CHAR_PERIOD = '.';
    CMM_CONSTEXPR char CHAR_PLUS = '+';
    CMM_CONSTEXPR char CHAR_RCURLY_BRACKET = '}';
    CMM_CONSTEXPR char CHAR_RSQUARE_BRACKET = ']';
    CMM_CONSTEXPR char CHAR_RBRACKET = ']';
    CMM_CONSTEXPR char CHAR_SEMI_COLON = ';';
    CMM_CONSTEXPR char CHAR_SINGLE_QOUTE = '\'';
    CMM_CONSTEXPR char CHAR_SPACE = ' ';
    CMM_CONSTEXPR char CHAR_TAB = '\t';

    enum class TokenType
    {
        BOOL = 0, CHAR, FLOAT, DOUBLE, INT16, INT32, INT64, NULL_T, STRING
    };

    CMM_CONSTEXPR_FUNC const char* toString(const TokenType tokenType) noexcept
    {
        switch (tokenType)
        {
        case TokenType::BOOL:
            return "bool";
        case TokenType::CHAR:
            return "char";
        case TokenType::FLOAT:
            return "float";
        case TokenType::DOUBLE:
            return "double";
        case TokenType::INT16:
            return "int16";
        case TokenType::INT32:
            return "int32";
        case TokenType::INT64:
            return "int64";
        case TokenType::NULL_T:
            return "NULL";
        case TokenType::STRING:
            return "string";
        default:
            return "<Invalid TokenType>";
        }
    }

    class Token
    {
    public:

        /**
         * Constructor initialized with the text to be tokenized.
         *
         * @param b the boolean.
         */
        explicit Token(const bool b) noexcept;

        /**
         * Constructor initialized with the text to be tokenized.
         *
         * @param ch the character represented by the char.
         */
        explicit Token(const char ch) noexcept;

        /**
         * Constructor initialized with the text to be tokenized.
         *
         * @param intValue the 32-bit integer value.
         */
        explicit Token(const s32 intValue) noexcept;

        /**
         * Constructor initialized with the text to be tokenized.
         *
         * @param doubleValue the 64-bit double value.
         */
        explicit Token(const f64 doubleValue) noexcept;

        /**
         * Constructor initialized with the text to be tokenized.
         *
         * @param str the character string represented by the StringView.
         */
        explicit Token(StringView str) noexcept;

        /**
         * Default copy constructor.
         */
        Token(const Token&) noexcept = default;

        /**
         * Default move constructor.
         */
        Token(Token&&) noexcept = default;

        /**
         * Default destructor.
         */
        ~Token() noexcept = default;

        /**
         * Default copy assignment operator.
         */
        Token& operator= (const Token&) = default;

        /**
         * Default move assignment operator.
         */
        Token& operator= (Token&&) = default;

        /**
         * Gets the TokenType.
         *
         * @return TokenType.
         */
        TokenType getType() const noexcept;

        /**
         * Gets the value as a bool.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a char (valid or not).
         *
         * @return bool.
         */
        bool asBool() const noexcept;

        /**
         * Gets whether the token is a bool.
         *
         * @return bool.
         */
        bool isBool() const noexcept;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param b the bool value to set.
         */
        void setBool(const bool b) noexcept;

        /**
         * Gets the value as a char.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a char (valid or not).
         *
         * @return char.
         */
        char asChar() const noexcept;

        /**
         * Gets whether the token is a char.
         *
         * @return bool.
         */
        bool isChar() const noexcept;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param ch the char to set.
         */
        void setChar(const char ch) noexcept;

        /**
         * Gets the value as a double.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a double (valid or not).
         *
         * @return f64.
         */
        f64 asDouble() const noexcept;

        /**
         * Gets whether the token is a double.
         *
         * @return bool.
         */
        bool isDouble() const noexcept;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param doubleValue the double to set.
         */
        void setDouble(const f64 doubleValue) noexcept;

        /**
         * Gets the value as a int16.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a int16 (valid or not).
         *
         * @return f64.
         */
        s16 asInt16() const noexcept;

        /**
         * Gets whether the token is a int16.
         *
         * @return bool.
         */
        bool isInt16() const noexcept;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param doubleValue the int16 to set.
         */
        void setInt16(const s16 intValue) noexcept;

        /**
         * Gets the value as a int32.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a int32 (valid or not).
         *
         * @return f64.
         */
        s32 asInt32() const noexcept;

        /**
         * Gets whether the token is a int32.
         *
         * @return bool.
         */
        bool isInt32() const noexcept;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param doubleValue the int32 to set.
         */
        void setInt32(const s32 intValue) noexcept;

        /**
         * Gets the value as a int64.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a int64 (valid or not).
         *
         * @return f64.
         */
        s64 asInt64() const noexcept;

        /**
         * Gets whether the token is a int64.
         *
         * @return bool.
         */
        bool isInt64() const noexcept;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param doubleValue the int64 to set.
         */
        void setInt64(const s64 intValue) noexcept;

        /**
         * Gets whether the token is a null value.
         *
         * @return bool.
         */
        bool isNull() const noexcept;

        /**
         * Sets the underlying value to null.
         */
        void setNull() noexcept;

        /**
         * Gets the value as a char.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a char (valid or not).
         *
         * @return StringView.
         */
        StringView asCString() const noexcept;

        /**
         * Gets whether the token is a StringView.
         *
         * @return bool.
         */
        bool isCString() const noexcept;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param str the StringView to set.
         */
        void setCString(StringView str) noexcept;

    private:
        union Values
        {
            bool b;
            char ch;
            f64 doubleValue;
            s16 int16Value;
            s32 int32Value;
            s64 int64Value;
            StringView* str;
            char strBuffer[sizeof(StringView)];
        };

        // The type of the token
        TokenType type;

        // The underlying token value.
        Values value;
    };
}

#endif //!CMM_TOKEN_H

