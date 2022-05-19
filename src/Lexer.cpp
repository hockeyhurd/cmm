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

// std includes
#include <optional>
#include <sstream>

namespace cmm
{
    static std::optional<f64> validateNumber(const std::string& str)
    {
        std::size_t parsedCount = 0;
        const f64 value = std::stod(str, &parsedCount);
        return parsedCount > 0 ? std::make_optional(value) : std::optional<f64>();
    }

    Lexer::Lexer(const std::string& text) : text(text), index(0)
    {
        builder.reserve(0x40);
    }

    Lexer::Lexer(std::string&& text) noexcept : text(std::move(text)), index(0)
    {
        builder.reserve(0x40);
    }

    Location Lexer::getLocation() const noexcept
    {
        return location;
    }

    bool Lexer::completed() const noexcept
    {
        return index == text.size();
    }

    bool Lexer::completedOrWhitespaceOnly() noexcept
    {
        // If already completed, early exit.
        if (completed())
            return true;

        Snapshot snap(index);
        consumeWhitespace();

        const bool result = completed();

        // Restore incase we might try to do more in the future.
        restore(snap);

        return result;
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

    char Lexer::nextChar() noexcept
    {
        if (index < text.size())
        {
            const auto ch = text[index++];

            switch (ch)
            {
            case CHAR_NEWLINE:
            case CHAR_CARRIAGE_RETURN:
                ++location.line;
                location.pos = 1;
                break;
            default:
                ++location.pos;
                break;
            }

            return ch;
        }

        return CHAR_EOF;
    }

    char Lexer::peekNextChar() const noexcept
    {
        if (index < text.size())
            return text[index];
        return CHAR_EOF;
    }

    bool Lexer::nextTokenInternal(Token& token)
    {
        // Always clear the buffer when lexing the next token.
        builder.clear();

        // Always start by consuming any 'dead' whitespace.
        consumeWhitespace();

        while (index < text.size())
        {
            auto currentChar = nextChar();

            switch (currentChar)
            {
            case CHAR_BACK_SLASH:
                continue;
            case CHAR_DOUBLE_QOUTE:
            {
                // TODO: Support unicode
                bool lastWasEscaped = false;
                const char* start = &text[index];

                do
                {
                    // TODO: Might be able to remove this, at least for this loop.
                    // Note: Leaving for now...
                    builder += currentChar;
                    lastWasEscaped = !lastWasEscaped && currentChar == CHAR_BACK_SLASH;
                    currentChar = nextChar();
                }
                while (index < text.size() && (lastWasEscaped || currentChar != CHAR_DOUBLE_QOUTE));

                if (currentChar == CHAR_DOUBLE_QOUTE)
                {
                    const char* end = &text[index - 1];
                    token.setCString(StringView(start, end));
                    return true;
                }
            }
                break;
            case CHAR_SINGLE_QOUTE:
            {
                // TODO: Support unicode
                builder += currentChar;
                currentChar = nextChar();
                char expectedResult = currentChar;
                builder += currentChar;

                if (currentChar == CHAR_SINGLE_QOUTE)
                {
                    // TODO: Do nothing or return false??
                    // return false;
                }

                else
                {
                    if (currentChar == CHAR_BACK_SLASH)
                    {
                        // Char to decode
                        // TODO: Implement
                        // expectedResult = <decoded value>
                    }

                    else
                    {
                        expectedResult = currentChar;
                    }

                    currentChar = nextChar();
                    builder += currentChar;

                    // Now expect close CHAR_SINGLE_QOUTE
                    if (currentChar == CHAR_SINGLE_QOUTE)
                    {
                        token.setChar(expectedResult);
                        return true;
                    }
                }
            }
                break;
            case 'f':
            {
                currentChar = nextChar();
                if (currentChar != 'a')
                    return false;

                currentChar = nextChar();
                if (currentChar != 'l')
                    return false;

                currentChar = nextChar();
                if (currentChar != 's')
                    return false;

                currentChar = nextChar();
                if (currentChar != 'e')
                    return false;

                token.setBool(false);
                return true;
            }
                break;
            case 't':
            {
                currentChar = nextChar();
                if (currentChar != 'r')
                    return false;

                currentChar = nextChar();

                if (currentChar != 'u')
                    return false;

                currentChar = nextChar();
                if (currentChar != 'e')
                    return false;

                token.setBool(true);
                return true;
            }
                break;
            case 'n':
            {
                currentChar = nextChar();

                if (currentChar != 'u')
                    return false;

                currentChar = nextChar();
                if (currentChar != 'l')
                    return false;

                currentChar = nextChar();
                if (currentChar != 'l')
                    return false;

                token.setNull();
            }
                break;

            case CHAR_MINUS:
            case CHAR_PLUS:
            {
                builder += currentChar;
                currentChar = nextChar();
            }
                // fallthrough
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            {
                bool seenDot = false;
                bool seenE = false;

                do
                {
                    builder += currentChar;
                    const auto nextCh = peekNextChar();

                    if (std::isdigit(nextCh))
                    {
                        currentChar = nextChar();
                    }

                    else if (nextCh == CHAR_PERIOD)
                    {
                        if (!seenDot && !seenE)
                        {
                            seenDot = true;
                            currentChar = nextChar();
                        }

                        else if (!seenDot && seenE)
                        {
                            std::ostringstream err;
                            err << "[LEXER]: Lexing a decimal number that contained '.' after using 'E' or 'e' at "
                                    << location.toString();
                            throw std::runtime_error(err.str());
                        }

                        else
                        {
                            std::ostringstream err;
                            err << "[LEXER]: Lexing a decimal number that contained multiple '.' in a double value at "
                                    << location.toString();
                            throw std::runtime_error(err.str());
                        }
                    }

                    else if (nextCh == 'e' || nextCh == 'E')
                    {
                        if (!seenE)
                        {
                            seenE = true;
                            currentChar = nextChar();
                        }

                        else
                        {
                            std::ostringstream err;
                            err << "[LEXER]: Lexing a decimal number that contained multiple 'e' or 'E' in a double value at "
                                    << location.toString();
                            throw std::runtime_error(err.str());
                        }
                    }

                    else
                    {
                        break;
                    }
                }
                while (true);

                const auto optionalF64 = validateNumber(builder);

                if (optionalF64.has_value())
                {
                    token.setDouble(*optionalF64);
                    return true;
                }

                else
                {
                    return false;
                }
            }
                break;
            case CHAR_LCURLY_BRACKET:
            case CHAR_RCURLY_BRACKET:
            case CHAR_LPAREN:
            case CHAR_RPAREN:
            case CHAR_LSQUARE_BRACKET:
            case CHAR_RSQUARE_BRACKET:
            case CHAR_COLON:
            case CHAR_COMMA:
                token.setSymbol(currentChar);
                return true;
            case CHAR_EOF:
                return false;
            default:
            {
                std::ostringstream err;
                err << "[LEXER]: Unexpected token exception '" << builder
                    << "\" at " << location.toString();
                throw std::runtime_error(err.str());
            }
            }
        }

        return false;
    }

    void Lexer::restore(const Snapshot& snap) noexcept
    {
        index = snap.getPosition();
    }
}

