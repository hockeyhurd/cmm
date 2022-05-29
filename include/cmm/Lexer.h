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
        Lexer(std::string&& text) noexcept;
        Lexer(const Lexer&) = default;
        Lexer(Lexer&&) noexcept = default;
        ~Lexer() = default;

        Lexer& operator= (const Lexer&) = default;
        Lexer& operator= (Lexer&&) noexcept = default;

        /**
         * Gets the current location.
         *
         * @return Location
         */
        Location getLocation() const noexcept;

        /**
         * Gets whether we have reached end of input or EOF.
         *
         * @return bool true if completed, else false.
         */
        bool completed() const noexcept;

        /**
         * Gets whether we have reached end of input or EOF,
         * or the remaining characters are just whitespace.
         *
         * @return bool true if completed, else false.
         */
        bool completedOrWhitespaceOnly() noexcept;

        bool nextToken(Token& token, std::string* errorMessage = nullptr);
        bool peekNextToken(Token& token);

    private:
        void consumeWhitespace();
        char nextChar() noexcept;
        char peekNextChar() const noexcept;
        bool nextTokenInternal(Token& token, std::string* errorMessage = nullptr);
        void restore(const Snapshot& snap) noexcept;

        static bool isEscape(char first, char second) noexcept;

        /**
         * Checks whether this char should be escaped.
         *
         * @param ch char to check.
         * @return bool.
         */
        static bool requiresEscape(char ch) noexcept;

        /**
         * Transforms the string sequence to its character representation.
         *
         * @param first char in sequence.
         * @param second char in sequence.
         * @return char.
         */
        static char transformEscapeSequence(char first, char second) noexcept;

    private:
        std::string text;
        std::size_t index;
        Location location;
        std::string builder;
    };
}

#endif //!CMM_LEXER_H

