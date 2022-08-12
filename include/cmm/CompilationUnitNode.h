/**
 * An AST node for the entire compilation unit.
 *
 * @author hockeyhurd
 * @version 2022-06-16
 */

#pragma once

#ifndef CMM_COMPILATION_UNIT_NODE_H
#define CMM_COMPILATION_UNIT_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Node.h>

namespace cmm
{
    // Forward declarations
    class StatementNode;

    class CompilationUnitNode : public Node
    {
    public:

        /**
         * Default constructor.
         */
        CompilationUnitNode() CMM_NOEXCEPT;

        /**
         * Constructor with some 'root' like node.
         * TODO: This should eventually be replaced with something more concrete like
         * one or more translation units ('TranslationUnitNode'??).
         */
        CompilationUnitNode(std::unique_ptr<StatementNode>&& root) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        CompilationUnitNode(const CompilationUnitNode&) = delete;

        /**
         * Move constructor.
         */
        CompilationUnitNode(CompilationUnitNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~CompilationUnitNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return CompilationUnitNode reference.
         */
        CompilationUnitNode& operator= (const CompilationUnitNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return CompilationUnitNode reference.
         */
        CompilationUnitNode& operator= (CompilationUnitNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the Root node.  This is needed for unit testing (at least for now).
         * TODO: Consider a better approach and/or re-using our visitor pattern etc.
         *
         * @return const StatementNode pointer.
         */
        StatementNode* getRoot() CMM_NOEXCEPT;

        /**
         * Gets the Root node.  This is needed for unit testing (at least for now).
         * TODO: Consider a better approach and/or re-using our visitor pattern etc.
         *
         * @return StatementNode pointer.
         */
        const StatementNode* getRoot() const CMM_NOEXCEPT;

        /**
         * Gets the Root node.  This is needed for unit testing (at least for now).
         * TODO: Consider a better approach and/or re-using our visitor pattern etc.
         *
         * @return NodeType.
         */
        NodeType getRootType() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override
        {
            return visitor->visit(*this);
        }

        std::string toString() const override;

    private:

        // The underlying 'root' node.
        std::unique_ptr<StatementNode> root;
    };
}

#endif //!CMM_COMPILATION_UNIT_NODE_H

