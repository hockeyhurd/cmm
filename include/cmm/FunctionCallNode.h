/**
 * An AST node for a function call node.
 *
 * @author hockeyhurd
 * @version 2022-07-24
 */

#pragma once

#ifndef CMM_FUNCTION_CALL_H
#define CMM_FUNCTION_CALL_H

// Our includes
#include <cmm/Types.h>
#include <cmm/ArgNode.h>
#include <cmm/Node.h>

// std includes
#include <string>
#include <vector>

namespace cmm
{
    class FunctionCallNode : public Node
    {
    public:
        using ArgList = std::vector<ArgNode>;
        using size_type = ArgList::size_type;
        using ArgListIter = ArgList::iterator;
        using ArgListConstIter = ArgList::const_iterator;

    public:

        /**
         * Default constructor
         */
        FunctionCallNode();

        /**
         * Constructor with list of arguments used.
         *
         * @param args the ArgList of the function call.
         */
        FunctionCallNode(ArgList&& args) CMM_NOEXCEPT;

        /**
         * Copy constructor
         */
        FunctionCallNode(const FunctionCallNode&) = delete;

        /**
         * Move constructor
         */
        FunctionCallNode(FunctionCallNode&&) CMM_NOEXCEPT = default;

        /**
         * Default destructor
         */
        ~FunctionCallNode() = default;

        /**
         * Copy assignment operator.
         */
        FunctionCallNode& operator= (const FunctionCallNode&) = delete;

        /**
         * Move assignment operator.
         */
        FunctionCallNode& operator= (FunctionCallNode&&) CMM_NOEXCEPT = default;

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
        ArgListIter begin() CMM_NOEXCEPT;

        /**
         * Const iterator to the beginning of the statement list.
         */
        const ArgListConstIter cbegin() const CMM_NOEXCEPT;

        /**
         * Iterator to the end of the statement list.
         */
        ArgListIter end() CMM_NOEXCEPT;

        /**
         * Const iterator to the end of the statement list.
         */
        const ArgListConstIter cend() const CMM_NOEXCEPT;

        /**
         * Generic and templated function needed for visitor pattern.
         */
        template<class ReturnT, class DerivedT, class VisitorT>
        ReturnT accept(VisitorT& visitor)
        {
            return visitor.visit(*std::static_pointer_cast<DerivedT>(*this));
        }

        std::string toString() const override;

    private:

        // The list of ArgNode's used in this function call.
        ArgList args;

    };
}

#endif //!CMM_FUNCTION_CALL_H

