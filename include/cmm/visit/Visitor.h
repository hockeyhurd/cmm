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

// std includes
#include <sstream>
#include <string>

#define CMM_UNIMPLEMENTED_EXCEPTION() unimplementedException(__FILE__, __func__, __LINE__)

namespace cmm
{
    enum class EnumVisitorResultType
    {
        NODE = 0, STRING, NULL_T
    };

    class VisitorResult
    {
    public:

        VisitorResult() CMM_NOEXCEPT;
        VisitorResult(Node* node, const bool owned) CMM_NOEXCEPT;
        VisitorResult(std::string* str, const bool owned) CMM_NOEXCEPT;

        VisitorResult(const VisitorResult&) = delete;
        VisitorResult(VisitorResult&& other) CMM_NOEXCEPT;
        ~VisitorResult();

        VisitorResult& operator= (const VisitorResult&) = delete;
        VisitorResult& operator= (VisitorResult&& other) CMM_NOEXCEPT;

    private:

        void clean() CMM_NOEXCEPT;

    public:

        union
        {
            Node* node;
            std::string* str;
            void* null;
        } result;

        bool owned;
        EnumVisitorResultType resultType;
    };

    class Visitor
    {
    protected:

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
        Visitor(const Visitor&) = delete;

        /**
         * Move constructor
         */
        Visitor(Visitor&&) CMM_NOEXCEPT = delete;

        /**
         * Default destructor
         */
        virtual ~Visitor() = default;

        /**
         * Copy assignment operator.
         */
        Visitor& operator= (const Visitor&) = delete;

        /**
         * Move assignment operator.
         */
        Visitor& operator= (Visitor&&) CMM_NOEXCEPT = delete;

        virtual VisitorResult visit(ArgNode& node) = 0;
        virtual VisitorResult visit(BinOpNode& node) = 0;
        virtual VisitorResult visit(BlockNode& node) = 0;
        virtual VisitorResult visit(CastNode& node) = 0;
        virtual VisitorResult visit(CompilationUnitNode& node) = 0;
        virtual VisitorResult visit(DerefNode& node) = 0;
        virtual VisitorResult visit(EnumDefinitionStatementNode& node) = 0;
        virtual VisitorResult visit(EnumUsageNode& node) = 0;
        virtual VisitorResult visit(ExpressionStatementNode& node) = 0;
        virtual VisitorResult visit(FieldAccessNode& node) = 0;
        virtual VisitorResult visit(FunctionCallNode& node) = 0;
        virtual VisitorResult visit(FunctionDeclarationStatementNode& node) = 0;
        virtual VisitorResult visit(FunctionDefinitionStatementNode& node) = 0;
        virtual VisitorResult visit(IfElseStatementNode& node) = 0;
        virtual VisitorResult visit(LitteralNode& node) = 0;
        virtual VisitorResult visit(ParameterNode& node) = 0;
        virtual VisitorResult visit(ParenExpressionNode& node) = 0;
        virtual VisitorResult visit(ReturnStatementNode& node) = 0;
        virtual VisitorResult visit(StructDefinitionStatementNode& node) = 0;
        virtual VisitorResult visit(StructFwdDeclarationStatementNode& node) = 0;
        virtual VisitorResult visit(TranslationUnitNode& node) = 0;
        virtual VisitorResult visit(TypeNode& node) = 0;
        virtual VisitorResult visit(UnaryOpNode& node) = 0;
        virtual VisitorResult visit(VariableDeclarationStatementNode& node) = 0;
        virtual VisitorResult visit(VariableNode& node) = 0;
        virtual VisitorResult visit(WhileStatementNode& node) = 0;

    protected:

        /**
         * Used throw a std::runtime_error exception for when a visiting function may
         * not be implemented.
         *
         * @param fileName the name of the current file being processed.
         * @param funcName the name of the function where the error occurred.
         * @param line the line in this non-implemented function.
         */
        [[noreturn]]
        static void unimplementedException(const char* fileName, const char* funcName, const std::size_t line)
        {
            std::ostringstream os;
            os << "[BUG]: Unimplemented exception thrown at " << fileName << ": " << funcName << ": " << line;

            throw std::runtime_error(os.str());
        }

    };
}

#endif //!CMM_VISITOR_H

