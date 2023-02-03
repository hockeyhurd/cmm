/**
 * An AST node for the current translation unit.
 *
 * @author hockeyhurd
 * @version 2022-08-15
 */

// Our includes
#include <cmm/TranslationUnitNode.h>
#include <cmm/StatementNode.h>

namespace cmm
{
    TranslationUnitNode::TranslationUnitNode() CMM_NOEXCEPT : Node(EnumNodeType::TRANSLATION_UNIT, Location())
    {
    }

    TranslationUnitNode::TranslationUnitNode(const Location& location, StatementList&& statements) CMM_NOEXCEPT :
        Node(EnumNodeType::TRANSLATION_UNIT, location), statements(std::move(statements))
    {
    }

    bool TranslationUnitNode::empty() const CMM_NOEXCEPT
    {
        return statements.empty();
    }

    std::size_t TranslationUnitNode::size() const CMM_NOEXCEPT
    {
        return statements.size();
    }

    StructTable& TranslationUnitNode::getStructTable() CMM_NOEXCEPT
    {
        return structTable;
    }

    const StructTable& TranslationUnitNode::getStructTable() const CMM_NOEXCEPT
    {
        return structTable;
    }

    StructTable* TranslationUnitNode::getStructTablePtr() CMM_NOEXCEPT
    {
        return &structTable;
    }

    const StructTable* TranslationUnitNode::getStructTablePtr() const CMM_NOEXCEPT
    {
        return &structTable;
    }

    TranslationUnitNode::StatementListIter TranslationUnitNode::begin() CMM_NOEXCEPT
    {
        return statements.begin();
    }

    const TranslationUnitNode::StatementListConstIter TranslationUnitNode::cbegin() const CMM_NOEXCEPT
    {
        return statements.cbegin();
    }

    TranslationUnitNode::StatementListIter TranslationUnitNode::end() CMM_NOEXCEPT
    {
        return statements.end();
    }

    const TranslationUnitNode::StatementListConstIter TranslationUnitNode::cend() const CMM_NOEXCEPT
    {
        return statements.cend();
    }

    VisitorResult TranslationUnitNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string TranslationUnitNode::toString() const /* override */
    {
        return "TranslationUnitNode";
    }
}

