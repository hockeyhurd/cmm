/**
 * The parser to our compiler.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

// Our includes
#include <cmm/Parser.h>
#include <cmm/NodeList.h>
#include <cmm/Reporter.h>
#include <cmm/Snapshot.h>
#include <cmm/Token.h>

// std includes
#include <iostream>
#include <cstring>
#include <optional>
#include <sstream>
#include <vector>

#if OS_WIN
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

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
    static std::unique_ptr<StatementNode> parseDeclarationStatement(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<StatementNode> parseExpressionStatement(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<StatementNode> parseIfElseStatement(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<StatementNode> parseReturnStatement(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<StatementNode> parseStatement(Lexer& lexer, std::string* errorMessage);

    // Other utility parsing functions
    static std::optional<BlockNode> parseBlockStatement(Lexer& lexer, std::string* errorMessage);
    static std::optional<std::vector<ArgNode>> parseFunctionCallArgs(Lexer& lexer, std::string* errorMessage);
    static std::optional<std::vector<ParameterNode>> parseFunctionParameters(Lexer& lexer, std::string* errorMessage);

    // Expression types:
    static std::unique_ptr<ExpressionNode> parseExpression(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseFunctionCallOrVariable(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ParenExpressionNode> parseParenExpression(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseMultiplyDivideBinOpNode(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseAddSubBinOpNode(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseAssignmentBinOpNode(Lexer& lexer, std::string* errorMessage);

    // Terminal nodes:
    static std::unique_ptr<ExpressionNode> parseLitteralOrLRValueNode(Lexer& lexer, std::string* errorMessage);
    static std::optional<VariableNode> parseVariableNode(Lexer& lexer, std::string* errorMessage);

    static std::optional<TypeNode> parseType(Lexer& lexer, std::string* errorMessage);

    Parser::Parser(const std::string& input) : lexer(input)
    {
    }

    Parser::Parser(std::string&& input) CMM_NOEXCEPT : lexer(std::move(input))
    {
    }

    std::unique_ptr<CompilationUnitNode> Parser::parseCompilationUnit(std::string* errorMessage)
    {
        static Reporter& reporter = Reporter::instance();

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
                *errorMessage = "Un-parsed tokens remaining. This likely indicates an internal bug.";
                reporter.error(*errorMessage, lexer.getLocation());
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
    std::unique_ptr<StatementNode> parseExpressionStatement(Lexer& lexer, std::string* errorMessage)
    {
        auto expression = parseExpression(lexer, errorMessage);

        return expectSemicolon(lexer, errorMessage) ?
               std::make_unique<ExpressionStatementNode>(std::move(expression)) :
               nullptr;
    }

    /* static */
    std::unique_ptr<StatementNode> parseIfElseStatement(Lexer& lexer, std::string* errorMessage)
    {
        const auto snapshot = lexer.snap();
        auto token = newToken();
        bool result = lexer.nextToken(token, errorMessage);

        if (!result || !token.isStringSymbol() || token.asStringSymbol() != "if")
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        // Expect open paren before conditional.
        result = lexer.nextToken(token, errorMessage);

        if (!result || !token.isCharSymbol() || token.asCharSymbol() != CHAR_LPAREN)
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        // Expect condition
        auto expression = parseExpression(lexer, errorMessage);

        // Expression MUST exist (i.e. not null).
        if (expression == nullptr)
        {
            if (canWriteErrorMessage(errorMessage))
            {
                std::ostringstream os;
                os << "[PARSER]: Expected a condition expression following the start of an 'if' statement at "
                   << lexer.getLocation();
                *errorMessage = os.str();
            }

            lexer.restore(snapshot);
            return nullptr;
        }

        // Expect closing paren after conditional.
        result = lexer.nextToken(token, errorMessage);

        if (!result || !token.isCharSymbol() || token.asCharSymbol() != CHAR_RPAREN)
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        // Expect some statement next.
        auto statement = parseStatement(lexer, errorMessage);

        // statement MUST exist (i.e. not null).
        if (statement == nullptr)
        {
            if (canWriteErrorMessage(errorMessage))
            {
                std::ostringstream os;
                os << "[PARSER]: Expected a statement following the an 'if' condition at "
                   << lexer.getLocation();
                *errorMessage = os.str();
            }

            lexer.restore(snapshot);
            return nullptr;
        }

        auto* rawStatement = static_cast<StatementNode*>(statement.release());
        std::unique_ptr<StatementNode> ifStatementPtr(rawStatement);

        // Lookahead to see if we have an 'else' part.
        const auto elseSnapshot = lexer.snap();
        result = lexer.nextToken(token, errorMessage);

        // Expect else statement next
        if (result && token.isStringSymbol() && token.asStringSymbol() == "else")
        {
            statement = parseStatement(lexer, errorMessage);

            auto* rawStatement = static_cast<StatementNode*>(statement.release());
            std::unique_ptr<StatementNode> elseStatementPtr(rawStatement);
            return std::make_unique<IfElseStatementNode>(std::move(expression), std::move(ifStatementPtr), std::move(elseStatementPtr));
        }

        // No else block, restore and continue with just the if block.
        else
        {
            lexer.restore(elseSnapshot);
        }

        return std::make_unique<IfElseStatementNode>(std::move(expression), std::move(ifStatementPtr));
    }

    /* static */
    std::unique_ptr<StatementNode> parseReturnStatement(Lexer& lexer, std::string* errorMessage)
    {
        const auto snapshot = lexer.snap();
        auto token = newToken();
        bool result = lexer.nextToken(token, errorMessage);

        if (!result || !token.isStringSymbol() || token.asStringSymbol() != "return")
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        // Optional expression.  Try and lookahead to see if something is there
        result = lexer.peekNextToken(token);

        if (!result)
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        // Must be a semi-colon, expect end of return statement.
        if (token.isCharSymbol() && token.asCharSymbol() == CHAR_SEMI_COLON)
        {
            // No need to consume the token after peek since 'expectSemicolon'
            // will handle this for us.
            return expectSemicolon(lexer, errorMessage) ?
                std::make_unique<ReturnStatementNode>() :
                nullptr;
        }

        // Else, must contain an expression
        auto expression = parseExpression(lexer, errorMessage);

        return expectSemicolon(lexer, errorMessage) ?
               std::make_unique<ReturnStatementNode>(std::move(expression)) :
               nullptr;
    }

    /* static */
    std::unique_ptr<StatementNode> parseStatement(Lexer& lexer, std::string* errorMessage)
    {
        const auto snapshot = lexer.snap();
        auto node = parseReturnStatement(lexer, errorMessage);

        if (node == nullptr)
        {
            lexer.restore(snapshot);
            node = parseIfElseStatement(lexer, errorMessage);
        }

        else
        {
            return node;
        }

        if (node == nullptr)
        {
            lexer.restore(snapshot);
            node = parseDeclarationStatement(lexer, errorMessage);
        }

        else
        {
            return node;
        }

        if (node == nullptr)
        {
            lexer.restore(snapshot);
            auto optionalBlockNode = parseBlockStatement(lexer, errorMessage);

            if (optionalBlockNode.has_value())
            {
                node = std::make_unique<BlockNode>(std::move(*optionalBlockNode));
            }
        }

        else
        {
            return node;
        }

        if (node == nullptr)
        {
            lexer.restore(snapshot);
            node = parseExpressionStatement(lexer, errorMessage);
        }

        else
        {
            return node;
        }

        if (node == nullptr)
        {
            lexer.restore(snapshot);
        }

        return node;
    }

    /* static */
    std::optional<std::vector<ArgNode>> parseFunctionCallArgs(Lexer& lexer, std::string* errorMessage)
    {
        auto snapshot = lexer.snap();
        auto token = newToken();
        bool result = lexer.peekNextToken(token);

        // Expect opening paren (i.e. '(').
        if (result && token.isCharSymbol() && token.asCharSymbol() == CHAR_LPAREN)
        {
            // Capture the token
            result = lexer.nextToken(token, errorMessage);

            // Lookahead to the next token
            result = lexer.peekNextToken(token);

            if (result)
            {
                std::vector<ArgNode> args;

                while (result)
                {
                    // No args, capture and return.
                    if (token.isCharSymbol() && token.asCharSymbol() == CHAR_RPAREN)
                    {
                        // Capture the token
                        lexer.nextToken(token, errorMessage);
                        return std::make_optional(std::move(args));
                    }

                    // else if (token.isStringSymbol())
                    else if (!token.isCharSymbol())
                    // else
                    {
                        // This could be 'func()' or 'func(x)', so we check if the variable was parsed or not.
#if 0
                        auto variableNodeOpt = parseVariableNode(lexer, errorMessage);

                        // This is the 'func(int x)' case.
                        if (variableNodeOpt.has_value())
                        {
                            // TODO: Should probably verify it is safe to do this
                            // without checking the optional for 'has_value'.
                            args.emplace_back(std::move(*variableNodeOpt));
                        }
#else
                        auto litteralPtr = parseLitteralOrLRValueNode(lexer, errorMessage);

                        if (litteralPtr != nullptr)
                        {
                            args.emplace_back(std::move(litteralPtr));
                        }
#endif
                    }

                    // Failed prediction, restore and continue with the assumption this is just a variable.
                    /*else
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
                    }*/

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
    std::optional<std::vector<ParameterNode>> parseFunctionParameters(Lexer& lexer, std::string* errorMessage)
    {
        static auto& reporter = Reporter::instance();

        auto snapshot = lexer.snap();
        auto token = newToken();
        bool result = lexer.peekNextToken(token);

        // Expect opening paren (i.e. '(').
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
                            os << "Encountered an un-expected token: " << toString(token.getType());
                            *errorMessage = os.str();

                            reporter.error(*errorMessage, lexer.getLocation());
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
                    os << "Failed to lookahead to the next token at " << lexer.getLocation();
                    *errorMessage = os.str();

                    reporter.error(*errorMessage, lexer.getLocation());
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
    std::unique_ptr<StatementNode> parseDeclarationStatement(Lexer& lexer, std::string* errorMessage)
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
        const auto snapshot = lexer.snap();
        auto node = parseAssignmentBinOpNode(lexer, errorMessage);

        if (node == nullptr)
        {
            lexer.restore(snapshot);
            node = parseFunctionCallOrVariable(lexer, errorMessage);
        }

        if (node == nullptr)
        {
            lexer.restore(snapshot);
            node = parseParenExpression(lexer, errorMessage);
        }

        return node;
    }

    /* static */
    std::unique_ptr<ExpressionNode> parseFunctionCallOrVariable(Lexer& lexer, std::string* errorMessage)
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
            return std::make_unique<VariableNode>(std::move(*optionalVariableNode));
        }

        // Should be unreachable...
        return nullptr;
    }

    /* static */
    std::unique_ptr<ParenExpressionNode> parseParenExpression(Lexer& lexer, std::string* errorMessage)
    {
        static Reporter& reporter = Reporter::instance();
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
                os << "Expected closing parenthesis";
                *errorMessage = os.str();

                reporter.error(*errorMessage, lexer.getLocation());
            }

            return nullptr;
        }

        return std::make_unique<ParenExpressionNode>(std::move(expression));
    }

    /* static */
    std::unique_ptr<ExpressionNode> parseMultiplyDivideBinOpNode(Lexer& lexer, std::string* errorMessage)
    {
        auto left = parseLitteralOrLRValueNode(lexer, errorMessage);

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
    std::unique_ptr<ExpressionNode> parseLitteralOrLRValueNode(Lexer& lexer, std::string* errorMessage)
    {
        const auto snapshot = lexer.snap();
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
        case TokenType::STRING:
        {
            const auto size = token.asCString().size();

            // Note: +1 for null-terminating character
            char* copyStr = new char[size + 1];
            std::strncpy(copyStr, token.asCString().c_str(), size);

            // Ensure last char is null-terminated
            copyStr[size] = '\0';

            return std::make_unique<LitteralNode>(copyStr);
        }
        case TokenType::NULL_T:
            return std::make_unique<LitteralNode>();
        case TokenType::SYMBOL:
            // We need to restore because 'parseFunctionCallOrVariable' with consume the token for us...
            lexer.restore(snapshot);
            return parseFunctionCallOrVariable(lexer, errorMessage);
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

#if OS_WIN
#pragma warning(pop)
#endif
