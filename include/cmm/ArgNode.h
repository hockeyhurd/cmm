/**
 * An AST node for an argument that is contained in a function call, node.
 *
 * @author hockeyhurd
 * @version 2022-07-24
 */

#pragma once

#ifndef CMM_ARG_H
#define CMM_ARG_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Node.h>

// std includes
#include <memory>
#include <string>

namespace cmm
{
    class ExpressionNode;

    class ArgNode : public Node
    {
    public:

        /**
         * Constructor with a value.
         *
         * @param value the value used as an argument.
         */
        ArgNode(std::unique_ptr<ExpressionNode>&& value) CMM_NOEXCEPT;

        /**
         * Copy constructor
         */
        ArgNode(const ArgNode&) = delete;

        /**
         * Move constructor
         */
        ArgNode(ArgNode&&) CMM_NOEXCEPT = default;

        /**
         * Default destructor
         */
        ~ArgNode() = default;

        /**
         * Copy assignment operator.
         */
        ArgNode& operator= (const ArgNode&) = delete;

        /**
         * Move assignment operator.
         */
        ArgNode& operator= (ArgNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the value of this argument.
         *
         * @return EnumCType.
         */
        ExpressionNode* getValue() CMM_NOEXCEPT;

        /**
         * Gets the value of this argument.
         *
         * @return EnumCType.
         */
        const ExpressionNode* getValue() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override
        {
            return visitor->visit(*this);
        }

        std::string toString() const override;

    private:

        // The value used in this arg
        std::unique_ptr<ExpressionNode> value;

    };
}

#endif //!CMM_ARG_H

