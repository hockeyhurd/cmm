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
#include <cmm/TranslationUnitNode.h>

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
         * Constructor with translationn unit.
         */
        CompilationUnitNode(TranslationUnitNode&& translationUnit) CMM_NOEXCEPT;

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
         * Gets the root TranslationUnitNode node.  This is needed for unit testing (at least for now).
         * TODO: Consider a better approach and/or re-using our visitor pattern etc.
         *
         * @return TranslationUnitNode pointer.
         */
        TranslationUnitNode& getRoot() CMM_NOEXCEPT;

        /**
         * Gets the root TranslationUnitNode node.  This is needed for unit testing (at least for now).
         * TODO: Consider a better approach and/or re-using our visitor pattern etc.
         *
         * @return const TranslationUnitNode pointer.
         */
        const TranslationUnitNode& getRoot() const CMM_NOEXCEPT;

        /**
         * Gets the Root node.  This is needed for unit testing (at least for now).
         * TODO: Consider a better approach and/or re-using our visitor pattern etc.
         *
         * @return NodeType.
         */
        NodeType getRootType() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;

        std::string toString() const override;

    private:

        // The underlying 'root' translation unit node.
        // TODO: This should be updated to be a vector at some point in order to
        // support multiple translation unit within this compilation unit.
        TranslationUnitNode root;
    };
}

#endif //!CMM_COMPILATION_UNIT_NODE_H

