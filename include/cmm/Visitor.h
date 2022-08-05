/**
 * A base visitor implementing the visitor pattern.
 *
 * @author hockeyhurd
 * @version 2022-08-24
 */

#pragma once

#ifndef CMM_VISITOR_H
#define CMM_VISITOR_H

// Our includes
#include <cmm/Types.h>
#include <cmm/NodeListFwd.h>

namespace cmm
{
    template<class T>
    class Visitor
    {
    public:

        /**
         * Constructor with a value.
         *
         * @param value the value used as an argument.
         */
        Visitor() CMM_NOEXCEPT = default;

    public:

        /**
         * Copy constructor
         */
        Visitor(const Visitor<T>&) = delete;

        /**
         * Move constructor
         */
        Visitor(Visitor<T>&&) CMM_NOEXCEPT = delete;

        /**
         * Default destructor
         */
        virtual ~Visitor() = default;

        /**
         * Copy assignment operator.
         */
        Visitor<T>& operator= (const Visitor<T>&) = delete;

        /**
         * Move assignment operator.
         */
        Visitor<T>& operator= (Visitor<T>&&) CMM_NOEXCEPT = delete;

        virtual T visitor(ArgNode& node) = 0;
        virtual T visitor(BinOpNode& node) = 0;
        virtual T visitor(BlockNode& node) = 0;
        virtual T visitor(CompilationUnitNode& node) = 0;
        virtual T visitor(FunctionCallNode& node) = 0;
        virtual T visitor(FunctionDeclarationStatementNode& node) = 0;
        virtual T visitor(FunctionDefinitionStatementNode& node) = 0;
        virtual T visitor(ExpressionNode& node) = 0;
        virtual T visitor(ExpressionStatementNode& node) = 0;
        virtual T visitor(IfElseStatementNode& node) = 0;
        virtual T visitor(LitteralNode& node) = 0;
        virtual T visitor(ParameterNode& node) = 0;
        virtual T visitor(ParenExpressionNode& node) = 0;
        virtual T visitor(ReturnStatementNode& node) = 0;
        virtual T visitor(TypeNode& node) = 0;
        virtual T visitor(VariableNode& node) = 0;
        virtual T visitor(VariableDeclarationStatementNode& node) = 0;

    protected:

        /**
         * Used throw a std::runtime_error exception for when a visiting function may
         * not be implemented.
         *
         * @param fileName the name of the current file being processed.
         * @param line the line in this non-implemented function.
         */
        [[noreturn]]
        static void unimplementedException(const char* fileName, const std::size_t line);

    };
}

#endif //!CMM_VISITOR_H

