/**
 * An AST node for declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#pragma once

#ifndef CMM_DECLARATION_STATEMENT_H
#define CMM_DECLARATION_STATEMENT_H

// Our includes
#include <cmm/Types.h>
#include <cmm/StatementNode.h>

// std includes
#include <string>

namespace cmm
{
    class TypeNode;
    class VariableNode;

    class DeclarationStatementNode : public StatementNode
    {
    public:

        /**
         * Default constructor.
         *
         * @param type the TypeNode.
         * @param variable the VariableNode.
         */
        DeclarationStatementNode(std::shared_ptr<TypeNode> type, std::shared_ptr<VariableNode> variable) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        DeclarationStatementNode(const DeclarationStatementNode&) = default;

        /**
         * Move constructor.
         */
        DeclarationStatementNode(DeclarationStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~DeclarationStatementNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return DeclarationStatementNode reference.
         */
        DeclarationStatementNode& operator= (const DeclarationStatementNode&) = default;

        /**
         * Move assignment operator.
         *
         * @return DeclarationStatementNode reference.
         */
        DeclarationStatementNode& operator= (DeclarationStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the datatype.
         *
         * @return TypeNode.
         */
        EnumCType getDatatype() const CMM_NOEXCEPT;

        /**
         * Gets the variable.
         *
         * @return VariableNode.
         */
        std::string& getName() CMM_NOEXCEPT;

        /**
         * Gets the variable.
         *
         * @return VariableNode.
         */
        const std::string& getName() const CMM_NOEXCEPT;

        /**
         * Generic and templated function needed for visitor pattern.
         */
        template<class ReturnT, class DerivedT, class VisitorT>
        ReturnT accept(VisitorT& visitor)
        {
            return visitor.visit(*std::static_pointer_cast<DerivedT>(*this));
        }

        std::string toString() const override;

    private:

        std::shared_ptr<TypeNode> type;
        std::shared_ptr<VariableNode> variable;
    };
}

#endif //!CMM_DECLARATION_STATEMENT_H

