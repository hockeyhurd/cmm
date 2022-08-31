/**
 * An AST node for a function definition statement ast nodes.
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
#include <cmm/ParameterNode.h>
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

        using ParamList = std::vector<ParameterNode>;
        using ParamListIter = ParamList::iterator;
        using ParamListConseIter = ParamList::const_iterator;

        /**
         * Constructor.
         *
         * @param location the location of this node.
         * @param type the TypeNode.
         * @param name the name of the function.
         * @param block the block of the function.
         * @param params the parameter list.
         */
        FunctionDefinitionStatementNode(const Location& location, TypeNode type,
            const std::string& funcName, BlockNode&& block, ParamList&& params = ParamList());

        /**
         * Constructor.
         *
         * @param location the location of this node.
         * @param type the TypeNode.
         * @param name the name of the function.
         * @param block the block of the function.
         * @param params the parameter list.
         */
        FunctionDefinitionStatementNode(const Location& location, TypeNode type, std::string&& funcName,
            BlockNode&& block, ParamList&& params = ParamList()) CMM_NOEXCEPT;

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
         * Gets the BlockNode.
         * Note: Exposed for unit testing.
         *
         * @return BlockNode.
         */
        BlockNode& getBlock() CMM_NOEXCEPT;

        /**
         * Gets the BlockNode.
         * Note: Exposed for unit testing.
         *
         * @return BlockNode.
         */
        const BlockNode& getBlock() const CMM_NOEXCEPT;

        /**
         * Parameter list iterator from the beginning.
         *
         * @return ParamListIter.
         */
        ParamListIter begin() CMM_NOEXCEPT;

        /**
         * Parameter list const iterator from the beginning.
         *
         * @return ParamListConseIter.
         */
        const ParamListConseIter cbegin() const CMM_NOEXCEPT;

        /**
         * Parameter list iterator from the end.
         *
         * @return ParamListIter.
         */
        ParamListIter end() CMM_NOEXCEPT;

        /**
         * Parameter list const iterator from the end.
         *
         * @return ParamListConseIter.
         */
        const ParamListConseIter cend() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override
        {
            return visitor->visit(*this);
        }

        std::string toString() const override;

    private:

        TypeNode type;
        std::string funcName;
        BlockNode block;
        ParamList params;
    };
}

#endif //!CMM_FUNCTION_DEFINITION_STATEMENT_H

