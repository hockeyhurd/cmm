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
    class Node;

    class CompilationUnitNode : public Node
    {
    public:

        /**
         * Default constructor.
         */
        CompilationUnitNode() noexcept;

        /**
         * Constructor with some 'root' like node.
         * TODO: This should eventually be replaced with something more concrete like
         * one or more translation units ('TranslationUnitNode'??).
         */
        CompilationUnitNode(std::shared_ptr<Node> node);

        /**
         * Copy constructor.
         */
        CompilationUnitNode(const CompilationUnitNode&) noexcept = default;

        /**
         * Move constructor.
         */
        CompilationUnitNode(CompilationUnitNode&&) noexcept = default;

        /**
         * Destructor
         */
        ~CompilationUnitNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return CompilationUnitNode reference.
         */
        CompilationUnitNode& operator= (const CompilationUnitNode&) noexcept = default;

        /**
         * Move assignment operator.
         *
         * @return CompilationUnitNode reference.
         */
        CompilationUnitNode& operator= (CompilationUnitNode&&) noexcept = default;

        /**
         * Gets the Root node.  This is needed for unit testing (at least for now).
         * TODO: Consider a better approach and/or re-using our visitor pattern etc.
         *
         * @return std::shared_ptr<Node>.
         */
        std::shared_ptr<Node> getRoot() const;

        /**
         * Gets the Root node.  This is needed for unit testing (at least for now).
         * TODO: Consider a better approach and/or re-using our visitor pattern etc.
         *
         * @return NodeType.
         */
        NodeType getRootType() const noexcept;

        /**
         * Generic and templated function needed for visitor pattern.
         */
        template<class ReturnT, class DerivedT, class VisitorT>
        ReturnT accept(VisitorT& visitor)
        {
            return visitor.visit(*std::static_pointer_cast<DerivedT>(shared_from_this()));
        }

        std::string toString() const override;

    private:

        // The underlying 'root' node.
        std::shared_ptr<Node> root;
    };
}

#endif //!CMM_COMPILATION_UNIT_NODE_H

