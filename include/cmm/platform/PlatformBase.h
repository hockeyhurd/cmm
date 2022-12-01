/**
 * A base class for handling platform specific instructions.
 *
 * @author hockeyhurd
 * @version 2022-11-21
 */

#pragma once

#ifndef CMM_PLATFORM_BASE_H
#define CMM_PLATFORM_BASE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/NodeListFwd.h>
#include <cmm/visit/Visitor.h>

// std includes
#include <optional>
#include <string>
#include <vector>

namespace cmm
{
    class Encode;
    class VisitorResult;

    class PlatformBase
    {
    protected:

        /**
         * Default constructor.
         *
         * @param name std::string l-value.
         */
        explicit PlatformBase(const std::string& name);

        /**
         * Default constructor.
         *
         * @param name std::string r-value.
         */
        explicit PlatformBase(std::string&& name) CMM_NOEXCEPT;

    public:

        /**
         * Deleted copy constructor.
         */
        PlatformBase(const PlatformBase&) CMM_NOEXCEPT = delete;

        /**
         * Default move constructor.
         */
        PlatformBase(PlatformBase&&) CMM_NOEXCEPT = default;

        /**
         * Default virtual destructor.
         */
        virtual ~PlatformBase() = default;

        /**
         * Deleted copy assignment operator.
         */
        PlatformBase& operator= (const PlatformBase&) CMM_NOEXCEPT = delete;

        /**
         * Default move assignment operator.
         */
        PlatformBase& operator= (PlatformBase&&) CMM_NOEXCEPT = default;

        /**
         * Gets the name of the platform.
         *
         * @return reference to std::string.
         */
        virtual std::string& getName() CMM_NOEXCEPT;

        /**
         * Gets the name of the platform.
         *
         * @return const reference to std::string.
         */
        virtual const std::string& getName() const CMM_NOEXCEPT;

        /**
         * Gets the header (if needed) per backend.
         *
         * @return std::string.
         */
        virtual std::string getHeader() const;

        /**
         * Gets the footer (if needed) per backend.
         *
         * @return std::string.
         */
        virtual std::string getFooter() const;

        virtual void emitBlockNodeStart(Encode* encoder) = 0;
        virtual void emitBlockNodeEnd(Encode* encoder) = 0;
        virtual void emitFunctionStart(Encode* encoder, const std::string& name) = 0;
        virtual void emitFunctionEnd(Encode* encoder) = 0;

        virtual std::string resolveDatatype(const CType& datatype) = 0;

        virtual std::optional<VisitorResult> emit(Encode* encoder, ArgNode& node, const VisitorResult& expr) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, BinOpNode& node, const VisitorResult& left, const VisitorResult& right) = 0;
        // virtual std::optional<VisitorResult> emit(Encode* encoder, BlockNode& node) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, CastNode& node, const VisitorResult& expr) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, DerefNode& node, const VisitorResult& varResult) = 0;
#if 0
        virtual std::optional<VisitorResult> emit(Encode* encoder, FunctionCallNode& node, const std::vector<VisitorResult>& params) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, FunctionDeclarationStatementNode& node, const VisitorResult& type,
            const std::vector<VisitorResult>& params) = 0;
#endif
        virtual std::optional<VisitorResult> emit(Encode* encoder, FunctionDefinitionStatementNode& node) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, IfElseStatementNode& node, const VisitorResult& ifCond,
            const VisitorResult& ifStatement, const std::optional<VisitorResult>& optElseStatement) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, LitteralNode& node, const bool defer) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, ParameterNode& node, const VisitorResult& type,
            const std::optional<VisitorResult>& optExpr) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, ParenExpressionNode& node, const VisitorResult& expr) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, ReturnStatementNode& node, const VisitorResult& expr) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, StructDefinitionStatementNode& node) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, StructFwdDeclarationStatementNode& node) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, TypeNode& node) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, UnaryOpNode& node, const VisitorResult& name) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, VariableNode& node, const bool defer) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, VariableDeclarationStatementNode& node) = 0;
        virtual std::optional<VisitorResult> emit(Encode* encoder, WhileStatementNode& node, const VisitorResult& cond,
            const VisitorResult& statement) = 0;

    protected:

        // The name of the implementing platform.
        std::string name;
    };
}

#endif //!CMM_PLATFORM_BASE_H
