/**
 * An AST node for an enum definition statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2023-02-03
 */

#pragma once

#ifndef CMM_ENUM_DEFINITION_STATEMENT_H
#define CMM_ENUM_DEFINITION_STATEMENT_H

// Our includes
#include <cmm/Types.h>
#include <cmm/StatementNode.h>

// std includes
#include <string>

namespace cmm
{
    class EnumDefinitionStatementNode : public StatementNode
    {
    public:

        /**
         * Constructor.
         *
         * @param location the location of this node.
         * @param name the name of the enum.
         */
        EnumDefinitionStatementNode(const Location& location, const std::string& name);

        /**
         * Constructor.
         *
         * @param location the location of this node.
         * @param name the name of the enum.
         */
        EnumDefinitionStatementNode(const Location& location, std::string&& name) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        EnumDefinitionStatementNode(const EnumDefinitionStatementNode&) = delete;

        /**
         * Move constructor.
         */
        EnumDefinitionStatementNode(EnumDefinitionStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~EnumDefinitionStatementNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return EnumDefinitionStatementNode reference.
         */
        EnumDefinitionStatementNode& operator= (const EnumDefinitionStatementNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return EnumDefinitionStatementNode reference.
         */
        EnumDefinitionStatementNode& operator= (EnumDefinitionStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the name of the enum.
         *
         * @return std::string.
         */
        std::string& getName() CMM_NOEXCEPT;

        /**
         * Gets the name of the enum.
         *
         * @return std::string.
         */
        const std::string& getName() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The name of the enum
        std::string name;
    };
}

#endif //!CMM_ENUM_DEFINITION_STATEMENT_H


