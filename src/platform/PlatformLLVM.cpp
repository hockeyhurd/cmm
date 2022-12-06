/**
 * A base class for handling platform LLVM specific instructions.
 *
 * @author hockeyhurd
 * @version 2022-11-23
 */

// Our includes
#include <cmm/platform/PlatformLLVM.h>
#include <cmm/NodeList.h>
#include <cmm/Reporter.h>
#include <cmm/visit/Encode.h>

// std includes
#include <sstream>

namespace cmm
{
    /* static */
    const std::string PlatformLLVM::structNamePrefix = "%struct.";

    PlatformLLVM::PlatformLLVM() : PlatformBase("LLVM")
    {
    }

    void PlatformLLVM::emitBlockNodeStart(Encode* encoder) /* override */
    {
        auto& os = encoder->getOStream();
        os << "{";
    }

    void PlatformLLVM::emitBlockNodeEnd(Encode* encoder) /* override */
    {
        auto& os = encoder->getOStream();
        os << "\n}";
    }

    void PlatformLLVM::emitBranchInstruction(Encode* encoder, const VisitorResult& expr, const std::string& ifLabel,
        const std::string& endLabel, const std::string* elseLabel) /* override */
    {
        encoder->printIndent();
        auto& os = encoder->getOStream();
        os << "br i1 " << *expr.result.str << ", label " << ifLabel << ", label ";

        if (elseLabel != nullptr)
        {
            os << *elseLabel;
        }

        else
        {
            os << endLabel;
        }

        encoder->emitNewline();
    }

    void PlatformLLVM::emitFunctionStart(Encode* encoder, const std::string& name) /* override */
    {
        auto& os = encoder->getOStream();
        os << "@" << name << "(";
    }

    void PlatformLLVM::emitFunctionEnd(Encode* encoder) /* override */
    {
        auto& os = encoder->getOStream();
        os << ")";
    }

    std::optional<std::string> PlatformLLVM::emitFunctionCallStart(Encode* encoder, const CType& datatype, const std::string& name) /* override */
    {
        auto& os = encoder->getOStream();
        auto temp = encoder->getTemp();
        const auto datatypeAsString = resolveDatatype(datatype);

        encoder->printIndent();
        os << temp << " = call " << datatypeAsString << " @" << name << "(";

        return std::make_optional(std::move(temp));
    }

    void PlatformLLVM::emitFunctionCallEnd(Encode* encoder) /* override */
    {
        auto& os = encoder->getOStream();
        os << ")";
    }

    void PlatformLLVM::emitJump(Encode* encoder, const std::string& label) /* override */
    {
        encoder->printIndent();
        auto& os = encoder->getOStream();
        os << "br label " << label;
        encoder->emitNewline();
    }

    void PlatformLLVM::emitLabel(Encode* encoder, const std::string& label) /* override */
    {
        auto& os = encoder->getOStream();
        os << label << ":";
        encoder->emitNewline();
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
        auto referenceDatatype = node.getRight()->getDatatype();
        auto leftTypeStr = resolveDatatype(node.getLeft()->getDatatype());
        auto rightTypeStr = resolveDatatype(referenceDatatype);
        const bool isFloatingPoint = referenceDatatype.pointers == 0 && (referenceDatatype.type == EnumCType::FLOAT || referenceDatatype.type == EnumCType::DOUBLE);

        // TODO: When we support unsigned types, make this dynamic:
        CMM_CONSTEXPR bool isSignedInt = false;

        auto& os = encoder->getOStream();
        encoder->printIndent();

        // ASSIGNMENT = 0, ADD, SUBTRACT, MULTIPLY, DIVIDE
        const EnumBinOpNodeType binOpType = node.getTypeof();

        if (binOpType == EnumBinOpNodeType::ASSIGNMENT)
        {
            os << "store " << rightTypeStr << " " << *right.result.str << ", " << leftTypeStr << "* " << *left.result.str;
            return std::nullopt;
        }

        bool reverseOperations = false;
        auto strResult = encoder->getTemp();
        os << strResult << " = ";

        switch (binOpType)
        {
        case EnumBinOpNodeType::ASSIGNMENT:
            break;
        case EnumBinOpNodeType::ADD:
            if (isFloatingPoint)
                os << "fadd ";
            else
                os << "add " << (isSignedInt ? "nsw " : "");

            break;
        case EnumBinOpNodeType::SUBTRACT:
            if (isFloatingPoint)
                os << "fsub ";
            else
                os << "sub " << (isSignedInt ? "nsw " : "");
            break;
        case EnumBinOpNodeType::MULTIPLY:
            if (isFloatingPoint)
                os << "fmul ";
            else
                os << "mul " << (isSignedInt ? "nsw " : "");
            break;
        case EnumBinOpNodeType::DIVIDE:
            if (isFloatingPoint)
                os << "fdiv ";
            else
                os << (isSignedInt ? "sdiv nsw " : "udiv");
            break;
        case EnumBinOpNodeType::CMP_NE:
            reverseOperations = true;

            if (isFloatingPoint)
                os << "fdiv ";
            else
                os << "icmp ne ";
            break;
        default:
            {
                static auto& reporter = Reporter::instance();
                reporter.bug("Un-implemented EnumBinOpNodeType", node.getLocation(), true);
            }

            return std::nullopt;
        }

        os << rightTypeStr << " ";

        if (reverseOperations)
        {
            os << *left.result.str << ", " << *right.result.str;
        }

        else
        {
            os << *right.result.str << ", " << *left.result.str;
        }

        return std::make_optional<VisitorResult>(new std::string(std::move(strResult)), true);
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, CastNode& node, const VisitorResult& expr) /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, DerefNode& node, const VisitorResult& varResult) /* override */
    {
        const auto& datatype = node.getDatatype();
        auto strType = resolveDatatype(datatype);
        auto temp = encoder->getTemp();
        encoder->printIndent();
        auto& os = encoder->getOStream();
        os << temp << " = load " << strType << ", " << strType << "* " << *varResult.result.str;

        return VisitorResult(new std::string(std::move(temp)), true);
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
#endif

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, FunctionDefinitionStatementNode& node) /* override */
    {
        auto& os = encoder->getOStream();
        os << "define";
        return std::nullopt;
    }

#if 0
    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, IfElseStatementNode& node, const VisitorResult& ifCond,
            const VisitorResult& ifStatement, const std::optional<VisitorResult>& optElseStatement) /* override */
    {
        return std::nullopt;
    }
#endif

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, LitteralNode& node, const bool defer) /* override */
    {
        static auto& reporter = Reporter::instance();

        const auto& datatype = node.getDatatype();
        std::string outputStr;

        switch (datatype.type)
        {
        case EnumCType::NULL_T:
            outputStr = "null";
            break;
        // case EnumCType::VOID:
        //     break;
        // case EnumCType::VOID_PTR:
        //     break;
        case EnumCType::STRING:
            outputStr = node.getValue().valueString;
            break;
        case EnumCType::BOOL:
            outputStr = node.getValue().valueBool ? "true" : "false";
            break;
        case EnumCType::CHAR:
            outputStr = std::to_string(node.getValue().valueChar);
            break;
        case EnumCType::INT8:
            outputStr = std::to_string(node.getValue().valueS8);
            break;
        case EnumCType::INT16:
            outputStr = std::to_string(node.getValue().valueS16);
            break;
        case EnumCType::INT32:
            outputStr = std::to_string(node.getValue().valueS32);
            break;
        case EnumCType::INT64:
            outputStr = std::to_string(node.getValue().valueS64);
            break;
        case EnumCType::FLOAT:
            outputStr = std::to_string(node.getValue().valueF32);
            break;
        case EnumCType::DOUBLE:
            outputStr = std::to_string(node.getValue().valueF64);
            break;
        // case EnumCType::STRUCT:
        //     break;
        default:
            reporter.bug("Un-expected enumeration for an EnumCType", node.getLocation(), true);
            break;
        }

        if (!defer)
        {
            auto& os = encoder->getOStream();
            os << outputStr;
        }

        return VisitorResult(new std::string(std::move(outputStr)), true);
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, ParameterNode& node) /* override */
    {
        const auto& optVariableNode = node.getVariable();

        auto& os = encoder->getOStream();
        os << " ";

        if (optVariableNode.has_value())
        {
            os << optVariableNode->getName();
            return std::make_optional<VisitorResult>(new std::string(optVariableNode->getName()), true);
        }

        auto tempParam = encoder->getParam();
        os << tempParam;

        return std::make_optional<VisitorResult>(new std::string(std::move(tempParam)), true);
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, ParenExpressionNode& node, const VisitorResult& expr) /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, ReturnStatementNode& node, const VisitorResult& expr) /* override */
    {
        std::string str = resolveDatatype(*node.getDatatype());

        auto& os = encoder->getOStream();
        encoder->printIndent();
        os << "ret " << str << " " << *expr.result.str;

        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, StructDefinitionStatementNode& node) /* override */
    {
        const auto& nameOfStruct = node.getName();
        auto& os = encoder->getOStream();
        os << PlatformLLVM::structNamePrefix << nameOfStruct;
        os << " = type { ";

        const auto& blockNode = node.getBlockNode();
        const auto endIter = blockNode.cend();

        for (auto iter = blockNode.cbegin(); iter != endIter; ++iter)
        {
            const auto& fieldStatementPtr = *iter;
            const auto& varDeclStatement = dynamic_cast<const VariableDeclarationStatementNode&>(*fieldStatementPtr.get());
            const auto& datatype = varDeclStatement.getDatatype();
            const auto datatypeAsString = resolveDatatype(datatype);
            os << datatypeAsString;

            if (iter + 1 != endIter)
            {
                os << ", ";
            }
        }

        os << " }";
        encoder->emitNewline();

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
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, VariableNode& node, const bool defer) /* override */
    {
        std::ostringstream output;
        output << "%" << node.getName();
        auto* outputStr = new std::string(output.str());

        if (!defer)
        {
            auto& os = encoder->getOStream();
            os << *outputStr;
        }

        return VisitorResult(outputStr, true);
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, VariableDeclarationStatementNode& node) /* override */
    {
        auto& os = encoder->getOStream();
        os << "= alloca";
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, WhileStatementNode& node, const VisitorResult& cond,
            const VisitorResult& statement) /* override */
    {
        return std::nullopt;
    }
}

