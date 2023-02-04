/**
 * A class for representing an enum's enumerator.
 *
 * @author hockeyhurd
 * @version 2023-02-04
 */

// Our includes
#include <cmm/Enumerator.h>

// std includes
#include <stdexcept>

namespace cmm
{
    Enumerator::Enumerator(const std::string& name, const s32 index) : name(name), index(index)
    {
    }

    Enumerator::Enumerator(std::string&& name, const s32 index) CMM_NOEXCEPT :
        name(std::move(name)), index(index)
    {
    }

    std::string& Enumerator::getName() CMM_NOEXCEPT
    {
        return name;
    }

    const std::string& Enumerator::getName() const CMM_NOEXCEPT
    {
        return name;
    }

    s32 Enumerator::getIndex() const CMM_NOEXCEPT
    {
        return index;
    }

    void Enumerator::setIndex(s32 index) CMM_NOEXCEPT
    {
        if (index < -1)
        {
            index = -1;
        }

        this->index = index;
    }
}

