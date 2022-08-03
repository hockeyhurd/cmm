/**
 * An AST node for litteral ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#include <cmm/LitteralNode.h>

namespace cmm
{
    LitteralNode::LitteralNode() CMM_NOEXCEPT : ExpressionNode(NodeType::LITTERAL),
        value((void*) nullptr), type(EnumCType::NULL_T)
    {
    }

    LitteralNode::LitteralNode(void* value) CMM_NOEXCEPT : ExpressionNode(NodeType::LITTERAL),
        value(value), type(EnumCType::VOID_PTR)
    {
    }

    LitteralNode::LitteralNode(const bool value) CMM_NOEXCEPT : ExpressionNode(NodeType::LITTERAL),
        value(value), type(EnumCType::BOOL)
    {
    }

    LitteralNode::LitteralNode(const char value) CMM_NOEXCEPT : ExpressionNode(NodeType::LITTERAL),
        value(value), type(EnumCType::CHAR)
    {
    }

    LitteralNode::LitteralNode(const s8 value) CMM_NOEXCEPT : ExpressionNode(NodeType::LITTERAL),
        value(value), type(EnumCType::INT8)
    {
    }

    LitteralNode::LitteralNode(const s16 value) CMM_NOEXCEPT : ExpressionNode(NodeType::LITTERAL),
        value(value), type(EnumCType::INT16)
    {
    }

    LitteralNode::LitteralNode(const s32 value) CMM_NOEXCEPT : ExpressionNode(NodeType::LITTERAL),
        value(value), type(EnumCType::INT32)
    {
    }

    LitteralNode::LitteralNode(const s64 value) CMM_NOEXCEPT : ExpressionNode(NodeType::LITTERAL),
        value(value), type(EnumCType::INT64)
    {
    }

    LitteralNode::LitteralNode(const f32 value) CMM_NOEXCEPT : ExpressionNode(NodeType::LITTERAL),
        value(value), type(EnumCType::FLOAT)
    {
    }

    LitteralNode::LitteralNode(const f64 value) CMM_NOEXCEPT : ExpressionNode(NodeType::LITTERAL),
        value(value), type(EnumCType::DOUBLE)
    {
    }

    // NOTE: Safe const_cast because this is a litteral and CType doesn't care what kind of char* it actually is.
    LitteralNode::LitteralNode(const char* value) CMM_NOEXCEPT : ExpressionNode(NodeType::LITTERAL),
        value(const_cast<char*>(value)), type(EnumCType::STRING)
    {
    }

    LitteralNode::~LitteralNode()
    {
        if (type == EnumCType::STRING && value.valueString != nullptr)
        {
            delete[] value.valueString;
            value.valueString = nullptr;
        }
    }

    EnumCType LitteralNode::getValueType() const CMM_NOEXCEPT
    {
        return type;
    }

    CType LitteralNode::getValue() const CMM_NOEXCEPT
    {
        return value;
    }

    std::string LitteralNode::toString() const /* override */
    {
        return "LitteralNode";
    }
}

