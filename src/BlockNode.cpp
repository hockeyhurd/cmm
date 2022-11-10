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
    BlockNode::BlockNode(const Location& beginLoc, const Location& endLoc, StatementList&& statements) CMM_NOEXCEPT :
        StatementNode(EnumNodeType::BLOCK, beginLoc), endLoc(endLoc), statements(std::move(statements))
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

    Location& BlockNode::getBeginLocation() CMM_NOEXCEPT
    {
        return getLocation();
    }

    const Location& BlockNode::getBeginLocation() const CMM_NOEXCEPT
    {
        return getLocation();
    }

    Location& BlockNode::getEndLocation() CMM_NOEXCEPT
    {
        return endLoc;
    }

    const Location& BlockNode::getEndLocation() const CMM_NOEXCEPT
    {
        return endLoc;
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

