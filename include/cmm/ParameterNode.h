/**
 * An AST node for a parameter ast node.
 *
 * @author hockeyhurd
 * @version 2022-07-20
 */

#pragma once

#ifndef CMM_PARAMETER_NODE_H
#define CMM_PARAMETER_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Node.h>
#include <cmm/TypeNode.h>
#include <cmm/VariableNode.h>

// std includes
#include <optional>
#include <string>

namespace cmm
{
    class ParameterNode : public Node
    {
    public:

        /**
         * Constructor without a variable name.
         *
         * @param location the location of this node.
         * @param type the type of this variable.
         */
        ParameterNode(const Location& location, TypeNode&& type) CMM_NOEXCEPT;

        /**
         * Constructor with a variable name.
         *
         * @param location the location of this node.
         * @param type the type of this variable.
         * @param variable the variable of this parameter.
         */
        ParameterNode(const Location& location, TypeNode&& type, VariableNode&& variable) CMM_NOEXCEPT;

        /**
         * Default copy constructor.
         */
        ParameterNode(const ParameterNode&) = default;

        /**
         * Default move constructor.
         */
        ParameterNode(ParameterNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~ParameterNode() = default;

        /**
         * Default move assignment operator.
         */
        ParameterNode& operator= (const ParameterNode&) = default;

        /**
         * Default move assignment operator.
         */
        ParameterNode& operator= (ParameterNode&&) CMM_NOEXCEPT = default;

        /**
         * The associated datatype of the parameter.
         *
         * @return reference to the TypeNode.
         */
        TypeNode& getDatatype() CMM_NOEXCEPT;

        /**
         * The associated datatype of the parameter.
         *
         * @return const reference to the TypeNode.
         */
        const TypeNode& getDatatype() const CMM_NOEXCEPT;

        /**
         * The associated variable of the parameter.
         *
         * @return optional reference to the VariableNode.
         */
        std::optional<VariableNode>& getVariable() CMM_NOEXCEPT;

        /**
         * The associated variable of the parameter.
         *
         * @return optional const reference to the VariableNode.
         */
        const std::optional<VariableNode>& getVariable() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override
        {
            return visitor->visit(*this);
        }

        std::string toString() const override;

    private:

        // The type for this parameter.
        TypeNode type;

        // The variable for this parameter.
        std::optional<VariableNode> variable;
    };
}

#endif //!CMM_PARAMETER_NODE_H

