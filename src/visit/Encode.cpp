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
    Encode::Encode(PlatformBase* platform, std::ostream& os) : platform(platform), os(os), indent(0)
    {
        if (platform == nullptr)
        {
            throw std::runtime_error("'platform' is a nullptr. Please specify a target platform.");
        }
    }

    std::ostream& Encode::getOStream() CMM_NOEXCEPT
    {
        return os;
    }

    std::string Encode::getTemp() const
    {
        static std::size_t count = 0;

        std::string result = "%temp";
        result += std::to_string(count++);

        return result;
    }

    void Encode::emitNewline() const CMM_NOEXCEPT
    {
        os << "\n";
        // printIndent();
    }

    void Encode::emitSpace() const CMM_NOEXCEPT
    {
        os << " ";
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated"
    VisitorResult Encode::visit(AddressOfNode& node)
    {
        return VisitorResult();
    }
#pragma GCC diagnostic pop

    VisitorResult Encode::visit(ArgNode& node)
    {
        auto* expression = node.getExpression();
        auto visitorResult = expression->accept(this);
        auto optVisitorResult = platform->emit(this, node, visitorResult);

        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(BinOpNode& node)
    {
        auto* rightNode = node.getRight();
        const auto rightNodeResult = rightNode->accept(this);

        auto* leftNode = node.getLeft();
        const auto leftNodeResult = leftNode->accept(this);

        auto optVisitorResult = platform->emit(this, node, leftNodeResult, rightNodeResult);
        emitNewline();

        return optVisitorResult.has_value() ? std::move(*optVisitorResult) : VisitorResult();
    }

    VisitorResult Encode::visit(BlockNode& node)
    {
        incrementIndent();
        emitNewline();
        scope.push(true);

        for (auto& statementPtr : node)
        {
            statementPtr->accept(this);
        }

        scope.pop();
        decrementIndent();

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
        [[maybe_unused]]
        auto visitorResult = expression->accept(this);
        auto optVisitorResult = platform->emit(this, node, visitorResult);
        emitNewline();

        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(FunctionCallNode& node)
    {
        platform->emitFunctionStart(this, node.getName());

        std::vector<VisitorResult> results;
        results.reserve(node.size());

        for (auto& arg : node)
        {
            auto result = arg.accept(this);
            results.emplace_back(std::move(result));
        }

        platform->emitFunctionEnd(this);
        emitNewline();

        // auto optVisitorResult = platform->emit(this, node, results);
        // return std::move(*optVisitorResult);
        return VisitorResult();
    }

    VisitorResult Encode::visit(FunctionDeclarationStatementNode& node)
    {
        auto& typeNode = node.getTypeNode();
        auto typeNodeVisitorResult = typeNode.accept(this);

        emitSpace();
        platform->emitFunctionStart(this, node.getName());

        std::vector<VisitorResult> paramResults;
        paramResults.reserve(node.paramCount());

        for (auto& paramNode : node)
        {
            auto result = paramNode.accept(this);
            paramResults.emplace_back(std::move(result));
        }

        platform->emitFunctionEnd(this);
        emitNewline();

        return VisitorResult();
    }

    VisitorResult Encode::visit(FunctionDefinitionStatementNode& node)
    {
        scope.push(true);

        platform->emit(this, node);
        emitSpace();

        auto& typeNode = node.getTypeNode();
        auto typeNodeVisitorResult = typeNode.accept(this);

        emitSpace();
        platform->emitFunctionStart(this, node.getName());

        std::vector<VisitorResult> paramResults;
        paramResults.reserve(node.paramCount());

        for (auto& paramNode : node)
        {
            auto result = paramNode.accept(this);
            paramResults.emplace_back(std::move(result));
        }

        platform->emitFunctionEnd(this);
        emitNewline();
        platform->emitBlockNodeStart(this);

        auto& blockNode = node.getBlock();
        blockNode.accept(this);

        scope.pop();

        platform->emitBlockNodeEnd(this);
        emitNewline();

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
        auto result = platform->emit(this, node, true);

        return std::move(*result);
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
        platform->emit(this, node, visitorResult);

        return VisitorResult();
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
        // TODO: We don't just want to emit the variable because
        // it may not be the 'latest'.
        auto optVisitorResult = platform->emit(this, node, true);
        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(VariableDeclarationStatementNode& node)
    {
        printIndent();
        auto& variable = node.getVariable();
        platform->emit(this, variable, false);
        emitSpace();

        platform->emit(this, node);
        emitSpace();

        auto& typeNode = node.getTypeNode();
        auto typeNodeVisitorResult = typeNode.accept(this);
        emitNewline();

        return VisitorResult();
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

    void Encode::printIndent() const
    {
        for (s32 i = 0; i < indent; ++i)
        {
            emitSpace();
        }
    }

    void Encode::incrementIndent(const s32 amount) CMM_NOEXCEPT
    {
        indent += amount;
    }

    void Encode::decrementIndent(const s32 amount) CMM_NOEXCEPT
    {
        indent -= amount;

        if (indent < 0)
        {
            indent = 0;
        }
    }
}

