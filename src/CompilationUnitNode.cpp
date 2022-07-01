/**
 * An AST node for the entire compilation unit.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#include <cmm/CompilationUnitNode.h>

namespace cmm
{
    CompilationUnitNode::CompilationUnitNode() CMM_NOEXCEPT : Node(NodeType::COMPILATION_UNIT),
        root(nullptr)
    {
    }

    CompilationUnitNode::CompilationUnitNode(std::unique_ptr<Node>&& root) CMM_NOEXCEPT :
        Node(NodeType::COMPILATION_UNIT), root(std::move(root))
    {
    }

    Node* CompilationUnitNode::getRoot() const CMM_NOEXCEPT
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

