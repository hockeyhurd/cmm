/**
 * An AST node for variable ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#pragma once

#ifndef CMM_VARIABLE_NODE_H
#define CMM_VARIABLE_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/ExpressionNode.h>

// std includes
#include <string>

namespace cmm
{
    class VariableNode : public ExpressionNode
    {
    public:

        /**
         * Constructor.
         *
         * @param name the std::string name of the variable.
         * @param derefCount the number of dereferences.
         */
        explicit VariableNode(const std::string& name);

        /**
         * Constructor.
         *
         * @param name the std::string name of the variable.
         */
        explicit VariableNode(std::string&& name) CMM_NOEXCEPT;

        /**
         * Constructor.
         *
         * @param name the std::string name of the variable.
         * @param derefCount the number of dereferences.
         */
        VariableNode(const std::string& name, const u32 derefCount);

        /**
         * Constructor.
         *
         * @param name the std::string name of the variable.
         * @param derefCount the number of dereferences.
         */
        VariableNode(std::string&& name, const u32 derefCount) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        VariableNode(const VariableNode&) = default;

        /**
         * Move constructor.
         */
        VariableNode(VariableNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~VariableNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return VariableNode reference.
         */
        VariableNode& operator= (const VariableNode&) = default;

        /**
         * Move assignment operator.
         *
         * @return VariableNode reference.
         */
        VariableNode& operator= (VariableNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the name of this variable.
         *
         * @return std::string.
         */
        std::string& getName() CMM_NOEXCEPT;

        /**
         * Gets the name of this variable.
         *
         * @return std::string.
         */
        const std::string& getName() const CMM_NOEXCEPT;

        /**
         * The number of dereferences.
         *
         * @return u32 dereference count.
         */
        u32 getDereferenceCount() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The name of the variable.
        std::string name;

        // The amount of derefencing '*'.
        u32 derefCount;
    };
}

#endif //!CMM_VARIABLE_NODE_H

