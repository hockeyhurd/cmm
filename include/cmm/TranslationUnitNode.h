/**
 * An AST node for the current translation unit.
 *
 * @author hockeyhurd
 * @version 2022-08-15
 */

#pragma once

#ifndef CMM_TRANSLATION_UNIT_NODE_H
#define CMM_TRANSLATION_UNIT_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/EnumTable.h>
#include <cmm/Node.h>
#include <cmm/StructTable.h>

// std includes
#include <memory>
#include <unordered_map>
#include <vector>

namespace cmm
{
    // Forward declarations
    class StatementNode;

    class TranslationUnitNode : public Node
    {
    public:

        using StatementList = std::vector<std::unique_ptr<StatementNode>>;
        using StatementListIter = StatementList::iterator;
        using StatementListConstIter = StatementList::const_iterator;
        using CStringTable = std::unordered_map<std::string, std::string>;

    public:

        /**
         * Default constructor.
         */
        TranslationUnitNode() CMM_NOEXCEPT;

        /**
         * Constructor.
         *
         * @param location the location of this node.
         * @param statements a vector of statements (declarations and/or definitions)
         *        defined within a single translation unit.
         */
        TranslationUnitNode(const Location& location, StatementList&& statements, EnumTable&& enumTable) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        TranslationUnitNode(const TranslationUnitNode&) = delete;

        /**
         * Move constructor.
         */
        TranslationUnitNode(TranslationUnitNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~TranslationUnitNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return TranslationUnitNode reference.
         */
        TranslationUnitNode& operator= (const TranslationUnitNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return TranslationUnitNode reference.
         */
        TranslationUnitNode& operator= (TranslationUnitNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets whether there are any statements within this translation unit.
         *
         * @return bool.
         */
        bool empty() const CMM_NOEXCEPT;

        /**
         * The count of statements within the translation unit.
         *
         * @return std::size_t count.
         */
        std::size_t size() const CMM_NOEXCEPT;

        /**
         * Gets the EnumTable.
         *
         * @return reference to the EnumTable.
         */
        EnumTable& getEnumTable() CMM_NOEXCEPT;

        /**
         * Gets the EnumTable.
         *
         * @return const reference to the EnumTable.
         */
        const EnumTable& getEnumTable() const CMM_NOEXCEPT;

        /**
         * Gets the StructTable.
         *
         * @return reference to the StructTable.
         */
        StructTable& getStructTable() CMM_NOEXCEPT;

        /**
         * Gets the StructTable.
         *
         * @return const reference to the StructTable.
         */
        const StructTable& getStructTable() const CMM_NOEXCEPT;

        /**
         * Gets the CStringTable.
         *
         * @return reference to the CStringTable.
         */
        CStringTable& getCStringTable() CMM_NOEXCEPT;

        /**
         * Gets the CStringTable.
         *
         * @return const reference to the CStringTable.
         */
        const CStringTable& getCStringTable() const CMM_NOEXCEPT;

        /**
         * Adds the c-string to the CStringTable.
         *
         * @param str the c-string to add.
         */
        void addCString(const std::string& str);

        /**
         * Adds the c-string to the CStringTable.
         *
         * @param str the c-string to add.
         */
        void addCString(std::string&& str);

        /**
         * Attempts to find if a c-string is in the table or not.
         *
         * @param str the c-string to lookup.
         * @return iterator to the std::string value.
         */
        CStringTable::iterator findCString(const std::string& str);

        /**
         * Attempts to find if a c-string is in the table or not.
         *
         * @param str the c-string to lookup.
         * @return const iterator to the std::string value.
         */
        CStringTable::const_iterator findCString(const std::string& str) const;

        /**
         * The beginning iterator to the statement list.
         *
         * @return StatementListIter.
         */
        StatementListIter begin() CMM_NOEXCEPT;

        /**
         * The beginning const iterator to the statement list.
         *
         * @return StatementListConstIter.
         */
        const StatementListConstIter cbegin() const CMM_NOEXCEPT;

        /**
         * The end iterator to the statement list.
         *
         * @return StatementListIter.
         */
        StatementListIter end() CMM_NOEXCEPT;

        /**
         * The end const iterator to the statement list.
         *
         * @return StatementListConstIter.
         */
        const StatementListConstIter cend() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;

        std::string toString() const override;

    private:

        // The list of statements in this translation unit.
        StatementList statements;

        // The enum table containing all enums for this translation unit.
        EnumTable enumTable;

        // The struct table containing all structs for this translation unit.
        StructTable structTable;

        // A table for RValue c-strings.
        CStringTable cstringTable;
    };
}

#endif //!CMM_TRANSLATION_UNIT_NODE_H

