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
    Field::Field(const std::string& name, const CType& datatype, const s32 index) :
        name(name), datatype(datatype), index(index)
    {
    }

    Field::Field(std::string&& name, CType&& datatype, const s32 index) CMM_NOEXCEPT :
        name(std::move(name)), datatype(std::move(datatype)), index(index)
    {
    }

    std::string& Field::getName() CMM_NOEXCEPT /* override */
    {
        return name;
    }

    const std::string& Field::getName() const CMM_NOEXCEPT /* override */
    {
        return name;
    }

    CType& Field::getDatatype() CMM_NOEXCEPT /* override */
    {
        return datatype;
    }

    const CType& Field::getDatatype() const CMM_NOEXCEPT /* override */
    {
        return datatype;
    }

    void Field::setDatatype(const CType& datatype) /* override */
    {
        this->datatype = datatype;
    }

    s32 Field::getIndex() const CMM_NOEXCEPT /* override */
    {
        return index;
    }

    void Field::setIndex(s32 index) CMM_NOEXCEPT /* override */
    {
        if (index < -1)
        {
            index = -1;
        }

        this->index = index;
    }

    void Field::set(const IField* other) /* override */
    {
        this->name = other->getName();
        this->datatype = other->getDatatype();
        this->index = other->getIndex();
    }
}

