/**
 * The lexer to our parser.
 *
 * @author hockeyhurd
 * @version 2022-05-13
 */

#pragma once

#ifndef CMM_LEXER_H
#define CMM_LEXER_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Location.h>

// std includes
#include <string>

namespace cmm
{
    class Snapshot;
    class Token;

    class Lexer
    {
    public:
        Lexer(const std::string& text);
        Lexer(std::string&& text) CMM_NOEXCEPT;
        Lexer(const Lexer&) = default;
        Lexer(Lexer&&) CMM_NOEXCEPT = default;
        ~Lexer() = default;

        Lexer& operator= (const Lexer&) = default;
        Lexer& operator= (Lexer&&) CMM_NOEXCEPT = default;

        /**
         * Gets the current location.
         *
         * @return Location
         */
        Location getLocation() const CMM_NOEXCEPT;

        /**
         * Gets whether we have reached end of input or EOF.
         *
         * @return bool true if completed, else false.
         */
        bool completed() const CMM_NOEXCEPT;

        /**
         * Gets whether we have reached end of input or EOF,
         * or the remaining characters are just whitespace.
         *
         * @return bool true if completed, else false.
         */
        bool completedOrWhitespaceOnly() CMM_NOEXCEPT;

        bool nextToken(Token& token, std::string* errorMessage = nullptr);
        bool peekNextToken(Token& token);

        void restore(const Snapshot& snap) CMM_NOEXCEPT;
        Snapshot snap() CMM_NOEXCEPT;

    private:
        void consumeWhitespace();
        char nextChar() CMM_NOEXCEPT;
        char peekNextChar() const CMM_NOEXCEPT;
        bool nextTokenInternal(Token& token, std::string* errorMessage = nullptr);

        /**
         * Checks whether the passed character is a alhpabetic or not.
         *
         * @param ch char to check.
         * @return bool.
         */
        static bool isAlpha(char ch) CMM_NOEXCEPT;

        /**
         * Checks whether the passed character is a digit or not.
         *
         * @param ch char to check.
         * @return bool.
         */
        static bool isDigit(char ch) CMM_NOEXCEPT;

        /**
         * Checks whether the passed character is an escape character or not.
         *
         * @param first char to check.
         * @param second char to check.
         * @return bool.
         */
        static bool isEscape(char first, char second) CMM_NOEXCEPT;

        /**
         * Checks whether this char should be escaped.
         *
         * @param ch char to check.
         * @return bool.
         */
        static bool requiresEscape(char ch) CMM_NOEXCEPT;

        /**
         * Transforms the string sequence to its character representation.
         *
         * @param first char in sequence.
         * @param second char in sequence.
         * @return char.
         */
        static char transformEscapeSequence(char first, char second) CMM_NOEXCEPT;

        /**
         * Checks whether the passed character is a newline charactger or not.
         *
         * @param ch char to check.
         * @return bool.
         */
        static bool isNewLine(char ch) CMM_NOEXCEPT;

        /**
         * Checks whether the passed character is whitespace or not.
         *
         * @param ch char to check.
         * @return bool.
         */
        static bool isWhitespace(char ch) CMM_NOEXCEPT;

    private:
        std::string text;
        std::size_t index;
        Location location;
        std::string builder;
    };
}

#endif //!CMM_LEXER_H

