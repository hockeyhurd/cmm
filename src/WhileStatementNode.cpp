/**
 * An AST node for representing a while loop.
 *
 * @author hockeyhurd
 * @version 2022-08-21
 */

// Our includes
#include <cmm/WhileStatementNode.h>
#include <cmm/BinOpNode.h>
#include <cmm/DerefNode.h>
#include <cmm/ExpressionNode.h>
#include <cmm/LitteralNode.h>

namespace cmm
{
    WhileStatementNode::WhileStatementNode(const Location& location, std::unique_ptr<ExpressionNode>&& conditional,
        std::unique_ptr<StatementNode>&& statement) CMM_NOEXCEPT : StatementNode(EnumNodeType::WHILE_STATEMENT, location),
        conditional(std::move(conditional)), statement(std::move(statement))
    {
    }

    ExpressionNode* WhileStatementNode::getConditional() CMM_NOEXCEPT
    {
        return conditional.get();
    }

    const ExpressionNode* WhileStatementNode::getConditional() const CMM_NOEXCEPT
    {
        return conditional.get();
    }

    StatementNode* WhileStatementNode::getStatement() CMM_NOEXCEPT
    {
        return statement.get();
    }

    const StatementNode* WhileStatementNode::getStatement() const CMM_NOEXCEPT
    {
        return statement.get();
    }

    void WhileStatementNode::derefConditional()
    {
        const auto location = conditional->getLocation();
        auto temp = std::move(conditional);
        conditional = std::make_unique<DerefNode>(location, std::move(temp));
    }

    void WhileStatementNode::wrapConditionalWithBinOpNode()
    {
        const auto& datatype = conditional->getDatatype();

        // Setup the LitteralNode 'null' value.
        auto comparator = std::make_unique<LitteralNode>(LitteralNode::getNullForType(conditional->getLocation(), datatype));

        // Move conditional to the side temporarily
        auto tempLHS = std::move(conditional);
        const auto location = tempLHS->getLocation();

        // Create the BinOpNode comparision to replace the conditional expression with.
        conditional = std::make_unique<BinOpNode>(location, EnumBinOpNodeType::CMP_NE, std::move(tempLHS), std::move(comparator));

        // Make sure the end datatype matches the original conditional.
        conditional->setDatatype(datatype);
    }

    VisitorResult WhileStatementNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string WhileStatementNode::toString() const /* override */
    {
        return "WhileStatementNode";
    }
}

