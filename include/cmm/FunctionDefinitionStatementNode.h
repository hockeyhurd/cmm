/**
 * An AST node for a function declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-07-16
 */

#pragma once

#ifndef CMM_FUNCTION_DEFINITION_STATEMENT_H
#define CMM_FUNCTION_DEFINITION_STATEMENT_H

// Our includes
#include <cmm/Types.h>
#include <cmm/BlockNode.h>
#include <cmm/StatementNode.h>
#include <cmm/TypeNode.h>

// std includes
#include <string>

namespace cmm
{
    class VariableNode;

    class FunctionDefinitionStatementNode : public StatementNode
    {
    public:

        /**
         * Constructor.
         *
         * @param type the TypeNode.
         * @param name the name of the function.
         * @param block the block of the function.
         */
        FunctionDefinitionStatementNode(TypeNode type, const std::string& funcName, BlockNode&& block);

        /**
         * Constructor.
         *
         * @param type the TypeNode.
         * @param name the name of the function.
         * @param block the block of the function.
         */
        FunctionDefinitionStatementNode(TypeNode type, std::string&& funcName, BlockNode&& block) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        FunctionDefinitionStatementNode(const FunctionDefinitionStatementNode&) = delete;

        /**
         * Move constructor.
         */
        FunctionDefinitionStatementNode(FunctionDefinitionStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~FunctionDefinitionStatementNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return FunctionDefinitionStatementNode reference.
         */
        FunctionDefinitionStatementNode& operator= (const FunctionDefinitionStatementNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return FunctionDefinitionStatementNode reference.
         */
        FunctionDefinitionStatementNode& operator= (FunctionDefinitionStatementNode&&) CMM_NOEXCEPT = default;

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
        BlockNode block;
    };
}

#endif //!CMM_FUNCTION_DEFINITION_STATEMENT_H

