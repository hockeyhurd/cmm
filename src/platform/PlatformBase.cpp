/**
 * A base class for handling platform specific instructions.
 *
 * @author hockeyhurd
 * @version 2022-11-21
 */

// Our includes
#include <cmm/platform/PlatformBase.h>
#include <cmm/NodeList.h>

namespace cmm
{
    PlatformBase::PlatformBase(const std::string& name) : name(name)
    {
    }

    PlatformBase::PlatformBase(std::string&& name) CMM_NOEXCEPT : name(std::move(name))
    {
    }

    /* virtual */
    std::string& PlatformBase::getName() CMM_NOEXCEPT
    {
        return name;
    }

    /* virtual */
    const std::string& PlatformBase::getName() const CMM_NOEXCEPT
    {
        return name;
    }

    /* virtual */
    std::string PlatformBase::getHeader() const
    {
        return "";
    }

    /* virtual */
    std::string PlatformBase::getFooter() const
    {
        return "";
    }
}

