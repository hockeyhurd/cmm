/**
 * An AST node for a block node.
 *
 * @author hockeyhurd
 * @version 2022-07-16
 */

#pragma once

#ifndef CMM_BLOCK_NODE_H
#define CMM_BLOCK_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Node.h>
#include <cmm/StatementNode.h>
// #include <cmm/TypeNode.h>

// std includes
#include <string>
#include <vector>

namespace cmm
{
    class BlockNode : public Node
    {
    public:
        using StatementList = std::vector<std::unique_ptr<StatementNode>>;
        using size_type = StatementList::size_type;
        using StatementListIter = StatementList::iterator;
        using StatementListConstIter = StatementList::const_iterator;

    public:

        /**
         * Default constructor
         */
        BlockNode();

        /**
         * Constructor with initializing statement list.
         *
         * @param statements StatementList.
         */
        BlockNode(StatementList&& statements) CMM_NOEXCEPT;

        /**
         * Copy constructor
         */
        BlockNode(const BlockNode&) = delete;

        /**
         * Move constructor
         */
        BlockNode(BlockNode&&) CMM_NOEXCEPT = default;

        /**
         * Default destructor
         */
        ~BlockNode() = default;

        /**
         * Copy assignment operator.
         */
        BlockNode& operator= (const BlockNode&) = delete;

        /**
         * Move assignment operator.
         */
        BlockNode& operator= (BlockNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets whether the block is empty of statements or not.
         *
         * @return bool true if empty, else false.
         */
        bool empty() const CMM_NOEXCEPT;

        /**
         * The number of statements in this block.
         *
         * @return size_type count.
         */
        size_type size() const CMM_NOEXCEPT;

        /**
         * Iterator to the beginning of the statement list.
         */
        StatementListIter begin() CMM_NOEXCEPT;

        /**
         * Const iterator to the beginning of the statement list.
         */
        const StatementListConstIter cbegin() const CMM_NOEXCEPT;

        /**
         * Iterator to the end of the statement list.
         */
        StatementListIter end() CMM_NOEXCEPT;

        /**
         * Const iterator to the end of the statement list.
         */
        const StatementListConstIter cend() const CMM_NOEXCEPT;

    private:

        // The internal list of StatementNodes
        StatementList statements;
    };
}

#endif //!CMM_BLOCK_NODE_H

