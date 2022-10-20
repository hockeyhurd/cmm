/**
 * An AST node for a struct forward declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-10-18
 */

#pragma once

#ifndef CMM_STRUCT_FWD_DECLARATION_STATEMENT_H
#define CMM_STRUCT_FWD_DECLARATION_STATEMENT_H

// Our includes
#include <cmm/Types.h>
#include <cmm/StatementNode.h>
#include <cmm/TypeNode.h>

// std includes
#include <optional>
#include <string>

namespace cmm
{
    class StructFwdDeclarationStatementNode : public StatementNode
    {
    public:

        /**
         * Default constructor.
         *
         * @param location the location of this node.
         * @param type the TypeNode.
         */
        StructFwdDeclarationStatementNode(const Location& location, TypeNode&& type) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        StructFwdDeclarationStatementNode(const StructFwdDeclarationStatementNode&) = delete;

        /**
         * Move constructor.
         */
        StructFwdDeclarationStatementNode(StructFwdDeclarationStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~StructFwdDeclarationStatementNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return StructFwdDeclarationStatementNode reference.
         */
        StructFwdDeclarationStatementNode& operator= (const StructFwdDeclarationStatementNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return StructFwdDeclarationStatementNode reference.
         */
        StructFwdDeclarationStatementNode& operator= (StructFwdDeclarationStatementNode&&) CMM_NOEXCEPT = default;

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
         * Gets the name of the struct.
         *
         * @return std::string.
         */
        std::string& getName() CMM_NOEXCEPT;

        /**
         * Gets the name of the struct.
         *
         * @return std::string.
         */
        const std::string& getName() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        TypeNode type;
    };
}

#endif //!CMM_STRUCT_FWD_DECLARATION_STATEMENT_H

