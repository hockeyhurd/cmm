/**
 * The lexer to our parser.
 *
 * @author hockeyhurd
 * @version 2022-05-13
 */

// Our includes
#include <cmm/Lexer.h>
#include <cmm/Reporter.h>
#include <cmm/Snapshot.h>
#include <cmm/Token.h>

// std includes
#include <limits>
#include <optional>
#include <sstream>

// GNU GMP incldues for non-Windows platforms
#if !OS_WIN
#include <gmpxx.h>
#else
#include <cstring>
#endif

namespace cmm
{
    static std::optional<s32> validateInt32(const std::string& str)
    {
#if !OS_WIN
        mpz_class value(str);
        return value.fits_sint_p() ? std::make_optional<s32>(value.get_si()) : std::nullopt;
#else
        std::size_t parseCount = 0;
        const s32 value = std::stoi(str, &parseCount);
        return parseCount > 0 ? std::make_optional<s32>(value) : std::nullopt;
#endif
    }

    static std::optional<f64> validateDouble(const std::string& str)
    {
#if !OS_WIN
        mpf_class value(str);
        return value ? std::make_optional<f64>(value.get_d()) : std::nullopt;
#else
        std::size_t parseCount = 0;
        const f64 value = std::stod(str, &parseCount);
        return parseCount > 0 ? std::make_optional<f64>(value) : std::nullopt;
#endif
    }

    static std::optional<f32> validateFloat(const std::string& str)
    {
#if !OS_WIN
        const auto doubleResult = validateDouble(str);
        return doubleResult.has_value() && (f64) std::numeric_limits<f32>::lowest() <= *doubleResult && *doubleResult <= (f64) std::numeric_limits<f32>::max() ?
               std::make_optional(static_cast<f32>(*doubleResult)) : std::nullopt;
#else
        std::size_t parseCount = 0;
        const f32 value = std::stof(str, &parseCount);
        return parseCount > 0 ? std::make_optional<f32>(value) : std::nullopt;
#endif
    }

    Lexer::Lexer(const std::string& text) : text(text), index(0), location(1, 0)
    {
        builder.reserve(0x40);
    }

    Lexer::Lexer(std::string&& text) CMM_NOEXCEPT : text(std::move(text)), index(0)
    {
        builder.reserve(0x40);
    }

    Location Lexer::getLocation() const CMM_NOEXCEPT
    {
        return location;
    }

    bool Lexer::completed() const CMM_NOEXCEPT
    {
        return index == text.size();
    }

    bool Lexer::completedOrWhitespaceOnly() CMM_NOEXCEPT
    {
        // If already completed, early exit.
        if (completed())
            return true;

        Snapshot snapshot = snap();
        consumeWhitespace();

        const bool result = completed();

        // Restore incase we might try to do more in the future.
        restore(snapshot);

        return result;
    }

    bool Lexer::nextToken(Token& token, std::string* errorMessage, Location* pLocation)
    {
        const bool result = nextTokenInternal(token, errorMessage, pLocation);
        return result;
    }

    bool Lexer::peekNextToken(Token& token)
    {
        Snapshot snapshot = snap();
        const bool result = nextToken(token, nullptr, nullptr);
        restore(snapshot);

        return result;
    }

    void Lexer::restore(const Snapshot& snap) CMM_NOEXCEPT
    {
        index = snap.getIndex();
        location = snap.getLocation();
    }

    Snapshot Lexer::snap() CMM_NOEXCEPT
    {
        return Snapshot(index, location);
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
            ++location.pos;
        }
    }

    char Lexer::nextChar() CMM_NOEXCEPT
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

    char Lexer::peekNextChar() const CMM_NOEXCEPT
    {
        if (index < text.size())
            return text[index];
        return CHAR_EOF;
    }

    bool Lexer::nextTokenInternal(Token& token, std::string* errorMessage, Location* pLocation)
    {
        static Reporter& reporter = Reporter::instance();

        // Always clear the buffer when lexing the next token.
        builder.clear();

        // Always start by consuming any 'dead' whitespace.
        consumeWhitespace();

        if (pLocation != nullptr)
        {
            *pLocation = getLocation();
        }

        while (index < text.size())
        {
            auto snapshot = snap();
            auto currentChar = nextChar();

            switch (currentChar)
            {
            case CHAR_BACK_SLASH:
                continue;
            case CHAR_FORWARD_SLASH:
            {
                const auto nextCh = peekNextChar();

                // Seek to end of comment.
                if (nextCh == CHAR_FORWARD_SLASH)
                {
                    currentChar = nextChar();
                    while (index < text.size() && !isNewLine(currentChar))
                    {
                        currentChar = nextChar();
                    }

                    currentChar = nextChar();
                    continue;
                }

                // Not a comment
                token.setCharSymbol(currentChar);
                return true;
            }
                break;
            case CHAR_DOUBLE_QOUTE:
            {
                bool lastWasEscape = false;
                char lastChar = currentChar;

                // Get rid of the leading '"'
                currentChar = nextChar();

                // For our actual sequence of characters after handling escape codes.
                std::string sequence;

                do
                {
                    // TODO: Might be able to remove this, at least for this loop.
                    // Note: Leaving for now...
                    builder += currentChar;

                    if (lastWasEscape)
                    {
                        if (requiresEscape(currentChar))
                        {
                            const char escapedChar = transformEscapeSequence(lastChar, currentChar);
                            sequence += escapedChar;
                        }

                        else
                        {
                            if (errorMessage != nullptr)
                            {
                                *errorMessage = "Last character was escaped, but this character does not need to be";
                                reporter.error(*errorMessage, location);
                            }
                        }
                    }

                    else
                    {
                        sequence += currentChar;
                    }

                    lastWasEscape = !lastWasEscape && (currentChar == CHAR_BACK_SLASH || currentChar == CHAR_DOUBLE_QOUTE);
                    lastChar = currentChar;
                    currentChar = nextChar();
                }
                while (index < text.size() && ((lastWasEscape) ^ (currentChar != CHAR_DOUBLE_QOUTE)));

                if (lastWasEscape)
                {
                    if (errorMessage != nullptr)
                    {
                        *errorMessage = "Unfinished escape sequences";
                        reporter.error(*errorMessage, location);
                    }
                }

                else if (currentChar == CHAR_DOUBLE_QOUTE)
                {
                    token.setCString(std::move(sequence));
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
            case CHAR_MINUS:
            case CHAR_PLUS:
            {
                const char lookaheadChar = peekNextChar();

                if (isWhitespace(lookaheadChar) ||
                   (!isDigit(lookaheadChar) && lookaheadChar != CHAR_PERIOD))
                {
                    if (currentChar == CHAR_PLUS && lookaheadChar == CHAR_PLUS)
                    {
                        nextChar();
                        token.setStringSymbol("++");
                    }

                    else if (currentChar == CHAR_MINUS && lookaheadChar == CHAR_MINUS)
                    {
                        nextChar();
                        token.setStringSymbol("--");
                    }

                    else if (currentChar == CHAR_MINUS && lookaheadChar == CHAR_GT)
                    {
                        nextChar();
                        token.setStringSymbol("->");
                    }

                    else
                    {
                        token.setCharSymbol(currentChar);
                    }

                    return true;
                }
            }
                // fallthrough
            case CHAR_PERIOD: case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            {
                bool seenDot = false;
                bool seenE = false;
                bool seenF = false;

                do
                {
                    builder += currentChar;
                    auto nextCh = peekNextChar();

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
                            if (errorMessage != nullptr)
                            {
                                *errorMessage = "Lexing a decimal number that contained '.' after using 'E' or 'e'";
                                reporter.error(*errorMessage, location);
                            }

                            return false;
                        }

                        else
                        {
                            if (errorMessage != nullptr)
                            {
                                *errorMessage = "Lexing a decimal number that contained multiple '.' in a double value";
                                reporter.error(*errorMessage, location);
                            }

                            return false;
                        }
                    }

                    else if (nextCh == 'e' || nextCh == 'E')
                    {
                        if (!seenE)
                        {
                            seenE = true;
                            currentChar = nextChar();
                            nextCh = peekNextChar();

                            if (isWhitespace(nextCh))
                            {
                                if (errorMessage != nullptr)
                                {
                                    *errorMessage = "Lexing a decimal number that contained whitespace after 'e' or 'E'";
                                    reporter.error(*errorMessage, location);
                                }

                                return false;
                            }

                            else if (nextCh != CHAR_PLUS && nextCh != CHAR_MINUS &&
                                     nextCh != CHAR_PERIOD &&
                                     !isDigit(nextCh))
                            {
                                if (errorMessage != nullptr)
                                {
                                    std::ostringstream err;
                                    err << "[LEXER]: Error: Invalid character after using 'e' or 'E' " << nextCh;
                                    *errorMessage = err.str();
                                    reporter.error(*errorMessage, location);
                                }

                                return false;
                            }
                        }

                        else
                        {
                            if (errorMessage != nullptr)
                            {
                                *errorMessage = "Lexing a decimal number that contained multiple 'e' or 'E' in a double value";
                                reporter.error(*errorMessage, location);
                            }

                            return false;
                        }
                    }

                    else if (nextCh == 'f' || nextCh == 'F')
                    {
                        seenF = true;
                        currentChar = nextChar();
                        nextCh = peekNextChar();

                        break;
                    }

                    else
                    {
                        break;
                    }
                }
                while (true);

                // Handle trivial case where the case statement was simply a '.'.
                if (builder == ".")
                {
                    token.setCharSymbol(CHAR_PERIOD);
                    return true;
                }

                else if (seenDot && builder.back() == CHAR_PERIOD)
                {
                    // Try to see what was before the "." to see if we have the case "+." or "-."
                    // instead of say "1."
                    // const auto chAtLastSnap = text[snapshot.getPosition()];
                    const auto chAtLastSnap = text.at(index - 2);

                    if (chAtLastSnap == CHAR_PLUS || chAtLastSnap == CHAR_MINUS)
                    {
                        // Accept the snapshot and recover, which is essentially
                        // the same as the (index - 2) lookup.
                        restore(snapshot);

                        // Move one past this symbol
                        currentChar = nextChar();

                        // Set the token result.
                        token.setCharSymbol(chAtLastSnap);

                        return true;
                    }

                    // Unexpected character
                    else if (!isDigit(chAtLastSnap))
                    {
                        return false;
                    }

                    // Could still be something like "1.", so don't return false quite yet...
                }

                if (!seenDot && !seenE && !seenF)
                {
                    const auto optionalS32 = validateInt32(builder);

                    if (optionalS32.has_value())
                    {
                        token.setInt32(*optionalS32);
                        return true;
                    }

                    return false;
                }

                else if (seenF)
                {
                    const auto optionalF32 = validateFloat(builder);

                    if (optionalF32.has_value())
                    {
                        token.setFloat(*optionalF32);
                        return true;
                    }

                    return false;
                }

                else
                {
                    const auto optionalF64 = validateDouble(builder);

                    if (optionalF64.has_value())
                    {
                        token.setDouble(*optionalF64);
                        return true;
                    }

                    return false;
                }
            }
                break;
            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
            case 'H': case 'I': case 'J': case 'K': case 'L': case 'M':
            case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
            case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
            case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
            case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
            case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': case '_':
            {
                builder += currentChar;
                auto lookaheadChar = peekNextChar();

                while (index < text.size() && (isAlpha(lookaheadChar)
                       || isDigit(lookaheadChar) || lookaheadChar == CHAR_UNDERSCORE))
                {
                    currentChar = nextChar();
                    builder += currentChar;
                    lookaheadChar = peekNextChar();
                }

                if (builder == "true")
                {
                    token.setBool(true);
                }

                else if (builder == "false")
                {
                    token.setBool(false);
                }

                else if (builder == "NULL")
                {
                    token.setNull();
                }

                else
                {
                    token.setStringSymbol(builder);
                }

                return true;
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
            case CHAR_SEMI_COLON:
            case CHAR_AMPERSAND:
            case CHAR_ASTERISK:
            case CHAR_GT:
            case CHAR_LT:
            case CHAR_EQUALS:
                token.setCharSymbol(currentChar);
                return true;
            case CHAR_EOF:
                return false;
            default:
            {
                if (errorMessage != nullptr)
                {
                    std::ostringstream err;
                    err << "Unexpected token exception '" << builder << "\"";
                    *errorMessage = err.str();
                    reporter.error(*errorMessage, location);
                }
            }
            }
        }

        return false;
    }

    /* static */
    bool Lexer::isAlpha(char ch) CMM_NOEXCEPT
    {
        return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
    }

    /* static */
    bool Lexer::isDigit(char ch) CMM_NOEXCEPT
    {
        return ch >= '0' && ch <= '9';
    }

    /* static */
    bool Lexer::isEscape(char first, char second) CMM_NOEXCEPT
    {
        if (first != '\\')
        {
            return false;
        }

        switch (second)
        {
        case 'b':
        case 'f':
        case 'n':
        case 'r':
        case 't':
        case CHAR_DOUBLE_QOUTE:
        case CHAR_BACK_SLASH:
            return true;
        default:
            return false;
        }

        // Should never be reached, but included to prevent some warnings in some compilers.
        return false;
    }

    /* static */
    bool Lexer::requiresEscape(char ch) CMM_NOEXCEPT
    {
        return isEscape(CHAR_BACK_SLASH, ch);
    }

    /* static */
    char Lexer::transformEscapeSequence(char first, char second) CMM_NOEXCEPT
    {
        if (first != CHAR_BACK_SLASH)
        {
            return EOF;
        }

        switch (second)
        {
        case 'b':
            return CHAR_BACK_SPACE;
        case 'f':
            return CHAR_FORM_FEED;
        case 'n':
            return CHAR_NEWLINE;
        case 'r':
            return CHAR_CARRIAGE_RETURN;
        case 't':
            return CHAR_TAB;
        case CHAR_DOUBLE_QOUTE:
            return CHAR_DOUBLE_QOUTE;
        case CHAR_BACK_SLASH:
            return CHAR_BACK_SLASH;
        default:
            return CHAR_EOF;
        }

        // Should never be reached, but included to prevent some warnings in some compilers.
        return CHAR_EOF;
    }

    /* static */
    bool Lexer::isNewLine(char ch) CMM_NOEXCEPT
    {
        return ch == CHAR_NEWLINE || ch == CHAR_CARRIAGE_RETURN;
    }

    /* static */
    bool Lexer::isWhitespace(char ch) CMM_NOEXCEPT
    {
        return ch == CHAR_SPACE || ch == CHAR_TAB ||
               ch == CHAR_NEWLINE || ch == CHAR_CARRIAGE_RETURN;
    }
}

