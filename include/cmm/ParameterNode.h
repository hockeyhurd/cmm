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
#include <string>

namespace cmm
{
    class ParameterNode : public Node
    {
    public:

        /**
         * Constructor
         *
         * @param type the type of this variable.
         * @param variable the variable of this parameter.
         */
        ParameterNode(TypeNode&& type, VariableNode&& variable) CMM_NOEXCEPT;

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
         * @return reference to the VariableNode.
         */
        VariableNode& getVariable() CMM_NOEXCEPT;

        /**
         * The associated variable of the parameter.
         *
         * @return const reference to the VariableNode.
         */
        const VariableNode& getVariable() const CMM_NOEXCEPT;

        std::string toString() const override;

    private:

        // The type for this parameter.
        TypeNode type;

        // The variable for this parameter.
        VariableNode variable;
    };
}

#endif //!CMM_PARAMETER_NODE_H

