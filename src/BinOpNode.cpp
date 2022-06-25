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

    std::optional<EnumBinOpNodeType> isEnumBinOpType(const Token& token) noexcept
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

    BinOpNode::BinOpNode(const EnumBinOpNodeType type, std::shared_ptr<Node> left,
                         std::shared_ptr<Node> right) :
        Node(NodeType::BIN_OP), type(type), left(left), right(right)
    {
    }

    EnumBinOpNodeType BinOpNode::getTypeof() const noexcept
    {
        return type;
    }

    std::string BinOpNode::toString() const /* override */
    {
        return "BinOpNode";
    }
}

