/**
 * An AST node for a block node.
 *
 * @author hockeyhurd
 * @version 2022-07-16
 */

// Our includes
#include <cmm/BlockNode.h>

namespace cmm
{
    BlockNode::BlockNode(const Location& location, StatementList&& statements) CMM_NOEXCEPT :
        StatementNode(EnumNodeType::BLOCK, location), statements(std::move(statements))
    {
    }

    bool BlockNode::empty() const CMM_NOEXCEPT
    {
        return statements.empty();
    }

    BlockNode::size_type BlockNode::size() const CMM_NOEXCEPT
    {
        return statements.size();
    }

    BlockNode::StatementListIter BlockNode::begin() CMM_NOEXCEPT
    {
        return statements.begin();
    }

    const BlockNode::StatementListConstIter BlockNode::cbegin() const CMM_NOEXCEPT
    {
        return statements.cbegin();
    }

    BlockNode::StatementListIter BlockNode::end() CMM_NOEXCEPT
    {
        return statements.end();
    }

    const BlockNode::StatementListConstIter BlockNode::cend() const CMM_NOEXCEPT
    {
        return statements.cend();
    }

    std::string BlockNode::toString() const /* override */
    {
        return "BlockNode";
    }
}

