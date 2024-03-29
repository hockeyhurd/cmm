/**
 * Represents a single token fed to the lexer.
 *
 * @author hockeyhurd
 * @version 2022-05-13
 */

#pragma once

#ifndef CMM_TOKEN_H
#define CMM_TOKEN_H

// cmm includes
#include <cmm/Types.h>

// std includes
#include <string>
#include <cstdio>

namespace cmm
{
    CMM_CONSTEXPR char CHAR_AMPERSAND = '&';
    CMM_CONSTEXPR char CHAR_ASTERISK = '*';
    CMM_CONSTEXPR char CHAR_BACK_SLASH = '\\';
    CMM_CONSTEXPR char CHAR_BACK_SPACE = (char) 12;
    CMM_CONSTEXPR char CHAR_CARRIAGE_RETURN = '\r';
    CMM_CONSTEXPR char CHAR_COLON = ':';
    CMM_CONSTEXPR char CHAR_COMMA = ',';
    CMM_CONSTEXPR char CHAR_DOUBLE_QOUTE = '"';
    CMM_CONSTEXPR char CHAR_EOF = EOF;
    CMM_CONSTEXPR char CHAR_EQUALS = '=';
    CMM_CONSTEXPR char CHAR_FORM_FEED = (char) 12;
    CMM_CONSTEXPR char CHAR_FORWARD_SLASH = '/';
    CMM_CONSTEXPR char CHAR_GT = '>';
    CMM_CONSTEXPR char CHAR_LCURLY_BRACKET = '{';
    CMM_CONSTEXPR char CHAR_LPAREN = '(';
    CMM_CONSTEXPR char CHAR_LSQUARE_BRACKET = '[';
    CMM_CONSTEXPR char CHAR_LT = '<';
    CMM_CONSTEXPR char CHAR_MINUS = '-';
    CMM_CONSTEXPR char CHAR_NEWLINE = '\n';
    CMM_CONSTEXPR char CHAR_NULL = '\0';
    CMM_CONSTEXPR char CHAR_PERIOD = '.';
    CMM_CONSTEXPR char CHAR_PLUS = '+';
    CMM_CONSTEXPR char CHAR_RCURLY_BRACKET = '}';
    CMM_CONSTEXPR char CHAR_RPAREN = ')';
    CMM_CONSTEXPR char CHAR_RSQUARE_BRACKET = ']';
    CMM_CONSTEXPR char CHAR_SEMI_COLON = ';';
    CMM_CONSTEXPR char CHAR_SINGLE_QOUTE = '\'';
    CMM_CONSTEXPR char CHAR_SPACE = ' ';
    CMM_CONSTEXPR char CHAR_TAB = '\t';
    CMM_CONSTEXPR char CHAR_UNDERSCORE = '_';

    enum class TokenType
    {
        BOOL = 0, CHAR, CHAR_SYMBOL, FLOAT, DOUBLE, INT16, INT32, INT64, NULL_T, STRING, SYMBOL
    };

    CMM_CONSTEXPR_FUNC const char* toString(const TokenType tokenType) CMM_NOEXCEPT
    {
        switch (tokenType)
        {
        case TokenType::BOOL:
            return "bool";
        case TokenType::CHAR:
            return "char";
        case TokenType::CHAR_SYMBOL:
            return "char_symbol";
        case TokenType::FLOAT:
            return "float";
        case TokenType::DOUBLE:
            return "double";
        case TokenType::INT16:
            return "i16";
        case TokenType::INT32:
            return "i32";
        case TokenType::INT64:
            return "i64";
        case TokenType::NULL_T:
            return "NULL";
        case TokenType::STRING:
            return "string";
        case TokenType::SYMBOL:
            return "symbol";
        default:
            return "<Invalid TokenType>";
        }
    }

    // Forward declaration
    struct TokenHasher;

    class Token
    {
        friend TokenHasher;

    public:

        /**
         * Constructor initialized with the text to be tokenized.
         *
         * @param b the boolean.
         */
        explicit Token(const bool b) CMM_NOEXCEPT;

        /**
         * Constructor initialized with the text to be tokenized.
         *
         * @param ch the character represented by the char.
         * @param isSymbol bool whether this is a char type "'a'" vs char symbol "a".
         */
        explicit Token(const char ch, const bool isSymbol) CMM_NOEXCEPT;

        /**
         * Constructor initialized with the text to be tokenized.
         *
         * @param intValue the 32-bit integer value.
         */
        explicit Token(const s32 intValue) CMM_NOEXCEPT;

        /**
         * Constructor initialized with the text to be tokenized.
         *
         * @param doubleValue the 64-bit double value.
         */
        explicit Token(const f64 doubleValue) CMM_NOEXCEPT;

        /**
         * Constructor initialized with the text to be tokenized.
         *
         * @param str the character string.
         * @param isSymbol whether this is a c-style(false) string or symbol (true).
         */
        explicit Token(const std::string& str, const bool isSymbol);

        /**
         * Constructor initialized with the text to be tokenized.
         *
         * @param str the character string.
         * @param isSymbol whether this is a c-style(false) string or symbol (true).
         */
        explicit Token(std::string&& str, const bool isSymbol);

        /**
         * Default copy constructor.
         */
        Token(const Token&);

        /**
         * Default move constructor.
         */
        Token(Token&&) CMM_NOEXCEPT;

        /**
         * Default destructor.
         */
        ~Token();

        /**
         * Default copy assignment operator.
         */
        Token& operator= (const Token&);

        /**
         * Default move assignment operator.
         */
        Token& operator= (Token&&) CMM_NOEXCEPT;

        /**
         * Gets the TokenType.
         *
         * @return TokenType.
         */
        TokenType getType() const CMM_NOEXCEPT;

        /**
         * Gets the value as a bool.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a char (valid or not).
         *
         * @return bool.
         */
        bool asBool() const CMM_NOEXCEPT;

        /**
         * Gets whether the token is a bool.
         *
         * @return bool.
         */
        bool isBool() const CMM_NOEXCEPT;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param b the bool value to set.
         */
        void setBool(const bool b) CMM_NOEXCEPT;

        /**
         * Gets the value as a char.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a char (valid or not).
         *
         * @return char.
         */
        char asChar() const CMM_NOEXCEPT;

        /**
         * Gets whether the token is a char.
         *
         * @return bool.
         */
        bool isChar() const CMM_NOEXCEPT;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param ch the char to set.
         */
        void setChar(const char ch) CMM_NOEXCEPT;

        /**
         * Gets the value as a double.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a double (valid or not).
         *
         * @return f64.
         */
        f64 asDouble() const CMM_NOEXCEPT;

        /**
         * Gets whether the token is a double.
         *
         * @return bool.
         */
        bool isDouble() const CMM_NOEXCEPT;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param doubleValue the double to set.
         */
        void setDouble(const f64 doubleValue) CMM_NOEXCEPT;

        /**
         * Gets the value as a float.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a double (valid or not).
         *
         * @return f32.
         */
        f32 asFloat() const CMM_NOEXCEPT;

        /**
         * Gets whether the token is a float.
         *
         * @return bool.
         */
        bool isFloat() const CMM_NOEXCEPT;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param floatValue the double to set.
         */
        void setFloat(const f32 floatValue) CMM_NOEXCEPT;

        /**
         * Gets the value as a int16.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a int16 (valid or not).
         *
         * @return f64.
         */
        s16 asInt16() const CMM_NOEXCEPT;

        /**
         * Gets whether the token is a int16.
         *
         * @return bool.
         */
        bool isInt16() const CMM_NOEXCEPT;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param doubleValue the int16 to set.
         */
        void setInt16(const s16 intValue) CMM_NOEXCEPT;

        /**
         * Gets the value as a int32.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a int32 (valid or not).
         *
         * @return f64.
         */
        s32 asInt32() const CMM_NOEXCEPT;

        /**
         * Gets whether the token is a int32.
         *
         * @return bool.
         */
        bool isInt32() const CMM_NOEXCEPT;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param doubleValue the int32 to set.
         */
        void setInt32(const s32 intValue) CMM_NOEXCEPT;

        /**
         * Gets the value as a int64.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a int64 (valid or not).
         *
         * @return f64.
         */
        s64 asInt64() const CMM_NOEXCEPT;

        /**
         * Gets whether the token is a int64.
         *
         * @return bool.
         */
        bool isInt64() const CMM_NOEXCEPT;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param doubleValue the int64 to set.
         */
        void setInt64(const s64 intValue) CMM_NOEXCEPT;

        /**
         * Gets whether the token is a null value.
         *
         * @return bool.
         */
        bool isNull() const CMM_NOEXCEPT;

        /**
         * Sets the underlying value to null.
         */
        void setNull() CMM_NOEXCEPT;

        /**
         * Gets the value as a char.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a char (valid or not).
         *
         * @return std::string reference.
         */
        std::string& asCString() CMM_NOEXCEPT;

        /**
         * Gets the value as a char.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a char (valid or not).
         *
         * @return const std::string reference.
         */
        const std::string& asCString() const CMM_NOEXCEPT;

        /**
         * Gets whether the token is a std::string.
         *
         * @return bool.
         */
        bool isCString() const CMM_NOEXCEPT;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param str the std::string to set.
         */
        void setCString(const std::string& str);

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param str the std::string to set.
         */
        void setCString(std::string&& str);

        /**
         * Gets the value as a char symbol.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a char symbol, that also happens to be a char (valid or not).
         *
         * @return char.
         */
        char asCharSymbol() const CMM_NOEXCEPT;

        /**
         * Gets whether the token is a char symbol.
         *
         * @return bool.
         */
        bool isCharSymbol() const CMM_NOEXCEPT;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param char symbol the char to set.
         */
        void setCharSymbol(const char charSymbol) CMM_NOEXCEPT;

        /**
         * Gets the value as a std::string symbol.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a char symbol, that also happens to be a char (valid or not).
         *
         * @return std::string.
         */
        std::string& asStringSymbol() CMM_NOEXCEPT;

        /**
         * Gets the value as a std::string symbol.
         * Note: the caller should check against the TokenType before calling
         * this as the result is the raw value as a char symbol, that also happens to be a char (valid or not).
         *
         * @return std::string.
         */
        const std::string& asStringSymbol() const CMM_NOEXCEPT;

        /**
         * Gets whether the token is a string symbol.
         *
         * @return bool.
         */
        bool isStringSymbol() const CMM_NOEXCEPT;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param string symbol the char to set.
         */
        void setStringSymbol(const std::string& stringSymbol) CMM_NOEXCEPT;

        /**
         * Sets the underlying value to the passed value and updates the TokenType.
         *
         * @param string symbol the char to set.
         */
        void setStringSymbol(std::string&& stringSymbol) CMM_NOEXCEPT;

        /**
         * Equality operator
         * 
         * @param other const reference to another Token to be compared.
         * @return bool.
         */
        bool operator== (const Token& other) const;

        /**
         * In-equality operator
         * 
         * @param other const reference to another Token to be compared.
         * @return bool.
         */
        bool operator!= (const Token& other) const;

        /**
         * Attempts to stringify the token.
         *
         * @return std::string.
         */
        std::string toString() const;

    private:

        /**
         * Performs logic to conditionally clean our value str (i.e. TokenType::SYMBOL or TokenType::STRING).
         */
        void conditionallyCleanString() CMM_NOEXCEPT;

        /**
         * Gets whether this is an std::string (i.e. TokenType::SYMBOL or TokenType::STRING).
         *
         * @return bool.
         */
        bool isCStringOrStringSymbol() const CMM_NOEXCEPT;

    private:
        union Values
        {
            bool b;
            char ch;
            f64 doubleValue;
            f32 floatValue;
            s16 int16Value;
            s32 int32Value;
            s64 int64Value;
            std::string* str;
            char symbol;
        };

        // The type of the token
        TokenType type;

        // The underlying token value.
        Values value;
    };

    struct TokenHasher
    {
        std::size_t operator() (const Token& token) const;
    };

    struct TokenTypeHasher
    {
        std::size_t operator() (const TokenType& type) const;
    };
}

#endif //!CMM_TOKEN_H

