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
    Enumerator::Enumerator(const std::string& name, s32 index) : name(name), index(index < 0 ? 0 : index),
        value(this->index), unsignedFlag(false)
    {
    }

    Enumerator::Enumerator(std::string&& name, s32 index) CMM_NOEXCEPT :
        name(std::move(name)), index(index < 0 ? 0 : index), value(this->index), unsignedFlag(false)
    {
    }

    Enumerator::Enumerator(const std::string& name, const s32 index, const s32 value, const bool isUnsigned) :
        name(name), index(index < 0 ? 0 : index), value(value), unsignedFlag(isUnsigned)
    {
    }

    Enumerator::Enumerator(std::string&& name, const s32 index, const s32 value, const bool isUnsigned) CMM_NOEXCEPT :
        name(std::move(name)), index(index < 0 ? 0 : index), value(value), unsignedFlag(isUnsigned)
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
        if (index < 0)
        {
            index = 0;
        }

        this->index = index;
    }

    s32 Enumerator::getValue() const CMM_NOEXCEPT
    {
        return value;
    }

    void Enumerator::setValue(s32 value) CMM_NOEXCEPT
    {
        this->value = value;
    }

    bool Enumerator::isUnsigned() const CMM_NOEXCEPT
    {
        return unsignedFlag;
    }

    void Enumerator::setUnsigned(bool isUnsigned) CMM_NOEXCEPT
    {
        this->unsignedFlag = isUnsigned;
    }
}

