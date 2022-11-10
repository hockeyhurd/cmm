/**
 * Location: This class represents a Location in a file or std::string.
 * LocationPair: This class represents a pair of Locations denoted by
 *               'begin' and 'end'.
 *
 * @author hockeyhurd
 * @version 2022-05-06
 */

// Our includes
#include <cmm/Location.h>

// std includes
#include <sstream> 

namespace cmm
{
    // Location class starts here:

    Location::Location() CMM_NOEXCEPT : line(0), pos(0)
    {
    }

    Location::Location(const std::size_t line, const std::size_t pos) CMM_NOEXCEPT : line(line), pos(pos)
    {
    }

    std::string Location::toString() const
    {
        std::ostringstream os;
        os << '(' << line << ", " << pos << ')';

        return os.str();
    }

    // Location class ends here:

    // LocationPair class starts here:

    LocationPair::LocationPair(const Location& begin, const Location& end) CMM_NOEXCEPT : begin(begin), end(end)
    {
    }

    LocationPair::LocationPair(Location&& begin, Location&& end) CMM_NOEXCEPT : begin(std::move(begin)), end(std::move(end))
    {
    }

    // Location class ends here:
}

std::ostream& operator<< (std::ostream& os, const cmm::Location& location)
{
    os << '(' << location.getLine() << ", " << location.getPosition() << ')';
    return os;
}

std::ostream& operator<< (std::ostream& os, const cmm::LocationPair& pair)
{
    os << "{ Begin: " << pair.begin << ", End: " << pair.end << " }";
    return os;
}

