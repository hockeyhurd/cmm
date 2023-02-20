/**
 * An AST node for a struct forward declaration statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-10-18
 */

#include <cmm/StructFwdDeclarationStatementNode.h>

namespace cmm
{
    StructFwdDeclarationStatementNode::StructFwdDeclarationStatementNode(const Location& location, TypeNode&& type) CMM_NOEXCEPT :
        StatementNode(EnumNodeType::STRUCT_FWD_DECLARATION, location), type(type)
    {
    }

    TypeNode& StructFwdDeclarationStatementNode::getTypeNode() CMM_NOEXCEPT
    {
        return type;
    }

    const TypeNode& StructFwdDeclarationStatementNode::getTypeNode() const CMM_NOEXCEPT
    {
        return type;
    }

    CType& StructFwdDeclarationStatementNode::getDatatype() CMM_NOEXCEPT
    {
        return type.getDatatype();
    }

    const CType& StructFwdDeclarationStatementNode::getDatatype() const CMM_NOEXCEPT
    {
        return type.getDatatype();
    }

    std::string& StructFwdDeclarationStatementNode::getName() CMM_NOEXCEPT
    {
        return *type.getDatatype().optTypeName;
    }

    const std::string& StructFwdDeclarationStatementNode::getName() const CMM_NOEXCEPT
    {
        return *type.getDatatype().optTypeName;
    }

    VisitorResult StructFwdDeclarationStatementNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string StructFwdDeclarationStatementNode::toString() const /* override */
    {
        return "StructFwdDeclarationStatementNode";
    }
}

