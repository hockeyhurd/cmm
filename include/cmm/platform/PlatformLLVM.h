/**
 * A base class for handling platform LLVM specific instructions.
 *
 * @author hockeyhurd
 * @version 2022-11-23
 */

#pragma once

#ifndef CMM_PLATFORM_LLVM_H
#define CMM_PLATFORM_LLVM_H

// Our includes
#include <cmm/Types.h>
#include <cmm/platform/PlatformBase.h>

namespace cmm
{
    class Encode;
    class VisitorResult;

    class PlatformLLVM : public PlatformBase
    {
    public:

        /**
         * Default constructor.
         */
        PlatformLLVM();

        /**
         * Deleted copy constructor.
         */
        PlatformLLVM(const PlatformLLVM&) CMM_NOEXCEPT = delete;

        /**
         * Default move constructor.
         */
        PlatformLLVM(PlatformLLVM&&) CMM_NOEXCEPT = default;

        /**
         * Default destructor.
         */
        virtual ~PlatformLLVM() = default;

        /**
         * Deleted copy assignment operator.
         */
        PlatformLLVM& operator= (const PlatformLLVM&) CMM_NOEXCEPT = delete;

        /**
         * Default move assignment operator.
         */
        PlatformLLVM& operator= (PlatformLLVM&&) CMM_NOEXCEPT = default;

        virtual void emitBlockNodeStart(Encode* encoder) override;
        virtual void emitBlockNodeEnd(Encode* encoder) override;
        virtual void emitFunctionStart(Encode* encoder, const std::string& name) override;
        virtual void emitFunctionEnd(Encode* encoder) override;

        virtual std::string resolveDatatype(const CType& datatype) override;

        virtual std::optional<VisitorResult> emit(Encode* encoder, ArgNode& node, const VisitorResult& expr) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, BinOpNode& node, const VisitorResult& left, const VisitorResult& right) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, CastNode& node, const VisitorResult& expr) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, DerefNode& node, const VisitorResult& varResult) override;
#if 0
        virtual std::optional<VisitorResult> emit(Encode* encoder, FunctionCallNode& node, const std::vector<VisitorResult>& params) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, FunctionDeclarationStatementNode& node, const VisitorResult& type,
            const std::vector<VisitorResult>& params) override;
#endif
        virtual std::optional<VisitorResult> emit(Encode* encoder, FunctionDefinitionStatementNode& node) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, IfElseStatementNode& node, const VisitorResult& ifCond,
            const VisitorResult& ifStatement, const std::optional<VisitorResult>& optElseStatement) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, LitteralNode& node, const bool defer) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, ParameterNode& node, const VisitorResult& type,
            const std::optional<VisitorResult>& optExpr) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, ParenExpressionNode& node, const VisitorResult& expr) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, ReturnStatementNode& node, const VisitorResult& expr) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, StructDefinitionStatementNode& node) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, StructFwdDeclarationStatementNode& node) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, TypeNode& node) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, UnaryOpNode& node, const VisitorResult& name) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, VariableNode& node, const bool defer) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, VariableDeclarationStatementNode& node) override;
        virtual std::optional<VisitorResult> emit(Encode* encoder, WhileStatementNode& node, const VisitorResult& cond,
            const VisitorResult& statement) override;
    };
}

#endif //!CMM_PLATFORM_LLVM_H
