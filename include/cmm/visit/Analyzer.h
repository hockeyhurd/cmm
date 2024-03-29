/**
 * An implementation of the base visitor class for analyzing an AST.
 *
 * @author hockeyhurd
 * @version 2022-08-31
 */

#pragma once

#ifndef CMM_ANALYZER_H
#define CMM_ANALYZER_H

// Our includes
#include <cmm/Types.h>
#include <cmm/NodeListFwd.h>
#include <cmm/ScopeManager.h>
#include <cmm/VariableContext.h>
#include <cmm/visit/Visitor.h>

// std includes
#include <stack>
#include <string>
#include <utility>
#include <vector>

namespace cmm
{
    // Forward declarations:
    class Reporter;
    class EnumTable;
    class StructTable;

    class Analyzer : public Visitor
    {
    public:

        /**
         * Constructor with a value.
         */
        Analyzer() CMM_NOEXCEPT;

        /**
         * Copy constructor
         */
        Analyzer(const Analyzer&) = delete;

        /**
         * Move constructor
         */
        Analyzer(Analyzer&&) CMM_NOEXCEPT = delete;

        /**
         * Default destructor
         */
        virtual ~Analyzer() = default;

        /**
         * Copy assignment operator.
         */
        Analyzer& operator= (const Analyzer&) = delete;

        /**
         * Move assignment operator.
         */
        Analyzer& operator= (Analyzer&&) CMM_NOEXCEPT = delete;

        virtual VisitorResult visit(ArgNode& node) override;
        virtual VisitorResult visit(BinOpNode& node) override;
        virtual VisitorResult visit(BlockNode& node) override;
        virtual VisitorResult visit(CastNode& node) override;
        virtual VisitorResult visit(CompilationUnitNode& node) override;
        virtual VisitorResult visit(DerefNode& node) override;
        virtual VisitorResult visit(EnumDefinitionStatementNode& node) override;
        virtual VisitorResult visit(EnumUsageNode& node) override;
        virtual VisitorResult visit(ExpressionStatementNode& node) override;
        virtual VisitorResult visit(FieldAccessNode& node) override;
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

        /**
         * Checks to see if the ExpressionNode is a VariableNode with a EnumLocality
         * of EnumLocality::PARAMETER.  This is a special case primarily for LLVM.
         *
         * @param node pointer to an ExpressionNode.
         * @return bool.
         */
        bool isExpressionNodeAParameterVariable(const ExpressionNode* node);

        /**
         * Validates that function already exists or does not requiring updating the internal map.
         *
         * @param name the name of the function.
         * @param state the state to be updated (Note: does not actually update the table).
         * @return bool.
         */
        bool validateFunction(const std::string& name, const EnumSymState state);

    private:

        // Our reporter for reporting things.
        static Reporter& reporter;

        // The symbol table wrapped around a stack based scope. 
        ScopeManager scope;

        // A map for keeping track of functions available.
        std::unordered_map<std::string, std::pair<EnumSymState, CType>> functionTable;

        // For caching the current translation unit such that we can
        // access some of its tables such as the EnumTable, StructTable, and more.
        TranslationUnitNode* currentTranslationUnitNodePtr;

        // For tracking current locality.
        std::stack<EnumLocality, std::vector<EnumLocality>> localityStack;

    };
}

#endif //!CMM_ANALYZER_H

