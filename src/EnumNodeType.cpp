/**
 * An enumeration for types of Nodes.
 *
 * @author hockeyhurd
 * @version 2022-11-01
 */

// Our includes
#include <cmm/EnumNodeType.h>

namespace cmm
{
    bool isValidNonLitteralRHSNodeType(const EnumNodeType type) CMM_NOEXCEPT
    {
        return type == EnumNodeType::VARIABLE || type == EnumNodeType::DEREF ||
               type == EnumNodeType::FIELD_ACCESS;
    }
}

