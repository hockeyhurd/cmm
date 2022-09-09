/**
 * An AST node for representing the address of operator node.
 *
 * @author hockeyhurd
 * @version 2022-08-25
 */

#pragma once

#ifndef CMM_ADDRESS_OF_NODE_H
#define CMM_ADDRESS_OF_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/UnaryOpNode.h>
#include <cmm/VariableNode.h>

// std includes
#include <string>

namespace cmm
{
    class AddressOfNode : public UnaryOpNode
    {
    public:

        /**
         * Constructor.
         *
         * @param location the location of this node.
         * @param variable the variable to get the address of.
         */
        AddressOfNode(const Location& location, VariableNode&& variable) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        AddressOfNode(const AddressOfNode&) = delete;

        /**
         * Move constructor.
         */
        AddressOfNode(AddressOfNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~AddressOfNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return AddressOfNode reference.
         */
        AddressOfNode& operator= (const AddressOfNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return AddressOfNode reference.
         */
        AddressOfNode& operator= (AddressOfNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the expression to be dereferenced.
         *
         * @return reference to the VariableNode.
         */
        VariableNode& getVariable() CMM_NOEXCEPT;

        /**
         * Gets the variable whose address is requested.
         *
         * @return const reference to the VariableNode.
         */
        const VariableNode& getVariable() const CMM_NOEXCEPT;

        /**
         * Attempts to pointer chase until we get the underlying type.
         * This is needed because there could be several other AddressOfNodes before
         * we get to say a VariableNode.
         *
         * @return underlying NodeType of wrapped ExpressionNode.
         */
        NodeType getRootType() const CMM_NOEXCEPT;

        bool hasExpression() const CMM_NOEXCEPT override;
        ExpressionNode* getExpression() CMM_NOEXCEPT override;
        const ExpressionNode* getExpression() const CMM_NOEXCEPT override;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The variable whose address is requested.
        VariableNode variable;
    };
}

#endif //!CMM_ADDRESS_OF_NODE_H

