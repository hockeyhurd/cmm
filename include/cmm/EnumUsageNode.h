/**
 * An AST node for an enumerator usage ast nodes.
 *
 * @author hockeyhurd
 * @version 2023-02-18
 */

#pragma once

#ifndef CMM_ENUM_USAGE_NODE_H
#define CMM_ENUM_USAGE_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/ExpressionNode.h>

// std includes
#include <string>

namespace cmm
{
    // Forward declarations
    class Enumerator;

    class EnumUsageNode : public ExpressionNode
    {
    public:

        /**
         * Constructor.
         *
         * @param location the location of this node.
         * @param name the name of the enumerator.
         */
        EnumUsageNode(const Location& location, const std::string& name);

        /**
         * Constructor.
         *
         * @param location the location of this node.
         * @param name the name of the enumerator.
         */
        EnumUsageNode(const Location& location, std::string&& name) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        EnumUsageNode(const EnumUsageNode&) = delete;

        /**
         * Move constructor.
         */
        EnumUsageNode(EnumUsageNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~EnumUsageNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return EnumUsageNode reference.
         */
        EnumUsageNode& operator= (const EnumUsageNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return EnumUsageNode reference.
         */
        EnumUsageNode& operator= (EnumUsageNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the name of the enumerator.
         *
         * @return std::string.
         */
        std::string& getName() CMM_NOEXCEPT;

        /**
         * Gets the name of the enumerator.
         *
         * @return std::string.
         */
        const std::string& getName() const CMM_NOEXCEPT;

        /**
         * Gets the Enumerator associated with this node.
         *
         * @return pointer to the Enumerator if set, else returns nullptr.
         */
        Enumerator* getEnumerator() CMM_NOEXCEPT;

        /**
         * Gets the Enumerator associated with this node.
         *
         * @return const pointer to the Enumerator if set, else returns nullptr.
         */
        const Enumerator* getEnumerator() const CMM_NOEXCEPT;

        /**
         * Sets the Enumerator.
         *
         * @param enumerator the pointer to the enum that 'owns' this enumerator.
         */
        void setEnumerator(Enumerator* enumerator) CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The name of the enumerator
        std::string name;

        // A pointer to the Enumerator's data.
        Enumerator* enumerator;
    };
}

#endif //!CMM_ENUM_USAGE_NODE_H


