// Our includes
#include <cmm/Location.h>

// std includes
#include <sstream> 

namespace cmm
{
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
}

std::ostream& operator<< (std::ostream& os, const cmm::Location& location)
{
    os << '(' << location.getLine() << ", " << location.getPosition() << ')';
    return os;
}

