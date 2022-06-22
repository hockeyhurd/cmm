/**
 * An AST node for litteral types.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#include <cmm/LitteralNode.h>

namespace cmm
{
    LitteralNode::LitteralNode(void* value) noexcept : Node(NodeType::LITTERAL),
        value(value), type(EnumCType::VOID_PTR)
    {
    }

    LitteralNode::LitteralNode(const bool value) noexcept : Node(NodeType::LITTERAL),
        value(value), type(EnumCType::BOOL)
    {
    }

    LitteralNode::LitteralNode(const char value) noexcept : Node(NodeType::LITTERAL),
        value(value), type(EnumCType::CHAR)
    {
    }

    LitteralNode::LitteralNode(const s8 value) noexcept : Node(NodeType::LITTERAL),
        value(value), type(EnumCType::INT8)
    {
    }

    LitteralNode::LitteralNode(const s16 value) noexcept : Node(NodeType::LITTERAL),
        value(value), type(EnumCType::INT16)
    {
    }

    LitteralNode::LitteralNode(const s32 value) noexcept : Node(NodeType::LITTERAL),
        value(value), type(EnumCType::INT32)
    {
    }

    LitteralNode::LitteralNode(const s64 value) noexcept : Node(NodeType::LITTERAL),
        value(value), type(EnumCType::INT64)
    {
    }

    // NOTE: Safe const_cast because this is a litteral and CType doesn't care what kind of char* it actually is.
    LitteralNode::LitteralNode(const char* value) noexcept : Node(NodeType::LITTERAL),
        value(const_cast<char*>(value)),
        type(EnumCType::STRING)
    {
    }

    EnumCType LitteralNode::getTypeof() const noexcept
    {
        return type;
    }

    CType LitteralNode::getValue() const noexcept
    {
        return value;
    }

    std::string LitteralNode::toString() const /* override */
    {
        return "LitteralNode";
    }
}

