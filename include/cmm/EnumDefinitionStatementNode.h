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
    struct EnumData;

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

        /**
         * Gets whether there are no enumerators in the enum's definition or not.
         *
         * @return bool true if empty, else false.
         */
        bool empty() const CMM_NOEXCEPT;

        /**
         * Gets the number of enumerators in the enum's definition.
         *
         * @return std::size_t count.
         */
        std::size_t size() const CMM_NOEXCEPT;

        /**
         * Gets the assigned EnumData containing all of the necessary enumerators.
         *
         * @return pointer to the EnumData.
         */
        EnumData* getEnumData() CMM_NOEXCEPT;

        /**
         * Gets the assigned EnumData containing all of the necessary enumerators.
         *
         * @return const pointer to the EnumData.
         */
        const EnumData* getEnumData() const CMM_NOEXCEPT;

        /**
         * Assigns the EnumData to this EnumDefinitionStatementNode.
         *
         * @param structData pointer to the EnumData being assigned.
         */
        void setEnumData(EnumData* enumData) CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The name of the enum
        std::string name;

        // A pointer to the enum's data which includes things
        // such as enumerator information.
        EnumData* enumData;
    };
}

#endif //!CMM_ENUM_DEFINITION_STATEMENT_H


