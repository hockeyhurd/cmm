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
#include <cmm/Lexer.h>

// std includes
#include <memory>
#include <string>

namespace cmm
{
    class CompilationUnitNode;
    class Lexer;

    class Parser
    {
    public:

        /**
         * Constructor
         *
         * @param input to copy
         */
        Parser(const std::string& input);

        /**
         * Constructor
         *
         * @param input to move
         */
        Parser(std::string&& input);

        /**
         * Default copy constructor.
         */
        Parser(const Parser&) = default;

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
        Parser& operator= (const Parser&) = default;

        /**
         * Move assignment operator.
         *
         * @return Parser reference.
         */
        Parser& operator= (Parser&&) noexcept = default;

        /**
         * Attempts to parse the compilation unit.
         *
         * @param errorMessage optional error message to set.  Assumes valid pointer if non-nullptr.
         * @return nullptr on failure, else valid.
         */
        std::shared_ptr<CompilationUnitNode> parseCompilationUnit(std::string* errorMessage);

    private:

        // The lexer to the input text to be parsed.
        Lexer lexer;
    };
}

#endif //!CMM_PARSER_H

