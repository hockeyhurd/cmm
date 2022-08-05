/**
 * An AST node for the entire compilation unit.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

// Our includes
#include <cmm/CompilationUnitNode.h>
#include <cmm/StatementNode.h>

namespace cmm
{
    CompilationUnitNode::CompilationUnitNode() CMM_NOEXCEPT : Node(NodeType::COMPILATION_UNIT),
        root(nullptr)
    {
    }

    CompilationUnitNode::CompilationUnitNode(std::unique_ptr<StatementNode>&& root) CMM_NOEXCEPT :
        Node(NodeType::COMPILATION_UNIT), root(std::move(root))
    {
    }

    StatementNode* CompilationUnitNode::getRoot() CMM_NOEXCEPT
    {
        return root.get();
    }

    const StatementNode* CompilationUnitNode::getRoot() const CMM_NOEXCEPT
    {
        return root.get();
    }

    NodeType CompilationUnitNode::getRootType() const CMM_NOEXCEPT
    {
        return root != nullptr ? root->getType() : NodeType::UNKNOWN;
    }

    std::string CompilationUnitNode::toString() const /* override */
    {
        return "CompilationUnitNode";
    }
}

