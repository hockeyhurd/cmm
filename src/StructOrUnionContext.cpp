/**
 * A class for handling struct and union metadata used primarily during analysis.
 *
 * @author hockeyhurd
 * @version 2022-10-17
 */

// Our includes
#include <cmm/StructOrUnionContext.h>

namespace cmm
{
    StructOrUnionContext::StructOrUnionContext(const EnumLocality locality, const EnumModifier modifiers) CMM_NOEXCEPT :
        locality(locality), modifiers(modifiers)
    {
    }

    EnumLocality StructOrUnionContext::getLocality() const CMM_NOEXCEPT
    {
        return locality;
    }

    EnumModifier StructOrUnionContext::getModifiers() const CMM_NOEXCEPT
    {
        return modifiers;
    }
}

