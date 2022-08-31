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
#include <cmm/ParameterNode.h>
#include <cmm/StatementNode.h>
#include <cmm/TypeNode.h>

// std includes
#include <string>
#include <vector>

namespace cmm
{
    class VariableNode;

    class FunctionDeclarationStatementNode : public StatementNode
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
         * @param params the parameter list.
         */
        FunctionDeclarationStatementNode(const Location& location, TypeNode type, const std::string& funcName,
            ParamList&& params = ParamList());

        /**
         * Constructor.
         *
         * @param location the location of this node.
         * @param type the TypeNode.
         * @param name the name of the function.
         * @param params the parameter list.
         */
        FunctionDeclarationStatementNode(const Location& location, TypeNode type, std::string&& funcName,
            ParamList&& params = ParamList()) CMM_NOEXCEPT;

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

        VisitorResult accept(Visitor* visitor) override;

        std::string toString() const override;

    private:

        TypeNode type;
        std::string funcName;
        ParamList params;
    };
}

#endif //!CMM_FUNCTION_DECLARATION_STATEMENT_H

