/**
 * An AST node for litteral ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#include <cmm/LitteralNode.h>

namespace cmm
{
    LitteralNode::LitteralNode(const Location& location) CMM_NOEXCEPT : ExpressionNode(EnumNodeType::LITTERAL, location, CType(EnumCType::NULL_T)),
        value((void*) nullptr)
    {
    }

    LitteralNode::LitteralNode(const Location& location, void* value) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::LITTERAL, location, CType(EnumCType::VOID_PTR)),
        value(value)
    {
    }

    LitteralNode::LitteralNode(const Location& location, const bool value) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::LITTERAL, location, CType(EnumCType::BOOL)),
        value(value)
    {
    }

    LitteralNode::LitteralNode(const Location& location, const char value) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::LITTERAL, location, CType(EnumCType::CHAR)),
        value(value)
    {
    }

    LitteralNode::LitteralNode(const Location& location, const s8 value) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::LITTERAL, location, CType(EnumCType::INT8)),
        value(value)
    {
    }

    LitteralNode::LitteralNode(const Location& location, const s16 value) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::LITTERAL, location, CType(EnumCType::INT16)),
        value(value)
    {
    }

    LitteralNode::LitteralNode(const Location& location, const s32 value) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::LITTERAL, location, CType(EnumCType::INT32)),
        value(value)
    {
    }

    LitteralNode::LitteralNode(const Location& location, const s64 value) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::LITTERAL, location, CType(EnumCType::INT64)),
        value(value)
    {
    }

    LitteralNode::LitteralNode(const Location& location, const f32 value) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::LITTERAL, location, CType(EnumCType::FLOAT)),
        value(value)
    {
    }

    LitteralNode::LitteralNode(const Location& location, const f64 value) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::LITTERAL, location, CType(EnumCType::DOUBLE)),
        value(value)
    {
    }

    // NOTE: Safe const_cast because this is a litteral and CTypeValue doesn't care what kind of char* it actually is.
    LitteralNode::LitteralNode(const Location& location, const char* value) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::LITTERAL, location, CType(EnumCType::STRING)),
        value(const_cast<char*>(value))
    {
    }

    LitteralNode::~LitteralNode()
    {
        if (datatype.type == EnumCType::STRING && value.valueString != nullptr)
        {
            delete[] value.valueString;
            value.valueString = nullptr;
        }
    }

    CTypeValue LitteralNode::getValue() const CMM_NOEXCEPT
    {
        return value;
    }

    std::string LitteralNode::toString() const /* override */
    {
        return "LitteralNode";
    }
}

