/**
 * A base visitor class implementing the visitor pattern.
 *
 * @author hockeyhurd
 * @version 2022-08-24
 */

// Our includes
#include <cmm/Visitor.h>

// std includes
#include <sstream>
#include <string>
#include <iostream>

#define UNIMPLEMENTED_EXCEPTION() unimplementedException(__FILE__, __LINE__)

namespace cmm
{
    /* static */
    template<class T>
    void Visitor<T>::unimplementedException(const char* fileName, const std::size_t line)
    {
        std::ostringstream os;
        os << "[BUG]: Unimplemented exception thrown at " << fileName << ": " << line;

        const auto message = os.str();
        std::cerr << message << std::endl;

        throw std::runtime_error(message);
    }
}

#undef UNIMPLEMENTED_EXCEPTION

