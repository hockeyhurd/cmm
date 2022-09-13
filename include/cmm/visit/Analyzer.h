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
#include <unordered_map>
#include <vector>

namespace cmm
{
    // Forward declarations:
    class Reporter;

    class Analyzer : public Visitor
    {
    private:

        enum class EnumFuncState
        {
            DECLARED = 0, DEFINED
        };

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

        virtual VisitorResult visit(AddressOfNode& node) override;
        virtual VisitorResult visit(ArgNode& node) override;
        virtual VisitorResult visit(BinOpNode& node) override;
        virtual VisitorResult visit(BlockNode& node) override;
        virtual VisitorResult visit(CastNode& node) override;
        virtual VisitorResult visit(CompilationUnitNode& node) override;
        virtual VisitorResult visit(DerefNode& node) override;
        virtual VisitorResult visit(FunctionCallNode& node) override;
        virtual VisitorResult visit(FunctionDeclarationStatementNode& node) override;
        virtual VisitorResult visit(FunctionDefinitionStatementNode& node) override;
        virtual VisitorResult visit(ExpressionStatementNode& node) override;
        virtual VisitorResult visit(IfElseStatementNode& node) override;
        virtual VisitorResult visit(LitteralNode& node) override;
        virtual VisitorResult visit(ParameterNode& node) override;
        virtual VisitorResult visit(ParenExpressionNode& node) override;
        virtual VisitorResult visit(ReturnStatementNode& node) override;
        virtual VisitorResult visit(TranslationUnitNode& node) override;
        virtual VisitorResult visit(TypeNode& node) override;
        virtual VisitorResult visit(UnaryOpNode& node) override;
        virtual VisitorResult visit(VariableNode& node) override;
        virtual VisitorResult visit(VariableDeclarationStatementNode& node) override;
        virtual VisitorResult visit(WhileStatementNode& node) override;

    private:

        static EnumCType deduceType(ExpressionNode* expression);

        /**
         * Validates that function already exists or does not requiring updating the internal map.
         *
         * @param name the name of the function.
         * @param state the state to be updated (Note: does not actually update the table).
         * @return bool.
         */
        bool validateFunction(const std::string& name, const EnumFuncState state);

    private:

        // TODO: When we get to structs, we will need some sort of
        // table for tracking this...

        // Our reporter for reporting things.
        static Reporter& reporter;

        // The symbol table wrapped around a stack based scope. 
        ScopeManager scope;

        // A map for keeping track of functions available.
        std::unordered_map<std::string, EnumFuncState> functionTable;

        // For tracking current locality.
        std::stack<EnumLocality, std::vector<EnumLocality>> localityStack;

    };
}

#endif //!CMM_ANALYZER_H

