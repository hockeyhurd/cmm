/**
 * The console output for reporting warnings, errors, etc.
 *
 * @author hockeyhurd
 * @version 2022-07-23
 */

// Our includes
#include <cmm/Reporter.h>

namespace cmm
{
    Reporter::Reporter() CMM_NOEXCEPT : errors(0), warnings(0)
    {
    }

    Reporter::~Reporter()
    {
        if (errors > 0)
        {
            std::cout << errors << " errors during compilation\n";
        }

        if (warnings > 0)
        {
            std::cout << warnings << " warnings during compilation\n";
        }
    }

    /* static */
    Reporter& Reporter::instance()
    {
        static Reporter inst;
        return inst;
    }
}

