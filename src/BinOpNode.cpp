/**
 * An AST node for binary operator ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-06-22
 */

#include <cmm/BinOpNode.h>
#include <cmm/Token.h>

namespace cmm
{

    std::optional<EnumBinOpNodeType> isEnumBinOpType(const Token& token) CMM_NOEXCEPT
    {
        if (token.isCharSymbol())
        {
            switch (token.asCharSymbol())
            {
            case CHAR_PLUS:
                return std::make_optional<EnumBinOpNodeType>(EnumBinOpNodeType::ADD);
            case CHAR_MINUS:
                return std::make_optional<EnumBinOpNodeType>(EnumBinOpNodeType::SUBTRACT);
            case CHAR_ASTERISK: // multiply
                return std::make_optional<EnumBinOpNodeType>(EnumBinOpNodeType::MULTIPLY);
            case CHAR_FORWARD_SLASH: // divide
                return std::make_optional<EnumBinOpNodeType>(EnumBinOpNodeType::DIVIDE);
            default:
                return std::nullopt;
            }
        }

        return std::nullopt;
    }

    BinOpNode::BinOpNode(const Location& location, const EnumBinOpNodeType type,
                         std::unique_ptr<ExpressionNode>&& left,
                         std::unique_ptr<ExpressionNode>&& right) CMM_NOEXCEPT :
        ExpressionNode(NodeType::BIN_OP, location), type(type), left(std::move(left)), right(std::move(right))
    {
    }

    EnumBinOpNodeType BinOpNode::getTypeof() const CMM_NOEXCEPT
    {
        return type;
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

    std::string BinOpNode::toString() const /* override */
    {
        return "BinOpNode";
    }
}

