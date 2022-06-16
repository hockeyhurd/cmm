#include <cmm/LitteralNode.h>

namespace cmm
{
    LitteralNode::LitteralNode(void* value) noexcept : value(value), type(EnumCType::VOID_PTR)
    {
    }

    LitteralNode::LitteralNode(const char value) noexcept : value(value), type(EnumCType::CHAR)
    {
    }

    LitteralNode::LitteralNode(const s8 value) noexcept : value(value), type(EnumCType::INT8)
    {
    }

    LitteralNode::LitteralNode(const s16 value) noexcept : value(value), type(EnumCType::INT16)
    {
    }

    LitteralNode::LitteralNode(const s32 value) noexcept : value(value), type(EnumCType::INT32)
    {
    }

    LitteralNode::LitteralNode(const s64 value) noexcept : value(value), type(EnumCType::INT64)
    {
    }

    LitteralNode::LitteralNode(char* value) : value(value), type(EnumCType::STRING)
    {
    }
}

