/**
 * An enumeration for types of Nodes.
 *
 * @author hockeyhurd
 * @version 2022-11-01
 */

#pragma once

#ifndef CMM_ENUM_NODE_TYPE_H
#define CMM_ENUM_NODE_TYPE_H

#ifndef CMM_NOEXCEPT
    #if CPP_VER >= 2014
        #define CMM_NOEXCEPT noexcept
    #elif CPP_VER >= 2011
        #define CMM_NOEXCEPT noexcept
    #else
        #define CMM_NOEXCEPT
    #endif
#endif

namespace cmm
{
    enum class EnumNodeType
    {
        UNKNOWN = 0, ADDRESS_OF, ARG, CAST, COMPILATION_UNIT, BIN_OP, BLOCK, DEREF,
        ENUM_DEFINITION, ENUM_USAGE,
        FIELD_ACCESS, FUNCTION_CALL, FUNCTION_DECLARATION_STATEMENT, FUNCTION_DEFINITION_STATEMENT,
        EXPRESSION_STATEMENT, EXPRESSION, IF_ELSE_STATEMENT, PARAMETER, PAREN_EXPRESSION,
        LITTERAL, RETURN_STATEMENT, STRUCT_DEFINITION, STRUCT_FWD_DECLARATION, TRANSLATION_UNIT, UNARY_OP,
        VARIABLE, VARIABLE_DECLARATION_STATEMENT,
        WHILE_STATEMENT
    };

    /**
     * Checks to see if the EnumNodeType is valid for RHS usage. This is typically
     * helpful for when checking whether a DerefNode should "pushed" or "popped".
     * Note: This does NOT include Litterals.
     *
     * @return bool.
     */
    bool isValidNonLitteralRHSNodeType(const EnumNodeType type) CMM_NOEXCEPT;
}

#endif //!CMM_ENUM_NODE_TYPE_H

