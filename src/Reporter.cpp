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
    Reporter::Reporter() CMM_NOEXCEPT : errors(0), warnings(0), canPrint(true)
    {
    }

    Reporter::~Reporter()
    {
        if (canPrint)
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
    }

    /* static */
    Reporter& Reporter::instance()
    {
        static Reporter inst;
        return inst;
    }

    s32 Reporter::getErrorCount() const CMM_NOEXCEPT
    {
        return errors;
    }

    s32 Reporter::getWarningCount() const CMM_NOEXCEPT
    {
        return warnings;
    }

    void Reporter::setEnablePrint(const bool enable) CMM_NOEXCEPT
    {
        this->canPrint = enable;
    }

    void Reporter::reset()
    {
        errors = 0;
        warnings = 0;
    }
}

