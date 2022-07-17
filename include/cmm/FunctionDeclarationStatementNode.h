/**
 * An AST node for a function declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-07-16
 */

#pragma once

#ifndef CMM_FUNCTION_DECLARATION_STATEMENT_H
#define CMM_FUNCTION_DECLARATION_STATEMENT_H

// Our includes
#include <cmm/Types.h>
#include <cmm/StatementNode.h>
#include <cmm/TypeNode.h>

// std includes
#include <string>

namespace cmm
{
    class VariableNode;

    class FunctionDeclarationStatementNode : public StatementNode
    {
    public:

        /**
         * Constructor.
         *
         * @param type the TypeNode.
         * @param name the name of the function.
         */
        FunctionDeclarationStatementNode(TypeNode type, const std::string& funcName);

        /**
         * Constructor.
         *
         * @param type the TypeNode.
         * @param name the name of the function.
         */
        FunctionDeclarationStatementNode(TypeNode type, std::string&& funcName) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        FunctionDeclarationStatementNode(const FunctionDeclarationStatementNode&) = delete;

        /**
         * Move constructor.
         */
        FunctionDeclarationStatementNode(FunctionDeclarationStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~FunctionDeclarationStatementNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return FunctionDeclarationStatementNode reference.
         */
        FunctionDeclarationStatementNode& operator= (const FunctionDeclarationStatementNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return FunctionDeclarationStatementNode reference.
         */
        FunctionDeclarationStatementNode& operator= (FunctionDeclarationStatementNode&&) CMM_NOEXCEPT = default;

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

        TypeNode type;
        std::string funcName;
    };
}

#endif //!CMM_FUNCTION_DECLARATION_STATEMENT_H

