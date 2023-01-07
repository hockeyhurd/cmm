/**
 * A class for representing unary operator expressions in ast.
 *
 * @author hockeyhurd
 * @version 2022-09-09
 */

// Our includes
#include <cmm/UnaryOpNode.h>
#include <cmm/DerefNode.h>
#include <cmm/Token.h>

// std includes
#include <unordered_map>

namespace cmm
{
    static std::unordered_map<std::string, EnumUnaryOpType> opTypeTable;

    static void oneTimeInitUnaryOpTypeTable()
    {
        if (opTypeTable.empty())
        {
            opTypeTable.emplace("&", EnumUnaryOpType::ADDRESS_OF);
            opTypeTable.emplace("-", EnumUnaryOpType::NEGATIVE);
            opTypeTable.emplace("+", EnumUnaryOpType::POSITIVE);
            opTypeTable.emplace("--", EnumUnaryOpType::DECREMENT);
            opTypeTable.emplace("++", EnumUnaryOpType::INCREMENT);
        }
    }

    const char* toString(const EnumUnaryOpType opType) CMM_NOEXCEPT
    {
        switch (opType)
        {
        case EnumUnaryOpType::ADDRESS_OF:
            return "ADDRESS_OF";
        case EnumUnaryOpType::NEGATIVE:
            return "NEGATIVE";
        case EnumUnaryOpType::POSITIVE:
            return "POSITIVE";
        case EnumUnaryOpType::DECREMENT:
            return "DECREMENT";
        case EnumUnaryOpType::INCREMENT:
            return "INCREMENT";
        default:
            return "Unknown EnumUnaryOpType";
        }

        return nullptr;
    }

    std::optional<EnumUnaryOpType> getOpType(const std::string& str)
    {
        oneTimeInitUnaryOpTypeTable();

        const auto findResult = opTypeTable.find(str);
        return findResult != opTypeTable.cend() ? std::make_optional(findResult->second) : std::nullopt;
    }

    std::optional<EnumUnaryOpType> getOpType(const Token& token)
    {
        oneTimeInitUnaryOpTypeTable();

        if (token.isStringSymbol())
        {
            return getOpType(token.asStringSymbol());
        }

        else if (token.isCharSymbol())
        {
            const char ch = token.asCharSymbol();

            switch (ch)
            {
            case CHAR_AMPERSAND:
                return EnumUnaryOpType::ADDRESS_OF;
            case CHAR_MINUS:
                return EnumUnaryOpType::NEGATIVE;
            case CHAR_PLUS:
                return EnumUnaryOpType::POSITIVE;
            default:
                return std::nullopt;
            }
        }

        return std::nullopt;
    }

    UnaryOpNode::UnaryOpNode(const EnumNodeType type, const Location& location,
        const EnumUnaryOpType opType, std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT :
        ExpressionNode(type, location), opType(opType), expression(std::move(expression))
    {
    }

    UnaryOpNode::UnaryOpNode(const Location& location, const EnumUnaryOpType opType,
        std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT :
        ExpressionNode(EnumNodeType::UNARY_OP, location), opType(opType), expression(std::move(expression))
    {
    }

    /* virtual */
    EnumUnaryOpType UnaryOpNode::getOpType() const CMM_NOEXCEPT
    {
        return opType;
    }

    /* virtual */
    bool UnaryOpNode::hasExpression() const CMM_NOEXCEPT
    {
        return expression != nullptr;
    }

    /* virtual */
    ExpressionNode* UnaryOpNode::getExpression() CMM_NOEXCEPT
    {
        return expression.get();
    }

    /* virtual */
    const ExpressionNode* UnaryOpNode::getExpression() const CMM_NOEXCEPT
    {
        return expression.get();
    }

    /* virtual */
    void UnaryOpNode::setExpression(std::unique_ptr<ExpressionNode>&& expression) CMM_NOEXCEPT
    {
        this->expression = std::move(expression);
    }

    /* virtual */
    std::optional<EnumNodeType> UnaryOpNode::getExpressionNodeType() const CMM_NOEXCEPT
    {
        return hasExpression() ? std::make_optional(expression->getType()) : std::nullopt;
    }

    /* virtual */
    void UnaryOpNode::derefNode()
    {
        const auto location = expression->getLocation();
        auto temp = std::move(expression);
        expression = std::make_unique<DerefNode>(location, std::move(temp));
    }

    /* virtual */
    VisitorResult UnaryOpNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    /* virtual */
    std::string UnaryOpNode::toString() const /* override */
    {
        return "UnaryOpNode";
    }
}

