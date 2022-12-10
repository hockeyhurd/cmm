/**
 * An AST node for a variable declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-25
 */

#pragma once

#ifndef CMM_VARIABLE_DECLARATION_STATEMENT_H
#define CMM_VARIABLE_DECLARATION_STATEMENT_H

// Our includes
#include <cmm/Types.h>
#include <cmm/StatementNode.h>
#include <cmm/TypeNode.h>
#include <cmm/VariableNode.h>

// std includes
#include <optional>
#include <string>

namespace cmm
{
    class VariableDeclarationStatementNode : public StatementNode
    {
    public:

        /**
         * Default constructor.
         *
         * @param location the location of this node.
         * @param type the TypeNode.
         * @param variable the VariableNode.
         */
        VariableDeclarationStatementNode(const Location& location, TypeNode type, VariableNode&& variable) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        VariableDeclarationStatementNode(const VariableDeclarationStatementNode&) = delete;

        /**
         * Move constructor.
         */
        VariableDeclarationStatementNode(VariableDeclarationStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~VariableDeclarationStatementNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return VariableDeclarationStatementNode reference.
         */
        VariableDeclarationStatementNode& operator= (const VariableDeclarationStatementNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return VariableDeclarationStatementNode reference.
         */
        VariableDeclarationStatementNode& operator= (VariableDeclarationStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the TypeNode type.
         *
         * @return reference to TypeNode type.
         */
        TypeNode& getTypeNode() CMM_NOEXCEPT;

        /**
         * Gets the TypeNode type.
         *
         * @return reference to TypeNode type.
         */
        const TypeNode& getTypeNode() const CMM_NOEXCEPT;

        /**
         * Gets the datatype.
         *
         * @return TypeNode.
         */
        CType& getDatatype() CMM_NOEXCEPT;

        /**
         * Gets the datatype.
         *
         * @return TypeNode.
         */
        const CType& getDatatype() const CMM_NOEXCEPT;

        /**
         * Gets the variable's name.
         *
         * @return std::string.
         */
        std::string& getName() CMM_NOEXCEPT;

        /**
         * Gets the variable's name.
         *
         * @return std::string.
         */
        const std::string& getName() const CMM_NOEXCEPT;

        /**
         * Gets the variable.
         *
         * @return VariableNode.
         */
        VariableNode& getVariable() CMM_NOEXCEPT;

        /**
         * Gets the variable.
         *
         * @return VariableNode.
         */
        const VariableNode& getVariable() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        TypeNode type;
        VariableNode variable;
    };
}

#endif //!CMM_VARIABLE_DECLARATION_STATEMENT_H

