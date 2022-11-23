/**
 * An implementation of the base visitor class for encoding an AST.
 *
 * @author hockeyhurd
 * @version 2022-11-21
 */

// Our includes
#include <cmm/visit/Encode.h>
#include <cmm/NodeList.h>
#include <cmm/platform/PlatformBase.h>

// std includes
#include <stdexcept>

namespace cmm
{
    Encode::Encode(PlatformBase* platform) : platform(platform)
    {
        if (platform == nullptr)
        {
            throw std::runtime_error("'platform' is a nullptr. Please specify a target platform.");
        }
    }

    std::string Encode::getTemp() const
    {
        static std::size_t count = 0;
        return std::to_string(count++);
    }

    VisitorResult Encode::visit(ArgNode& node)
    {
        auto* expression = node.getExpression();
        auto visitorResult = expression->accept(this);
        auto optVisitorResult = platform->emit(this, node, visitorResult);

        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(BinOpNode& node)
    {
        // TODO: How should we handle the case of: "x = y = 42;"??
        // Ideally, we would like to treat the sub/right-expression as "normal",
        // then re-write the sub/right expression as just another "RVALUE".
        // I guess we'll just need to re-visit this when we get to code generation.

        auto* leftNode = node.getLeft();
        auto leftNodeResult = leftNode->accept(this);

        // Establish the Node's datatype by it's left node.
        const auto& leftType = leftNode->getDatatype();
        node.setDatatype(leftType);

        auto* rightNode = node.getRight();
        auto rightNodeResult = rightNode->accept(this);

        auto optVisitorResult = platform->emit(this, node, leftNodeResult, rightNodeResult);

        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(BlockNode& node)
    {
        scope.push(true);

        for (auto& statementPtr : node)
        {
            statementPtr->accept(this);
        }

        scope.pop();

        return VisitorResult();
    }

    VisitorResult Encode::visit(CastNode& node)
    {
        auto* expression = node.getExpression();
        auto visitorResult = expression->accept(this);
        auto optVisitorResult = platform->emit(this, node, visitorResult);

        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(CompilationUnitNode& node)
    {
        auto result = node.getRoot().accept(this);

        return result;
    }

    VisitorResult Encode::visit(DerefNode& node)
    {
        auto* expression = node.getExpression();
        auto visitorResult = expression->accept(this);
        auto optVisitorResult = platform->emit(this, node, visitorResult);

        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(FunctionCallNode& node)
    {
        std::vector<VisitorResult> results;
        results.reserve(node.size());

        for (auto& arg : node)
        {
            auto result = arg.accept(this);
            results.emplace_back(std::move(result));
        }

        auto optVisitorResult = platform->emit(this, node, results);
        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(FunctionDeclarationStatementNode& node)
    {
        auto& typeNode = node.getTypeNode();
        auto typeNodeVisitorResult = typeNode.accept(this);

        std::vector<VisitorResult> paramResults;
        paramResults.reserve(node.paramCount());

        for (auto& paramNode : node)
        {
            auto result = paramNode.accept(this);
            paramResults.emplace_back(std::move(result));
        }

        platform->emit(this, node, typeNodeVisitorResult, paramResults);

        return VisitorResult();
    }

    VisitorResult Encode::visit(FunctionDefinitionStatementNode& node)
    {
        scope.push(true);

        auto& typeNode = node.getTypeNode();
        auto typeNodeVisitorResult = typeNode.accept(this);

        std::vector<VisitorResult> paramResults;
        paramResults.reserve(node.paramCount());

        for (auto& paramNode : node)
        {
            auto result = paramNode.accept(this);
            paramResults.emplace_back(std::move(result));
        }

        platform->emit(this, node, typeNodeVisitorResult, paramResults);
        platform->emitBlockNodeStart(this);

        auto& blockNode = node.getBlock();
        blockNode.accept(this);

        scope.pop();

        auto* returnStatementPtr = node.getReturnStatement();
        auto returnStatementVisitorResult = returnStatementPtr->accept(this);
        platform->emit(this, *returnStatementPtr, returnStatementVisitorResult);
        platform->emitBlockNodeEnd(this);

        return VisitorResult();
    }

    VisitorResult Encode::visit(ExpressionStatementNode& node)
    {
        auto visitorResult = node.getExpression()->accept(this);

        return visitorResult;
    }

    VisitorResult Encode::visit(IfElseStatementNode& node)
    {
        auto* ifCondExpression = node.getIfConditional();
        auto ifCondVisitorResult = ifCondExpression->accept(this);

        auto* ifStatement = node.getIfStatement();
        auto ifStatementVisitorResult = ifStatement->accept(this);

        auto* elseStatement = node.getElseStatement();
        std::optional<VisitorResult> optElseStatement = std::nullopt;

        if (elseStatement != nullptr)
        {
            optElseStatement = std::make_optional(ifStatement->accept(this));
        }

        platform->emit(this, node, ifCondVisitorResult, ifStatementVisitorResult, optElseStatement);

        return VisitorResult();
    }

    VisitorResult Encode::visit(LitteralNode& node)
    {
        switch (node.getDatatype().type)
        {
        case EnumCType::NULL_T:
            break;
        case EnumCType::VOID:
            break;
        case EnumCType::VOID_PTR:
            break;
        case EnumCType::STRING:
            break;
        case EnumCType::BOOL:
            return VisitorResult(new std::string(node.getValue().valueBool ? "true" : "false"), true);
        case EnumCType::CHAR:
            return VisitorResult(new std::string(std::to_string(node.getValue().valueChar)), true);
        case EnumCType::INT8:
            return VisitorResult(new std::string(std::to_string(node.getValue().valueS8)), true);
        case EnumCType::INT16:
            return VisitorResult(new std::string(std::to_string(node.getValue().valueS16)), true);
        case EnumCType::INT32:
            return VisitorResult(new std::string(std::to_string(node.getValue().valueS32)), true);
        case EnumCType::INT64:
            return VisitorResult(new std::string(std::to_string(node.getValue().valueS64)), true);
        case EnumCType::FLOAT:
            return VisitorResult(new std::string(std::to_string(node.getValue().valueF32)), true);
        case EnumCType::DOUBLE:
            return VisitorResult(new std::string(std::to_string(node.getValue().valueF64)), true);
        case EnumCType::STRUCT:
            break;
        default:
            break;
        }

        return VisitorResult();
    }

    VisitorResult Encode::visit(ParameterNode& node)
    {
        auto& typeNode = node.getDatatype();
        auto typeNodeVisitorResult = typeNode.accept(this);

        auto& optionalVariableNode = node.getVariable();
        std::optional<VisitorResult> optVariableVisitorResult = std::nullopt;

        if (optionalVariableNode.has_value())
        {
            auto result = optionalVariableNode->accept(this);
            optVariableVisitorResult = std::make_optional(std::move(result));
        }

        auto optVisitorResult = platform->emit(this, node, typeNodeVisitorResult, optVariableVisitorResult);

        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(ParenExpressionNode& node)
    {
        auto* expression = node.getExpression();
        auto visitorResult = expression->accept(this);
        auto optVisitorResult = platform->emit(this, node, visitorResult);

        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(ReturnStatementNode& node)
    {
        auto* expression = node.getExpression();
        auto visitorResult = expression->accept(this);
        auto optVisitorResult = platform->emit(this, node, visitorResult);

        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(StructDefinitionStatementNode& node)
    {
        platform->emit(this, node);
        return VisitorResult();
    }

    VisitorResult Encode::visit(StructFwdDeclarationStatementNode& node)
    {
        platform->emit(this, node);
        return VisitorResult();
    }

    VisitorResult Encode::visit(TranslationUnitNode& node)
    {
        for (auto& statement : node)
        {
            statement->accept(this);
        }

        return VisitorResult();
    }

    VisitorResult Encode::visit(TypeNode& node)
    {
        auto optVisitorResult = platform->emit(this, node);
        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(UnaryOpNode& node)
    {
        auto* expression = node.getExpression();
        auto visitorResult = expression->accept(this);
        auto optVisitorResult = platform->emit(this, node, visitorResult);

        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(VariableNode& node)
    {
        auto optVisitorResult = platform->emit(this, node);
        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(VariableDeclarationStatementNode& node)
    {
        auto& typeNode = node.getTypeNode();
        auto typeNodeVisitorResult = typeNode.accept(this);
        auto optVisitorResult = platform->emit(this, node, typeNodeVisitorResult);

        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(WhileStatementNode& node)
    {
        auto* conditional = node.getConditional();
        auto condVisitorResult = conditional->accept(this);

        auto* statement = node.getStatement();
        auto statementVisitorResult = statement->accept(this);
        platform->emit(this, node, condVisitorResult, statementVisitorResult);

        return VisitorResult();
    }
}

