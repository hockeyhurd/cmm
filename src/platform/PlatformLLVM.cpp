/**
 * A base class for handling platform LLVM specific instructions.
 *
 * @author hockeyhurd
 * @version 2022-11-23
 */

// Our includes
#include <cmm/Types.h>
#include <cmm/visit/Visitor.h>
#include <cmm/platform/PlatformLLVM.h>
#include <cmm/NodeList.h>
#include <cmm/Reporter.h>
#include <cmm/visit/Encode.h>

// std includes
#include <memory>
#include <optional>
#include <sstream>
#include <string>

namespace cmm
{
    /* static */
    const std::string PlatformLLVM::structNamePrefix = "%struct.";

    PlatformLLVM::PlatformLLVM() : PlatformBase("LLVM"), currentTranslationUnit(nullptr)
    {
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::castForBranch(const VisitorResult& name) CMM_NOEXCEPT /* override */
    {
        return std::nullopt;
    }

    /* virtual */
    void PlatformLLVM::emitBlockNodeStart(Encode* encoder) /* override */
    {
        auto& os = encoder->getOStream();
        os << "{";
    }

    /* virtual */
    void PlatformLLVM::emitBlockNodeEnd(Encode* encoder) /* override */
    {
        auto& os = encoder->getOStream();
        os << "\n}";
    }

    /* virtual */
    void PlatformLLVM::emitBranchInstruction(Encode* encoder, const VisitorResult& expr, const std::string& ifLabel,
        const std::string& endLabel, const std::string* elseLabel) /* override */
    {
        encoder->printIndent();
        auto& os = encoder->getOStream();
        os << "br i1 " << *expr.result.str << ", label %" << ifLabel << ", label %";

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

    /* virtual */
    void PlatformLLVM::emitFunctionStart(Encode* encoder, const std::string& name) /* override */
    {
        auto& os = encoder->getOStream();
        os << "@" << name << "(";
    }

    /* virtual */
    void PlatformLLVM::emitFunctionEnd(Encode* encoder) /* override */
    {
        auto& os = encoder->getOStream();
        os << ")";
    }

    /* virtual */
    std::optional<std::string> PlatformLLVM::emitFunctionCallStart(Encode* encoder, const CType& datatype, const std::string& name) /* override */
    {
        auto& os = encoder->getOStream();
        encoder->printIndent();

        if (datatype.type == EnumCType::VOID && !datatype.isPointerType())
        {
            os << "call void @" << name << "(";
            return std::nullopt;
        }

        auto temp = encoder->getTemp();
        const auto datatypeAsString = resolveDatatype(datatype);

        os << temp << " = call " << datatypeAsString << " @" << name << "(";
        return std::make_optional(std::move(temp));
    }

    /* virtual */
    void PlatformLLVM::emitFunctionCallEnd(Encode* encoder) /* override */
    {
        auto& os = encoder->getOStream();
        os << ")";
    }

    /* virtual */
    void PlatformLLVM::emitBranch(Encode* encoder, const std::string& label) /* override */
    {
        encoder->printIndent();
        auto& os = encoder->getOStream();
        os << "br label %" << label;
        encoder->emitNewline();
    }

    /* virtual */
    void PlatformLLVM::emitLabel(Encode* encoder, const std::string& label) /* override */
    {
        auto& os = encoder->getOStream();
        os << label << ":";
        encoder->emitNewline();
    }

    /* virtual */
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
            str = "i8*";
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
        case EnumCType::ENUM:
        // fallthrough
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
            str = "i8*";
            break;
        case EnumCType::STRUCT:
            str = "%struct." + *datatype.optTypeName;
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
        const auto& datatype = node.getDatatype();
        const std::string typeStr = resolveDatatype(datatype);
        auto& os = encoder->getOStream();
        os << typeStr << " " << *expr.result.str;

        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, BinOpNode& node, const VisitorResult& left, const VisitorResult& right) /* override */
    {
        auto referenceDatatype = node.getRight()->getDatatype();
        const auto leftTypeStr = resolveDatatype(node.getLeft()->getDatatype());
        const auto rightTypeStr = resolveDatatype(referenceDatatype);
        const bool isFloatingPoint = referenceDatatype.isFloatingPoint();

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

        bool reverseOperations = true;
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
            reverseOperations = false;

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
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, CastNode& node, VisitorResult&& expr) /* override */
    {
        const auto& fromCType = node.getExpression()->getDatatype();
        const auto& toCType = node.getDatatype();

        // If the cast gets NOP'd, do nothing but return the same expression we
        // were given as the actual result.
        if (node.getCastType() == EnumCastType::NOP)
        {
            return std::move(expr);
        }

        static auto& reporter = Reporter::instance();

        /**
         * This function facilitates the prefix or suffic to an LLVM cast.
         *
         * @param datatype the type to interpret.
         * @return std::string.
         */
        static auto datatypeToStr = [](const CType& datatype) -> std::string
        {
            if (datatype.isFloatingPoint())
            {
                return "fp";
            }

            else if (datatype.isInt())
            {
                // TODO: When we support unsigned types, make this dynamic:
                CMM_CONSTEXPR bool isSignedInt = true;
                return isSignedInt ? "si" : "ui";
            }

            std::ostringstream os;
            os << "unexpected CType (see compiler source code at " << __FILE__ << ": " << __LINE__ << ")";
            reporter.bug(os.str(), Location(0, 0), true);
            return "";
        };

        auto& os = encoder->getOStream();
        auto temp = encoder->getTemp();

        if (!fromCType.isPointerType() && !toCType.isPointerType() &&
            (fromCType.isFloatingPoint() ^ toCType.isFloatingPoint()))
        {
            encoder->printIndent();
            auto temp = encoder->getTemp();
            os << temp << " = " << datatypeToStr(fromCType) << "to" << datatypeToStr(toCType)
               << ' ' << resolveDatatype(fromCType) << ' ' << *expr.result.str << " to " << resolveDatatype(toCType);
            encoder->emitNewline();

            return VisitorResult(new std::string(std::move(temp)), true);
        }

        encoder->printIndent();
        os << temp << " = " << (toCType.isFloatingPoint() ? "fp" : "z");

        switch (node.getCastType())
        {
        case EnumCastType::NARROWING:
            os << "trunc";
            break;
        case EnumCastType::WIDENING:
            os << "ext";
            break;
        default:
        {
            std::ostringstream os;
            os << "unexpected EnumCastType::NOP (see compiler source code at " << __FILE__ << ": " << __LINE__ << ")";
            reporter.bug(os.str(), node.getLocation(), true);
            return VisitorResult();
        }
        }

        os << ' ' << resolveDatatype(fromCType) << ' ' << *expr.result.str << " to " << resolveDatatype(toCType);
        encoder->emitNewline();

        return VisitorResult(new std::string(std::move(temp)), true);
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, DerefNode& node, const VisitorResult& varResult) /* override */
    {
        const auto& datatype = node.getDatatype();
        const auto strType = resolveDatatype(datatype);
        auto temp = encoder->getTemp();
        encoder->printIndent();
        auto& os = encoder->getOStream();

        os << temp << " = load " << strType << ", " << strType << "* " << *varResult.result.str;

        return VisitorResult(new std::string(std::move(temp)), true);
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, EnumDefinitionStatementNode& node) /* override */
    {
        // Do nothing on LLVM.
        return VisitorResult();
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, EnumUsageNode& node) /* override */
    {
        const Enumerator* enumerator = node.getEnumerator();
        const s32 value = enumerator->getValue();

        return VisitorResult(new std::string(std::to_string(value)), true);
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, FieldAccessNode& node, const VisitorResult& expr) /* override */
    {
        std::string temp = encoder->getTemp();
        const CType& structType = node.getExpression()->getDatatype();
        const std::string structTypeStr = resolveDatatype(structType);
        const auto fieldInStructIndex = node.getIndex();

        encoder->printIndent();

        // Note: See https://llvm.org/docs/LangRef.html#getelementptr-instruction for semantics of the 'getelementptr' instruction.
        auto& os = encoder->getOStream();
        os << temp << " = getelementptr inbounds " << structTypeStr
           << ", " << structTypeStr << "* " << *expr.result.str << ", i32 0, i32 " << fieldInStructIndex;

        encoder->emitNewline();

        return VisitorResult(new std::string(std::move(temp)), true);
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, FunctionDeclarationStatementNode& node) /* override */
    {
        auto& os = encoder->getOStream();
        os << "declare";
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, FunctionDefinitionStatementNode& node) /* override */
    {
        auto& os = encoder->getOStream();
        os << "define";
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, LitteralNode& node, const bool defer) /* override */
    {
        static auto& reporter = Reporter::instance();

        static auto assembleCString = [](const std::string& key, const std::string& value) -> std::string
        {
            // For reference: noundef getelementptr inbounds ([14 x i8], [14 x i8]* @.str
            const std::size_t keySize = key.size() + 1;
            std::ostringstream builder;
            builder << "noundef getelementptr inbounds (["
                    << keySize << " x i8], ["
                    << keySize << " x i8]* @"
                    << value << ", i64 0, i64 0)";

            return builder.str();
        };

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
        {
            // Note: we can't just pass the "raw" string as an argument in LLVM.
            // Instead, we use our CStringTable to lookup this pre-computed value.
            // See PlatformLLVM::emit(..., TranslationUnitNode&) for this pre-computation.
            // outputStr = node.getValue().valueString;
            const auto valueIter = currentTranslationUnit->findCString(node.getValue().valueString);
            outputStr = assembleCString(valueIter->first, valueIter->second);
        }
            break;
        case EnumCType::BOOL:
            outputStr = node.getValue().valueBool ? "true" : "false";
            break;
        case EnumCType::CHAR:
            outputStr = std::to_string(node.getValue().valueChar);
            break;
        case EnumCType::ENUM:
            outputStr = std::to_string(node.getValue().valueEnum);
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
        encoder->emitSpace();

        if (optVariableNode.has_value())
        {
            os << "%" << optVariableNode->getName();
            return std::make_optional<VisitorResult>(new std::string(optVariableNode->getName()), true);
        }

        auto tempParam = encoder->getParam();
        os << tempParam;

        return std::make_optional<VisitorResult>(new std::string(std::move(tempParam)), true);
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, ReturnStatementNode& node, const std::optional<VisitorResult>& expr) /* override */
    {
        const auto* datatype = node.getDatatype();
        auto& os = encoder->getOStream();
        encoder->printIndent();

        if (datatype != nullptr && expr.has_value())
        {
            const std::string str = resolveDatatype(*node.getDatatype());
            os << "ret " << str << " " << *expr->result.str;
        }

        else
        {
            os << "ret void";
        }

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
        // Do nothing??
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
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, TranslationUnitNode& node) /* override */
    {
        static auto genStringName = []() -> std::string
        {
            static std::size_t count = 0;

            // Note: should not need to worry about multiple allocations
            // since nominally this should all fit within the capacity
            // of 'small string optimization'.
            std::string baseName = ".str.";
            baseName += std::to_string(count++);

            return baseName;
        };

        // Capture the current transation unit
        // TODO: Figure out how to avoid having a dangling pointer...
        currentTranslationUnit = std::addressof(node);
        TranslationUnitNode::CStringTable& stringLitteralTable = node.getCStringTable();

        // Note: this check is actually important so we don't just blankly emit
        // new lines before exitting this function.
        if (!stringLitteralTable.empty())
        {
            auto& os = encoder->getOStream();

            for (auto& [key, value] : stringLitteralTable)
            {
                // Override the name for future LitteralNode lookups
                value = genStringName();

                // +1 for null termination character
                const std::size_t strSize = key.size() + 1;
                os << "@" << value << " = private unnamed_addr constant [" << strSize << " x i8] c\""
                    << key << "\\00\", align 1";

                encoder->emitNewline();
            }

            encoder->emitNewline();
        }

        // Nothing to ever return, but we'll keep it for consistency.
        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, UnaryOpNode& node, VisitorResult&& expr) /* override */
    {
        static auto& reporter = Reporter::instance();
        const EnumUnaryOpType opType = node.getOpType();
        const auto& datatype = node.getDatatype();
        const bool isFloatingPoint = datatype.isFloatingPoint();

        // TODO: When we support unsigned types, make this dynamic:
        CMM_CONSTEXPR bool isSignedInt = false;

        auto& os = encoder->getOStream();
        std::string temp;

        switch (opType)
        {
        case EnumUnaryOpType::ADDRESS_OF: // &x
        {
            temp = std::move(*expr.result.str);
            goto endUnaryOpNodeLabel;
        }
            break;
        case EnumUnaryOpType::NEGATIVE: // -x
        {
            encoder->printIndent();
            temp = encoder->getTemp();
            os << temp << " = ";

            const std::string typeStr = resolveDatatype(datatype);

            if (isFloatingPoint)
            {
                os << "fneg " << typeStr << " ";
            }

            else
            {
                os << "sub " << (isSignedInt ? "nsw " : "") << typeStr << " 0, ";
            }

            os << *expr.result.str;
        }
            break;
        case EnumUnaryOpType::POSITIVE: // +x
            return std::make_optional<VisitorResult>(std::move(expr));
        case EnumUnaryOpType::INCREMENT: // ++x or x++
        {
            encoder->printIndent();
            temp = encoder->getTemp();
            os << temp << " = ";

            const std::string typeStr = resolveDatatype(datatype);

            if (isFloatingPoint)
            {
                os << "fadd " << typeStr << " " << *expr.result.str << ", 1.000000e+00";
            }

            else
            {
                os << "add " << typeStr << (isSignedInt ? " nsw " : " ") << *expr.result.str << ", 1";
            }

            // If it's postfix, we can either swap with temp or just return the original.
            if (node.isPostfix())
            {
                encoder->emitNewline();
                return std::make_optional<VisitorResult>(std::move(expr));
            }
        }
            break;
        case EnumUnaryOpType::DECREMENT: // --x or x--
        {
            encoder->printIndent();
            temp = encoder->getTemp();
            os << temp << " = ";

            const std::string typeStr = resolveDatatype(datatype);

            if (isFloatingPoint)
            {
                os << "fsub " << typeStr << " " << *expr.result.str << ", 1.000000e+00";
            }

            else
            {
                os << "sub " << typeStr << (isSignedInt ? " nsw " : " ") << *expr.result.str << ", 1";
            }

            // If it's postfix, we can either swap with temp or just return the original.
            if (node.isPostfix())
            {
                encoder->emitNewline();
                return std::make_optional<VisitorResult>(std::move(expr));
            }
        }
            break;
        default:
            reporter.bug("Un-supported EnumUnaryOpType", node.getLocation(), true);
            break;
        }

        encoder->emitNewline();

endUnaryOpNodeLabel:;
        return VisitorResult(new std::string(std::move(temp)), true);
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
        static auto& reporter = Reporter::instance();
        const EnumLocality locality = node.getLocality();
        auto& os = encoder->getOStream();

        switch (locality)
        {
        case (EnumLocality::GLOBAL):
            os << "= external global ";
            printType(os, node.getDatatype());
            os << " 0";
            break;
        case (EnumLocality::INTERNAL):
            os << "= internal global ";
            printType(os, node.getDatatype());
            os << " 0";
            break;
        case (EnumLocality::LOCAL):
            os << "= alloca";
            break;
        case (EnumLocality::PARAMETER):
            reporter.bug("Unimplemented EnumLocality::PARAMETER for PlatformLLVM::emit(Encode* encoder, VariableDeclarationStatementNode& node)", node.getLocation(), true);
            break;
        default:
            reporter.bug("Unimplemented EnumLocality type for PlatformLLVM::emit(Encode* encoder, VariableDeclarationStatementNode& node)", node.getLocation(), true);
            break;
        }

        return std::nullopt;
    }

    /* virtual */
    std::optional<VisitorResult> PlatformLLVM::emit(Encode* encoder, WhileStatementNode& node, const VisitorResult& cond,
            const VisitorResult& statement) /* override */
    {
        return std::nullopt;
    }
}

