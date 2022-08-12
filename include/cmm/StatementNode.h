/**
 * An AST node for statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#pragma once

#ifndef CMM_STATEMENT_H
#define CMM_STATEMENT_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Node.h>

namespace cmm
{
    class StatementNode : public Node
    {
    protected:

        /**
         * Default constructor.
         *
         * @param type the NodeType to pass to the parent Node class.
         */
        StatementNode(const NodeType type) CMM_NOEXCEPT;

    public:

        /**
         * Copy constructor.
         */
        StatementNode(const StatementNode&) CMM_NOEXCEPT = default;

        /**
         * Move constructor.
         */
        StatementNode(StatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        virtual ~StatementNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return StatementNode reference.
         */
        StatementNode& operator= (const StatementNode&) CMM_NOEXCEPT = default;

        /**
         * Move assignment operator.
         *
         * @return StatementNode reference.
         */
        StatementNode& operator= (StatementNode&&) CMM_NOEXCEPT = default;

        virtual VisitorResult accept(Visitor* visitor) override = 0;
        virtual std::string toString() const override;
    };
}

#endif //!CMM_STATEMENT_H

