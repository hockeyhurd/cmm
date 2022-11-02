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
    CompilationUnitNode::CompilationUnitNode() CMM_NOEXCEPT : Node(EnumNodeType::COMPILATION_UNIT, Location()),
        root()
    {
    }

    CompilationUnitNode::CompilationUnitNode(TranslationUnitNode&& translationUnit) CMM_NOEXCEPT :
        Node(EnumNodeType::COMPILATION_UNIT, translationUnit.getLocation()), root(std::move(translationUnit))
    {
    }

    TranslationUnitNode& CompilationUnitNode::getRoot() CMM_NOEXCEPT
    {
        return root;
    }

    const TranslationUnitNode& CompilationUnitNode::getRoot() const CMM_NOEXCEPT
    {
        return root;
    }

    EnumNodeType CompilationUnitNode::getRootType() const CMM_NOEXCEPT
    {
        return root.getType();
    }

    VisitorResult CompilationUnitNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string CompilationUnitNode::toString() const /* override */
    {
        return "CompilationUnitNode";
    }
}

