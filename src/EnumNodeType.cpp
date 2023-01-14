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
    bool isValidRHSNodeType(const EnumNodeType type) CMM_NOEXCEPT
    {
#if 0
        return type == EnumNodeType::VARIABLE || type == EnumNodeType::DEREF ||
               type == EnumNodeType::FIELD_ACCESS || type == EnumNodeType::FUNCTION_CALL;
#else
        return type == EnumNodeType::VARIABLE || type == EnumNodeType::DEREF ||
               type == EnumNodeType::FIELD_ACCESS;
#endif
    }
}

