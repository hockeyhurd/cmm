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

    Encode::Encode(PlatformBase* platform, std::ostream& os) : platform(platform), os(os),
        indent(0), tempVarCounter(0), paramCounter(0)
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

    std::string Encode::getLabel()
    {
        std::string result = "%l_";
        result += std::to_string(labelCounter++);

        return result;
    }

    std::string Encode::getParam()
    {
        std::string result = "%p_";
        result += std::to_string(paramCounter++);

        return result;
    }

    std::string Encode::getTemp()
    {
        std::string result = "%t_";
        result += std::to_string(tempVarCounter++);

        return result;
    }

    void Encode::emitNewline() const CMM_NOEXCEPT
    {
        os << "\n";
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
        emitNewline();

        for (auto& statementPtr : node)
        {
            statementPtr->accept(this);
        }

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
        const auto& datatype = node.getDatatype();
        auto optLabel = platform->emitFunctionCallStart(this, datatype, node.getName());

        std::vector<VisitorResult> results;
        results.reserve(node.size());

        for (auto& arg : node)
        {
            auto result = arg.accept(this);
            results.emplace_back(std::move(result));
        }

        platform->emitFunctionCallEnd(this);
        emitNewline();

        // Note: this std::move will make this non-portable. Leave for now
        // to support LLVM.
        return VisitorResult(new std::string(std::move(*optLabel)), true);
    }

    VisitorResult Encode::visit(FunctionDeclarationStatementNode& node)
    {
        platform->emit(this, node);
        emitSpace();

        auto& typeNode = node.getTypeNode();
        typeNode.accept(this);

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
        // Reset the counter at the start of each function definition since temporary's
        // are only relevant/contained a single function.  Same thing for parameters with
        // "no names".
        tempVarCounter = 0;
        paramCounter = 0;

        platform->emit(this, node);
        emitSpace();

        auto& typeNode = node.getTypeNode();
        typeNode.accept(this);

        emitSpace();
        platform->emitFunctionStart(this, node.getName());

        std::vector<VisitorResult> paramResults;
        paramResults.reserve(node.paramCount());

        const auto endParamIter = node.end();

        for (auto iter = node.begin(); iter != endParamIter; ++iter)
        {
            auto result = iter->accept(this);
            paramResults.emplace_back(std::move(result));

            if (iter + 1 != endParamIter)
            {
                os << ", ";
            }
        }

        platform->emitFunctionEnd(this);
        emitNewline();
        platform->emitBlockNodeStart(this);
        incrementIndent();

        auto& blockNode = node.getBlock();
        blockNode.accept(this);

        decrementIndent();
        platform->emitBlockNodeEnd(this);
        emitNewline();

        // emit a second new line for better readiblity.
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
        auto* ifStatement = node.getIfStatement();
        auto* elseStatement = node.getElseStatement();

        std::string ifLabel = getLabel();
        std::string endLabel = getLabel();
        std::string elseLabel;

        if (elseStatement != nullptr)
        {
            elseLabel = getLabel();
        }

        auto ifCondVisitorResult = ifCondExpression->accept(this);
        platform->emitBranchInstruction(this, ifCondVisitorResult, ifLabel, endLabel, !elseLabel.empty() ? &elseLabel : nullptr);

        platform->emitLabel(this, ifLabel);
        ifStatement->accept(this);

        // TODO: This is may only be for LLVM.  Consider having the Platform handle this
        platform->emitBranch(this, endLabel);

        if (elseStatement != nullptr)
        {
            platform->emitLabel(this, elseLabel);
            // auto elseStatementVisitorResult = elseStatement->accept(this);
            elseStatement->accept(this);

            // TODO: This is may only be for LLVM.  Consider having the Platform handle this
            platform->emitBranch(this, endLabel);
        }

        platform->emitLabel(this, endLabel);

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
        typeNode.accept(this);

        auto& optionalVariableNode = node.getVariable();
        std::optional<VisitorResult> optVariableVisitorResult;

        if (optionalVariableNode.has_value())
        {
            auto result = optionalVariableNode->accept(this);
            optVariableVisitorResult = std::make_optional(std::move(result));
        }

        auto optVisitorResult = platform->emit(this, node);

        return std::move(*optVisitorResult);
    }

    VisitorResult Encode::visit(ParenExpressionNode& node)
    {
        auto* expression = node.getExpression();
        auto visitorResult = expression->accept(this);

        return visitorResult;
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

        // We'll emit a second line to make formatting a little more clean.
        emitNewline();
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
            emitNewline();
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
        auto optVisitorResult = platform->emit(this, node, std::move(visitorResult));

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
        typeNode.accept(this);
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

