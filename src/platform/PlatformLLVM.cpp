/**
 * A base class for handling platform LLVM specific instructions.
 *
 * @author hockeyhurd
 * @version 2022-11-23
 */

// Our includes
#include <cmm/platform/PlatformLLVM.h>
#include <cmm/NodeList.h>
#include <cmm/visit/Encode.h>

namespace cmm
{
    PlatformLLVM::PlatformLLVM() : PlatformBase("LLVM")
    {
    }

    void PlatformLLVM::emitBlockNodeStart(Encode* encoder) /* override */
    {
        auto& os = encoder->getOStream();
        os << "{\n";
    }

    void PlatformLLVM::emitBlockNodeEnd(Encode* encoder) /* override */
    {
        auto& os = encoder->getOStream();
        os << "\n}\n";
    }

    void PlatformLLVM::emitFunctionStart(Encode* encoder, const std::string& name) /* override */
    {
        auto& os = encoder->getOStream();
        os << name << "(";
    }

    void PlatformLLVM::emitFunctionEnd(Encode* encoder) /* override */
    {
        auto& os = encoder->getOStream();
        os << ")";
    }

    std::string PlatformLLVM::resolveDatatype(const CType& datatype) /* override */
    {
        std::string str;

        switch (datatype.type)
        {
        case EnumCType::NULL_T:
            str = "null";
            break;
        case EnumCType::VOID:
            str = "void";
            break;
        case EnumCType::VOID_PTR:
            str = "ptr";
            break;
        case EnumCType::BOOL:
        // fallthrough
        case EnumCType::CHAR:
        // fallthrough
        case EnumCType::INT8:
            str = "i8";
            break;
        case EnumCType::INT16:
            str = "i16";
            break;
        case EnumCType::INT32:
            str = "i32";
            break;
        case EnumCType::INT64:
            str = "i64";
            break;
        case EnumCType::FLOAT:
            str = "float";
            break;
        case EnumCType::DOUBLE:
            str = "double";
            break;
        case EnumCType::STRING:
            str = "ptr";
            break;
        case EnumCType::STRUCT:
            str = "struct." + *datatype.optStructName;
            break;
        default:
            str = "Unknown type";
            break;
        }

        return str;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, ArgNode& node, const VisitorResult& expr) /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, BinOpNode& node, const VisitorResult& left, const VisitorResult& right) /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, CastNode& node, const VisitorResult& expr) /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, DerefNode& node, const VisitorResult& expr) /* override */
    {
        return std::nullopt;
    }

#if 0
    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, FunctionCallNode& node, const std::vector<VisitorResult>& params) /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, FunctionDeclarationStatementNode& node, const VisitorResult& type,
            const std::vector<VisitorResult>& params) /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, FunctionDefinitionStatementNode& node, const VisitorResult& type,
            const std::vector<VisitorResult>& params) /* override */
    {
        auto& os = encoder->getOStream();
        os << node.getName();
        return std::nullopt;
    }
#endif

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, IfElseStatementNode& node, const VisitorResult& ifCond,
            const VisitorResult& ifStatement, const std::optional<VisitorResult>& optElseStatement) /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, LitteralNode& node) /* override */
    {
        auto& os = encoder->getOStream();
        const auto& datatype = node.getDatatype();

        switch (datatype.type)
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
            os << (node.getValue().valueBool ? "true" : "false");
            break;
        case EnumCType::CHAR:
            os << (std::to_string(node.getValue().valueChar));
            break;
        case EnumCType::INT8:
            os << (std::to_string(node.getValue().valueS8));
            break;
        case EnumCType::INT16:
            os << (std::to_string(node.getValue().valueS16));
            break;
        case EnumCType::INT32:
            os << (std::to_string(node.getValue().valueS32));
            break;
        case EnumCType::INT64:
            os << (std::to_string(node.getValue().valueS64));
            break;
        case EnumCType::FLOAT:
            os << (std::to_string(node.getValue().valueF32));
            break;
        case EnumCType::DOUBLE:
            os << (std::to_string(node.getValue().valueF64));
            break;
        case EnumCType::STRUCT:
            break;
        default:
            break;
        }

        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, ParameterNode& node, const VisitorResult& type,
            const std::optional<VisitorResult>& optExpr) /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, ParenExpressionNode& node, const VisitorResult& expr) /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, ReturnStatementNode& node) /* override */
    {
        std::string str = resolveDatatype(*node.getDatatype());

        auto& os = encoder->getOStream();
        os << "ret " << str;

        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, StructDefinitionStatementNode& node) /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, StructFwdDeclarationStatementNode& node) /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, TypeNode& node) /* override */
    {
        const auto& datatype = node.getDatatype();
        std::string str = resolveDatatype(datatype);

        auto& os = encoder->getOStream();
        os << str;

        return VisitorResult(new std::string(std::move(str)), true);
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, UnaryOpNode& node, const VisitorResult& name) /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, VariableNode& node) /* override */
    {
        auto& os = encoder->getOStream();
        os << "%" << node.getName();
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, VariableDeclarationStatementNode& node, const VisitorResult& type) /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, WhileStatementNode& node, const VisitorResult& cond,
            const VisitorResult& statement) /* override */
    {
        return std::nullopt;
    }
}

