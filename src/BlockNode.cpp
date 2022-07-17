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
    BlockNode::BlockNode() : Node(NodeType::BLOCK)
    {
    }

    BlockNode::BlockNode(StatementList&& statements) CMM_NOEXCEPT : Node(NodeType::BLOCK),
        statements(std::move(statements))
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
}

