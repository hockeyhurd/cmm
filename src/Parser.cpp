/**
 * The parser to our compiler.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#include <cmm/Parser.h>
#include <cmm/ArgNode.h>
#include <cmm/BinOpNode.h>
#include <cmm/BlockNode.h>
#include <cmm/CompilationUnitNode.h>
#include <cmm/FunctionCallNode.h>
#include <cmm/FunctionDeclarationStatementNode.h>
#include <cmm/FunctionDefinitionStatementNode.h>
#include <cmm/ExpressionNode.h>
#include <cmm/ExpressionStatementNode.h>
#include <cmm/LitteralNode.h>
#include <cmm/ParameterNode.h>
#include <cmm/ParenExpressionNode.h>
#include <cmm/Snapshot.h>
#include <cmm/Token.h>
#include <cmm/TypeNode.h>
#include <cmm/VariableNode.h>
#include <cmm/VariableDeclarationStatementNode.h>

#include <iostream>
#include <optional>
#include <sstream>
#include <vector>

namespace cmm
{

    // TODO: This should be broken out into a different class or file.
    template<class T>
    [[noreturn]]
    static void unimplementedAbort(const T& message)
    {
        std::cout << "[PARSER][UnimplementedException]: " << message << std::endl;
        std::exit(EXIT_FAILURE);
    }

    /**
     * Checks if we can write an error message if and only if the pointer is
     * NOT 'nullptr' and an  error message does NOT already exist (i.e. is empty).
     *
     * return bool.
     */
    inline static bool canWriteErrorMessage(std::string* errorMessage)
    {
        return errorMessage != nullptr && errorMessage->empty();
    }

    inline static Token newToken()
    {
        return Token('\0', false);
    }

    static bool expectSemicolon(Lexer& lexer, std::string* errorMessage);

    // Statements:
    static std::unique_ptr<Node> parseDeclarationStatement(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<Node> parseExpressionStatement(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<Node> parseStatement(Lexer& lexer, std::string* errorMessage);

    // Other utility parsing functions
    static std::optional<BlockNode> parseBlockStatement(Lexer& lexer, std::string* errorMessage);
    static std::optional<std::vector<ArgNode>> parseFunctionCallArgs(Lexer& lexer, std::string* errorMessage);
    static std::optional<std::vector<ParameterNode>> parseFunctionParameters(Lexer& lexer, std::string* errorMessage);

    // Expression types:
    static std::unique_ptr<ExpressionNode> parseExpression(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<FunctionCallNode> parseFunctionCall(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ParenExpressionNode> parseParenExpression(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseMultiplyDivideBinOpNode(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseAddSubBinOpNode(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseAssignmentBinOpNode(Lexer& lexer, std::string* errorMessage);

    // Terminal nodes:
    static std::unique_ptr<ExpressionNode> parseLitteralOrVariableNode(Lexer& lexer, std::string* errorMessage);
    static std::optional<VariableNode> parseVariableNode(Lexer& lexer, std::string* errorMessage);

    static std::optional<TypeNode> parseType(Lexer& lexer, std::string* errorMessage);

    Parser::Parser(const std::string& input) : lexer(input)
    {
    }

    Parser::Parser(std::string&& input) : lexer(std::move(input))
    {
    }

    std::unique_ptr<CompilationUnitNode> Parser::parseCompilationUnit(std::string* errorMessage)
    {
        if (lexer.completedOrWhitespaceOnly())
        {
            // TODO: Add debug logging??
            return nullptr;
        }

        auto node = parseStatement(lexer, errorMessage);

        // Make sure no other tokens are left in the lexer's token stream.
        if (!lexer.completedOrWhitespaceOnly())
        {
            if (canWriteErrorMessage(errorMessage))
            {
                std::ostringstream os;
                os << "[PARSER]: Error un-parsed tokens at " << lexer.getLocation()
                   << ". This likely indicates an internal bug.";
                *errorMessage = os.str();
            }

            return nullptr;
        }

        // For now since we don't have proper statements, expect a semi-colon here.
        return std::make_unique<CompilationUnitNode>(std::move(node));
    }

    /* static */
    bool expectSemicolon(Lexer& lexer, std::string* errorMessage)
    {
        auto token = newToken();
        const bool lexResult = lexer.nextToken(token, errorMessage);

        return lexResult && token.getType() == TokenType::CHAR_SYMBOL &&
               token.asCharSymbol() == CHAR_SEMI_COLON;
    }

    /* static */
    std::optional<BlockNode> parseBlockStatement(Lexer& lexer, std::string* errorMessage)
    {
        auto snapshot = lexer.snap();
        auto token = newToken();
        bool result = lexer.peekNextToken(token);

        if (!result || !token.isCharSymbol() || token.asCharSymbol() != CHAR_LCURLY_BRACKET)
        {
            lexer.restore(snapshot);
            return std::nullopt;
        }

        // Consume token
        lexer.nextToken(token, errorMessage);

        BlockNode::StatementList statements;
        statements.reserve(0x10);

        //{
        while (true)
        {
            auto currentStatement = parseStatement(lexer, errorMessage);

            if (currentStatement != nullptr)
            {
                statements.push_back(std::unique_ptr<StatementNode>(static_cast<StatementNode*>(currentStatement.release())));
            }

            else
            {
                break;
            }
        }
        //}

        result = lexer.peekNextToken(token);
        if (!result || !token.isCharSymbol() || token.asCharSymbol() != CHAR_RCURLY_BRACKET)
        {
            lexer.restore(snapshot);
            return std::nullopt;
        }

        // Consume token
        lexer.nextToken(token, errorMessage);

        return std::make_optional<BlockNode>(std::move(statements));
    }

    /* static */
    std::unique_ptr<Node> parseExpressionStatement(Lexer& lexer, std::string* errorMessage)
    {
        auto expression = parseExpression(lexer, errorMessage);

        return expectSemicolon(lexer, errorMessage) ?
               std::make_unique<ExpressionStatementNode>(std::move(expression)) :
               nullptr;
    }

    /* static */
    std::unique_ptr<Node> parseStatement(Lexer& lexer, std::string* errorMessage)
    {
        const auto snapshot = lexer.snap();
        auto node = parseDeclarationStatement(lexer, errorMessage);

        if (node == nullptr)
        {
            // TODO: Do we need to restore??
            lexer.restore(snapshot);

            node = parseExpressionStatement(lexer, errorMessage);
        }

        if (node == nullptr)
        {
            // TODO: Do we need to restore??
            lexer.restore(snapshot);
        }

        return node;
    }

    /* static */
    std::optional<std::vector<ArgNode>> parseFunctionCallArgs(Lexer& lexer, std::string* errorMessage)
    {
        auto snapshot = lexer.snap();
        auto token = newToken();
        bool result = lexer.nextToken(token, errorMessage);

        // Expect opening paren (i.e. '(').
        if (!result || !token.isCharSymbol() || token.asCharSymbol() != CHAR_LPAREN)
        {
            lexer.restore(snapshot);
            return std::nullopt;
        }

        // TODO: Parser non-trivial, empty args case.
        result = lexer.nextToken(token, errorMessage);

        // Expect closing paren (i.e. ')').
        if (!result || !token.isCharSymbol() || token.asCharSymbol() != CHAR_RPAREN)
        {
            lexer.restore(snapshot);
            return std::nullopt;
        }

        return std::make_optional(std::vector<ArgNode>());
    }

    /* static */
    std::optional<std::vector<ParameterNode>> parseFunctionParameters(Lexer& lexer, std::string* errorMessage)
    {
        auto snapshot = lexer.snap();
        auto token = newToken();
        bool result = lexer.peekNextToken(token);

        if (result && token.isCharSymbol() && token.asCharSymbol() == CHAR_LPAREN)
        {
            // Capture the token
            lexer.nextToken(token, errorMessage);

            // Lookahead to the next token
            result = lexer.peekNextToken(token);

            if (result)
            {
                std::vector<ParameterNode> params;

                while (result)
                {
                    // No args, capture and return.
                    if (token.isCharSymbol() && token.asCharSymbol() == CHAR_RPAREN)
                    {
                        // Capture the token
                        lexer.nextToken(token, errorMessage);
                        return std::make_optional(std::move(params));
                    }

                    // TODO: For now, not supporting structs.  Assume primitive or error out.
                    else if (token.isStringSymbol())
                    {
                        // This could be 'func(int)' or 'func(int x)', so we check if the variable was parsed or not.
                        auto typeOpt = parseType(lexer, errorMessage);
                        auto variableNodeOpt = parseVariableNode(lexer, errorMessage);

                        // This is the 'func(int x)' case.
                        if (variableNodeOpt.has_value())
                        {
                            // TODO: Should probably verify it is safe to do this
                            // without checking the optional for 'has_value'.
                            params.emplace_back(std::move(*typeOpt), std::move(*variableNodeOpt));
                        }

                        // This is the 'func(int)' case.
                        else
                        {
                            // TODO: Should probably verify it is safe to do this
                            // without checking the optional for 'has_value'.
                            params.emplace_back(std::move(*typeOpt));
                        }
                    }

                    // Failed prediction, restore and continue with the assumption this is just a variable.
                    else
                    {
                        if (canWriteErrorMessage(errorMessage))
                        {
                            std::ostringstream os;
                            os << "[PARSER]: Encountered an un-expected token: "
                                << toString(token.getType()) << " at " << lexer.getLocation();
                            *errorMessage = os.str();
                        }

                        lexer.restore(snapshot);
                        return std::nullopt;
                    }

                    // Peek ahead
                    result = lexer.peekNextToken(token);

                    // Check to see if the next char is a comma and if so consume that now.
                    if (result && token.isCharSymbol() && token.asCharSymbol() == CHAR_COMMA)
                    {
                        // Consume the comma
                        result = lexer.nextToken(token, errorMessage);

                        // Now do the peek ahead again for the next iteration.
                        result = lexer.peekNextToken(token);
                    }
                }

                // If we get to this point, result must be false OR we forget to return.
                // Assume the first case and report as an error.
                if (canWriteErrorMessage(errorMessage))
                {
                    std::ostringstream os;
                    // TODO: Come up with a better error message.
                    os << "[PARSER]: failed to lookahead to the next token at " << lexer.getLocation();
                    *errorMessage = os.str();
                }

                lexer.restore(snapshot);
                return std::nullopt;
            }

            // Failed prediction, restore and continue with the assumption this is just a variable.
            else
            {
                lexer.restore(snapshot);
            }
        }

        return std::nullopt;
    }

    /* static */
    std::unique_ptr<Node> parseDeclarationStatement(Lexer& lexer, std::string* errorMessage)
    {
        auto snapshot = lexer.snap();
        auto type = parseType(lexer, errorMessage);

        if (!type.has_value())
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        snapshot = lexer.snap();
        auto variableNameOpt = parseVariableNode(lexer, errorMessage);

        if (!variableNameOpt.has_value())
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        // Lookahead to see if this is a function declaration or definition before
        // committing to this being a variable.
        auto optionalFunctionArgs = parseFunctionParameters(lexer, errorMessage);

        if (optionalFunctionArgs.has_value())
        {
            auto optionalBlockStatement = parseBlockStatement(lexer, errorMessage);

            // Function definition
            if (optionalBlockStatement.has_value())
            {
                return std::make_unique<FunctionDefinitionStatementNode>(*type, std::move(variableNameOpt->getName()), std::move(*optionalBlockStatement));
            }

            // Function declaration
            else
            {
                return expectSemicolon(lexer, errorMessage) ?
                    std::make_unique<FunctionDeclarationStatementNode>(*type, std::move(variableNameOpt->getName())) :
                    nullptr;
            }
        }

        return expectSemicolon(lexer, errorMessage) ?
               std::make_unique<VariableDeclarationStatementNode>(*type, std::move(*variableNameOpt)) :
               nullptr;
    }

    /* static */
    std::unique_ptr<ExpressionNode> parseExpression(Lexer& lexer, std::string* errorMessage)
    {
#if 0
        const auto snapshot = lexer.snap();
        auto node = parseAssignmentBinOpNode(lexer, errorMessage);

        if (node == nullptr)
        {
            lexer.restore(snapshot);
            node = parseFunctionCall(lexer, errorMessage);
        }

        if (node == nullptr)
        {
            lexer.restore(snapshot);
            node = parseParenExpression(lexer, errorMessage);
        }
#else
        const auto snapshot = lexer.snap();

        // Note: with the additions of function calls, this needs to be a 'higher'
        // priority (i.e. called before parseAssignmentBinOpNode) because it doesn't
        // handle functions, just variables.
        // TODO: Is this a bug??
        std::unique_ptr<ExpressionNode> node = parseFunctionCall(lexer, errorMessage);

        if (node == nullptr)
        {
            lexer.restore(snapshot);
            node = parseAssignmentBinOpNode(lexer, errorMessage);
        }

        if (node == nullptr)
        {
            lexer.restore(snapshot);
            node = parseParenExpression(lexer, errorMessage);
        }
#endif

        return node;
    }

    /* static */
    std::unique_ptr<FunctionCallNode> parseFunctionCall(Lexer& lexer, std::string* errorMessage)
    {
        // Options are:
        // func()
        // func(void) (same as no args, TODO: future capability)
        // func(arg)
        // func(arg0, arg1, ..., argN)
        // func(...) (variadic args, TODO: future capability)

        auto snapshot = lexer.snap();
        auto optionalVariableNode = parseVariableNode(lexer, errorMessage);

        // Did not get a name of the function, early exit.
        if (!optionalVariableNode.has_value())
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        auto optionalArgList = parseFunctionCallArgs(lexer, errorMessage);

        // Has args
        if (optionalArgList.has_value())
        {
            return std::make_unique<FunctionCallNode>(std::move(*optionalVariableNode), std::move(*optionalArgList));
        }

        // No args present, normal variable.
        else
        {
            return nullptr;
            // return std::make_unique<FunctionCallNode>(std::move(*optionalVariableNode));
        }

        // Should be unreachable...
        return nullptr;
    }

    /* static */
    std::unique_ptr<ParenExpressionNode> parseParenExpression(Lexer& lexer, std::string* errorMessage)
    {
        const auto snapshot = lexer.snap();

        auto token = newToken();
        bool lexResult = lexer.nextToken(token);

        // Expect opening '('
        if (!lexResult || !token.isCharSymbol() || token.asCharSymbol() != CHAR_LPAREN)
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        // wrapped expression i.e. (expr)
        auto expression = parseExpression(lexer, errorMessage);
        lexResult = lexer.nextToken(token);

        // Expect closing ')'
        if (!lexResult || !token.isCharSymbol() || token.asCharSymbol() != CHAR_RPAREN)
        {
            lexer.restore(snapshot);

            if (canWriteErrorMessage(errorMessage))
            {
                std::ostringstream os;
                os << "[PARSER]: Error: Expected closing parenthesis at "
                   << lexer.getLocation();
                *errorMessage = os.str();
            }

            return nullptr;
        }

        return std::make_unique<ParenExpressionNode>(std::move(expression));
    }

    /* static */
    std::unique_ptr<ExpressionNode> parseMultiplyDivideBinOpNode(Lexer& lexer, std::string* errorMessage)
    {
        auto left = parseLitteralOrVariableNode(lexer, errorMessage);

        if (left == nullptr)
        {
            return nullptr;
        }

        auto token = newToken();
        bool lexResult = lexer.peekNextToken(token);

        while (lexResult)
        {
            if (token.isCharSymbol() && (token.asCharSymbol() == CHAR_ASTERISK || token.asCharSymbol() == CHAR_FORWARD_SLASH))
            {
                // Valid bin type accept the token
                lexResult = lexer.nextToken(token, errorMessage);
            }

            else
            {
                return left;
            }

            const auto actualBinOp = token.asCharSymbol() == CHAR_ASTERISK ? EnumBinOpNodeType::MULTIPLY : EnumBinOpNodeType::DIVIDE;
            auto right = parseMultiplyDivideBinOpNode(lexer, errorMessage);
            left = std::make_unique<BinOpNode>(actualBinOp, std::move(left), std::move(right));

            // Lookahead for next iteration.
            lexResult = lexer.peekNextToken(token);
        }

        return left;
    }

    /* static */
    std::unique_ptr<ExpressionNode> parseAddSubBinOpNode(Lexer& lexer, std::string* errorMessage)
    {
        auto left = parseMultiplyDivideBinOpNode(lexer, errorMessage);

        if (left == nullptr)
        {
            return nullptr;
        }

        auto token = newToken();
        bool lexResult = lexer.peekNextToken(token);

        while (lexResult)
        {
            if (token.isCharSymbol() && (token.asCharSymbol() == CHAR_PLUS || token.asCharSymbol() == CHAR_MINUS))
            {
                // Valid bin type accept the token
                lexResult = lexer.nextToken(token, errorMessage);
            }

            else
            {
                return left;
            }

            const auto actualBinOp = token.asCharSymbol() == CHAR_PLUS ? EnumBinOpNodeType::ADD: EnumBinOpNodeType::SUBTRACT;
            auto right = parseAddSubBinOpNode(lexer, errorMessage);
            left = std::make_unique<BinOpNode>(actualBinOp, std::move(left), std::move(right));

            // Lookahead for next iteration.
            lexResult = lexer.peekNextToken(token);
        }

        return left;
    }

    /* static */
    std::unique_ptr<ExpressionNode> parseAssignmentBinOpNode(Lexer& lexer, std::string* errorMessage)
    {
        auto left = parseAddSubBinOpNode(lexer, errorMessage);

        if (left == nullptr)
        {
            return nullptr;
        }

        auto token = newToken();
        bool lexResult = lexer.peekNextToken(token);

        while (lexResult)
        {
            if (token.isCharSymbol() && token.asCharSymbol() == CHAR_EQUALS)
            {
                // Valid bin type accept the token
                lexResult = lexer.nextToken(token, errorMessage);
            }

            else
            {
                return left;
            }

            auto right = parseAssignmentBinOpNode(lexer, errorMessage);
            left = std::make_unique<BinOpNode>(EnumBinOpNodeType::ASSIGNMENT, std::move(left), std::move(right));

            // Lookahead for next iteration.
            lexResult = lexer.peekNextToken(token);
        }

        return left;
    }

    /* static */
    std::unique_ptr<ExpressionNode> parseLitteralOrVariableNode(Lexer& lexer, std::string* errorMessage)
    {
        auto token = newToken();
        const bool lexResult = lexer.nextToken(token, errorMessage);

        if (!lexResult)
        {
            return nullptr;
        }

        switch (token.getType())
        {
        case TokenType::BOOL:
            return std::make_unique<LitteralNode>(token.asBool());
        case TokenType::CHAR:
            return std::make_unique<LitteralNode>(token.asChar());
        case TokenType::FLOAT:
            return std::make_unique<LitteralNode>(token.asFloat());
        case TokenType::DOUBLE:
            return std::make_unique<LitteralNode>(token.asDouble());
        case TokenType::INT16:
            return std::make_unique<LitteralNode>(token.asInt16());
        case TokenType::INT32:
            return std::make_unique<LitteralNode>(token.asInt32());
        case TokenType::INT64:
            return std::make_unique<LitteralNode>(token.asInt64());
        case TokenType::SYMBOL:
            return std::make_unique<VariableNode>(token.asStringSymbol());
        // Unimplemented types
        default:
            return nullptr;
        }

        return nullptr;
    }

    /* static */
    std::optional<VariableNode> parseVariableNode(Lexer& lexer, std::string* errorMessage)
    {
        const auto snapshot = lexer.snap();
        auto token = newToken();
        const bool lexResult = lexer.nextToken(token, errorMessage);

        if (!lexResult || !token.isStringSymbol())
        {
            lexer.restore(snapshot);
            return std::nullopt;
        }

        return std::make_optional<VariableNode>(token.asStringSymbol());
    }

    /* static */
    std::optional<TypeNode> parseType(Lexer& lexer, std::string* errorMessage)
    {
        auto token = newToken();
        const bool lexResult = lexer.nextToken(token, errorMessage);

        if (!lexResult || !token.isStringSymbol())
        {
            return std::nullopt;
        }

        auto type = token.asStringSymbol();

        if (isCType(type))
        {
            const auto enumType = getCType(token.asStringSymbol());

            if (enumType.has_value())
            {
                return std::make_optional<TypeNode>(enumType.value());
            }
        }

        return std::nullopt;
    }
}

