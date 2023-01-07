/**
 * An implementation of the base visitor class for dumping an AST.
 *
 * @author hockeyhurd
 * @version 2022-08-24
 */

#pragma once

#ifndef CMM_DUMP_H
#define CMM_DUMP_H

// Our includes
#include <cmm/Types.h>
#include <cmm/NodeListFwd.h>
#include <cmm/visit/Visitor.h>

namespace cmm
{
    class Dump : public Visitor
    {
    public:

        /**
         * Constructor with a value.
         */
        Dump() CMM_NOEXCEPT;

        /**
         * Copy constructor
         */
        Dump(const Dump&) = delete;

        /**
         * Move constructor
         */
        Dump(Dump&&) CMM_NOEXCEPT = delete;

        /**
         * Default destructor
         */
        virtual ~Dump() = default;

        /**
         * Copy assignment operator.
         */
        Dump& operator= (const Dump&) = delete;

        /**
         * Move assignment operator.
         */
        Dump& operator= (Dump&&) CMM_NOEXCEPT = delete;

        virtual VisitorResult visit(ArgNode& node) override;
        virtual VisitorResult visit(BinOpNode& node) override;
        virtual VisitorResult visit(BlockNode& node) override;
        virtual VisitorResult visit(CastNode& node) override;
        virtual VisitorResult visit(CompilationUnitNode& node) override;
        virtual VisitorResult visit(DerefNode& node) override;
        virtual VisitorResult visit(ExpressionStatementNode& node) override;
        virtual VisitorResult visit(FunctionCallNode& node) override;
        virtual VisitorResult visit(FunctionDeclarationStatementNode& node) override;
        virtual VisitorResult visit(FunctionDefinitionStatementNode& node) override;
        virtual VisitorResult visit(IfElseStatementNode& node) override;
        virtual VisitorResult visit(LitteralNode& node) override;
        virtual VisitorResult visit(ParameterNode& node) override;
        virtual VisitorResult visit(ParenExpressionNode& node) override;
        virtual VisitorResult visit(ReturnStatementNode& node) override;
        virtual VisitorResult visit(StructDefinitionStatementNode& node) override;
        virtual VisitorResult visit(StructFwdDeclarationStatementNode& node) override;
        virtual VisitorResult visit(TranslationUnitNode& node) override;
        virtual VisitorResult visit(TypeNode& node) override;
        virtual VisitorResult visit(UnaryOpNode& node) override;
        virtual VisitorResult visit(VariableDeclarationStatementNode& node) override;
        virtual VisitorResult visit(VariableNode& node) override;
        virtual VisitorResult visit(WhileStatementNode& node) override;

    private:

        void increaseIntentation(const s32 amount = 4) CMM_NOEXCEPT;
        void decreaseIntentation(const s32 amount = 4) CMM_NOEXCEPT;
        void printIndentation() const;
        void printNewLine() const;

    private:

        // The current indentation
        s32 indent;
    };
}

#endif //!CMM_DUMP_H

