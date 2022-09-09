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
    VariableContext::VariableContext(const EnumCType type, const EnumLocality locality, const EnumModifier modifiers) CMM_NOEXCEPT :
        type(type), locality(locality), modifiers(modifiers)
    {
    }

    EnumCType VariableContext::getType() const CMM_NOEXCEPT
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
}

