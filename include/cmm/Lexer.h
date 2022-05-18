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

        bool nextToken(Token& token);
        bool peekNextToken(Token& token);

    private:
        void consumeWhitespace();
        char nextChar() noexcept;
        char peekNextChar() const noexcept;
        bool nextTokenInternal(Token& token);
        void restore(const Snapshot& snap) noexcept;

    private:
        std::string text;
        std::size_t index;
        Location location;
        std::string builder;
    };
}

#endif //!CMM_LEXER_H

