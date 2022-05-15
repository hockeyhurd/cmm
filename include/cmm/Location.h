/**
 * The JSON parser.
 *
 * @author hockeyhurd
 * @version 2022-05-06
 */

#pragma once

#ifndef HSON_LOCATION_H
#define HSON_LOCATION_H

// Our includes
#include <cmm/Types.h>

// std includes
#include <iosfwd>
#include <string>

namespace cmm
{
    class Lexer;

    class Location
    {
        friend class Lexer;

    public:

        /**
         * Default constructor
         */
        Location() noexcept;

        /**
         * Constructor
         *
         * @param line the line in the file
         * @param pos the position offset from the specified line.
         */
        Location(const std::size_t line, const std::size_t pos) noexcept;

        /**
         * Default copy constructor.
         */
        Location(const Location&) noexcept = default;

        /**
         * Default move constructor.
         */
        Location(Location&&) noexcept = default;

        /**
         * Default destructor
         */
        ~Location() = default;

        /**
         * Default copy assignment operator.
         *
         * @return this Location's instance.
         */
        Location& operator= (const Location&) noexcept = default;

        /**
         * Default move assignment operator.
         *
         * @return this Location's instance.
         */
        Location& operator= (Location&&) noexcept = default;

        /**
         * Gets the line in the file.
         *
         * @return std::size_t line.
         */
        inline std::size_t getLine() const noexcept
        {
            return line;
        }

        /**
         * Gets the position in the file.
         *
         * @return std::size_t position.
         */
        inline std::size_t getPosition() const noexcept
        {
            return pos;
        }

        /**
         * Converts this object to a std::string format.
         *
         * @return std::string.
         */
        std::string toString() const;

    private:
        std::size_t line;
        std::size_t pos;
    };
}

std::ostream& operator<< (std::ostream& os, const cmm::Location& location);

#endif //!HSON_LOCATION_H

