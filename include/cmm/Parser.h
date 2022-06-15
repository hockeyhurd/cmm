/**
 * The parser to our compiler.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#pragma once

#ifndef CMM_PARSER_H
#define CMM_PARSER_H

// Our includes
#include <cmm/Types.h>

namespace cmm
{
    class Lexer;

    class Parser
    {
    public:

        /**
         * Default constructor.
         */
        Parser() noexcept = default;

        /**
         * Default copy constructor.
         */
        Parser(const Parser&) noexcept = default;

        /**
         * Default move constructor.
         */
        Parser(Parser&&) noexcept = default;

        /**
         * Destructor
         */
        ~Parser() = default;

        /**
         * Copy assignment operator.
         *
         * @return Parser reference.
         */
        Parser& operator= (const Parser&) noexcept = default;

        /**
         * Move assignment operator.
         *
         * @return Parser reference.
         */
        Parser& operator= (Parser&&) noexcept = default;
    };
}

#endif //!CMM_PARSER_H

