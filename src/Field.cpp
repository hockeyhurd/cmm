/**
 * A class for representing a struct or union's field.
 *
 * @author hockeyhurd
 * @version 2023-01-08
 */

// Our includes
#include <cmm/Field.h>

namespace cmm
{
    Field::Field(const std::string& name, const CType& datatype, const u32 index) :
        name(name), datatype(datatype), index(index)
    {
    }

    Field::Field(std::string&& name, CType&& datatype, const u32 index) CMM_NOEXCEPT :
        name(std::move(name)), datatype(std::move(datatype)), index(index)
    {
    }

    std::string& Field::getName() CMM_NOEXCEPT
    {
        return name;
    }

    const std::string& Field::getName() const CMM_NOEXCEPT
    {
        return name;
    }

    CType& Field::getDatatype() CMM_NOEXCEPT
    {
        return datatype;
    }

    const CType& Field::getDatatype() const CMM_NOEXCEPT
    {
        return datatype;
    }

    u32 Field::getIndex() const CMM_NOEXCEPT
    {
        return index;
    }
}

