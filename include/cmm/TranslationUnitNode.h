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
#include <cmm/Node.h>

// std includes
#include <memory>
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

    public:

        /**
         * Default constructor.
         */
        TranslationUnitNode() CMM_NOEXCEPT;

        /**
         * Constructor.
         *
         * @param statements a vector of statements (declarations and/or definitions)
         *        defined within a single translation unit.
         */
        TranslationUnitNode(StatementList&& statements) CMM_NOEXCEPT;

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
    };
}

#endif //!CMM_TRANSLATION_UNIT_NODE_H

