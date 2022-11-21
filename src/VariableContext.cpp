/**
 * A class for handling variable metadata used primarily during analysis.
 *
 * @author hockeyhurd
 * @version 2022-08-31
 */

// Our includes
#include <cmm/VariableContext.h>

namespace cmm
{
    VariableContext::VariableContext(const CType& type, const EnumLocality locality, const EnumModifier modifiers) CMM_NOEXCEPT :
        type(type), locality(locality), modifiers(modifiers)
    {
    }

    CType& VariableContext::getType() CMM_NOEXCEPT
    {
        return type;
    }

    const CType& VariableContext::getCType() const CMM_NOEXCEPT
    {
        return type;
    }

    EnumLocality VariableContext::getLocality() const CMM_NOEXCEPT
    {
        return locality;
    }

    EnumModifier VariableContext::getModifiers() const CMM_NOEXCEPT
    {
        return modifiers;
    }

    void VariableContext::setModifiers(const EnumModifier modifiers) CMM_NOEXCEPT
    {
        this->modifiers = modifiers;
    }
}

