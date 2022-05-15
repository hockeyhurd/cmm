/**
 * The lexer to our parser.
 *
 * @author hockeyhurd
 * @version 2022-05-13
 */

// Our includes
#include <cmm/Lexer.h>
#include <cmm/Snapshot.h>
#include <cmm/Token.h>

namespace cmm
{
    Lexer::Lexer(const std::string& text) : text(text), index(0)
    {
    }

    Lexer::Lexer(std::string&& text) : text(std::move(text)), index(0)
    {
    }

    bool Lexer::nextToken(Token& token)
    {
        const bool result = nextTokenInternal(token);
        return result;
    }

    bool Lexer::peekNextToken(Token& token)
    {
        Snapshot snap(index);
        const bool result = nextToken(token);
        restore(snap);

        return result;
    }

    void Lexer::consumeWhitespace()
    {
        while (true)
        {
            // See what the next char is.
            const char currentChar = peekNextChar();

            // If it is NOT considered whitespace, we can't consume so break
            // the loop.
            if (currentChar != CHAR_SPACE && currentChar != CHAR_TAB &&
                currentChar != CHAR_NEWLINE && currentChar != CHAR_CARRIAGE_RETURN)
            {
                break;
            }

            else if (currentChar == CHAR_NEWLINE || currentChar == CHAR_CARRIAGE_RETURN)
            {
                ++location.line;
                location.pos = 1;
            }

            // We know it's whitespace and room to consume the char, so simply
            // increment the index.
            ++index;
        }
    }

    char Lexer::peekNextChar() const noexcept
    {
        if (index < text.size())
            return text[index];
        return CHAR_EOF;
    }

    bool Lexer::nextTokenInternal(Token& token)
    {
        return false;
    }

    void Lexer::restore(const Snapshot& snap) noexcept
    {
        index = snap.getPosition();
    }
}

