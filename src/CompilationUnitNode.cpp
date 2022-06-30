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

    CompilationUnitNode::CompilationUnitNode(std::shared_ptr<Node> node) :
        Node(NodeType::COMPILATION_UNIT), root(node)
    {
    }

    std::shared_ptr<Node> CompilationUnitNode::getRoot() const
    {
        return root;
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

