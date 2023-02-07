/**
 * An AST node for binary operator ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-22
 */

// Our includes
#include <cmm/BinOpNode.h>
#include <cmm/CastNode.h>
#include <cmm/DerefNode.h>
#include <cmm/Token.h>

namespace cmm
{

    BinOpNode::BinOpNode(const Location& location, const EnumBinOpNodeType type,
                         std::unique_ptr<ExpressionNode>&& left,
                         std::unique_ptr<ExpressionNode>&& right) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::BIN_OP, location), type(type), left(std::move(left)), right(std::move(right))
    {
    }

    EnumBinOpNodeType BinOpNode::getTypeof() const CMM_NOEXCEPT
    {
        return type;
    }

    bool BinOpNode::isComparisonOp() const CMM_NOEXCEPT
    {
        switch (type)
        {
        case EnumBinOpNodeType::CMP_EQ:
        case EnumBinOpNodeType::CMP_NE:
        case EnumBinOpNodeType::CMP_GE:
        case EnumBinOpNodeType::CMP_GT:
        case EnumBinOpNodeType::CMP_LE:
        case EnumBinOpNodeType::CMP_LT:
            return true;
        default:
            return false;
        }

        return false;
    }

    ExpressionNode* BinOpNode::getLeft() CMM_NOEXCEPT
    {
        return left.get();
    }

    const ExpressionNode* BinOpNode::getLeft() const CMM_NOEXCEPT
    {
        return left.get();
    }

    ExpressionNode* BinOpNode::getRight() CMM_NOEXCEPT
    {
        return right.get();
    }

    const ExpressionNode* BinOpNode::getRight() const CMM_NOEXCEPT
    {
        return right.get();
    }

    void BinOpNode::castLeft(const CType& newType)
    {
        const auto location = left->getLocation();
        auto tempLeft = std::move(left);
        left = std::make_unique<CastNode>(location, newType, std::move(tempLeft));
    }

    void BinOpNode::castRight(const CType& newType)
    {
        const auto location = right->getLocation();
        auto tempRight = std::move(right);
        right = std::make_unique<CastNode>(location, newType, std::move(tempRight));
    }

    void BinOpNode::derefNodeLeft()
    {
        const auto location = left->getLocation();
        auto tempLeft = std::move(left);
        left = std::make_unique<DerefNode>(location, std::move(tempLeft));
    }

    void BinOpNode::derefNodeRight()
    {
        const auto location = right->getLocation();
        auto tempRight = std::move(right);
        right = std::make_unique<DerefNode>(location, std::move(tempRight));
    }

    void BinOpNode::popDerefNodeLeft()
    {
        auto tempLeft = std::move(left);
        auto* leftExpr = static_cast<DerefNode*>(tempLeft.get());
        left = leftExpr->release();

        // We 'pop' the pointer count to make the datatypes agree.
        auto& datatype = left->getDatatype();
        --datatype.pointers;
    }

    void BinOpNode::popDerefNodeRight()
    {
        auto tempRight = std::move(right);
        auto* rightExpr = static_cast<DerefNode*>(tempRight.get());
        right = rightExpr->release();

        // We 'pop' the pointer count to make the datatypes agree.
        auto& datatype = right->getDatatype();
        --datatype.pointers;
    }

    // TODO @@@: Find a better solution
    void BinOpNode::setLeftNode(ExpressionNode* left) CMM_NOEXCEPT
    {
        this->left.reset(left);
    }

    // TODO @@@: Find a better solution
    void BinOpNode::setRightNode(ExpressionNode* right) CMM_NOEXCEPT
    {
        this->right.reset(right);
    }

    VisitorResult BinOpNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string BinOpNode::toString() const /* override */
    {
        return "BinOpNode";
    }
}

