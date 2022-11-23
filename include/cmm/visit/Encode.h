/**
 * An implementation of the base visitor class for encoding an AST.
 *
 * @author hockeyhurd
 * @version 2022-11-21
 */

#pragma once

#ifndef CMM_ENCODE_H
#define CMM_ENCODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/NodeListFwd.h>
#include <cmm/ScopeManager.h>
#include <cmm/visit/Visitor.h>

namespace cmm
{
    class PlatformBase;

    class Encode : public Visitor
    {
    public:

        /**
         * Constructor with a platform.
         * Note: Platform mustn't be a nullptr, otherwise an std::runtime_error
         *       exception is thrown.
         *
         * @param platform A pointer to the implementing platform.
         */
        explicit Encode(PlatformBase* platform);

        /**
         * Copy constructor
         */
        Encode(const Encode&) = delete;

        /**
         * Move constructor
         */
        Encode(Encode&&) CMM_NOEXCEPT = delete;

        /**
         * Default destructor
         */
        virtual ~Encode() = default;

        /**
         * Copy assignment operator.
         */
        Encode& operator= (const Encode&) = delete;

        /**
         * Move assignment operator.
         */
        Encode& operator= (Encode&&) CMM_NOEXCEPT = delete;

        /**
         * Gets a unique temp variable.
         *
         * @return std::string.
         */
        std::string getTemp() const;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated"
        virtual VisitorResult visit(AddressOfNode& node) override;
#pragma GCC diagnostic pop
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

        // A pointer to the current platform.
        PlatformBase* platform;

        // In this context, ScopeManager will re-use variables
        // for all labels/asm tags.
        // @@@ TODO: Do we really need this??
        ScopeManager scope;

    };
}

#endif //!CMM_ENCODE_H

