/**
 * The parser to our compiler.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#include <cmm/Parser.h>
#include <cmm/BinOpNode.h>
#include <cmm/CompilationUnitNode.h>
#include <cmm/FunctionDeclarationStatementNode.h>
#include <cmm/ExpressionNode.h>
#include <cmm/ExpressionStatementNode.h>
#include <cmm/LitteralNode.h>
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

    inline static Token newToken()
    {
        return Token('\0', false);
    }

    static bool expectSemicolon(Lexer& lexer, std::string* errorMessage);

    // Statements:
    static std::unique_ptr<Node> parseDeclarationStatement(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<Node> parseExpressionStatement(Lexer& lexer, std::string* errorMessage);
    static std::optional<std::vector<std::string>> parseFunctionParameters(Lexer& lexer, std::string* errorMessage);

    // Expression types:
    static std::unique_ptr<ExpressionNode> parseExpression(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ParenExpressionNode> parseParenExpression(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseMultiplyDivideBinOpNode(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseAddSubBinOpNode(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseAssignmentBinOpNode(Lexer& lexer, std::string* errorMessage);

    // Terminal nodes:
    static std::unique_ptr<ExpressionNode> parseLitteralOrVariableNode(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<VariableNode> parseVariableNode(Lexer& lexer, std::string* errorMessage);

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

        const auto snapshot = lexer.snap();
        auto node = parseDeclarationStatement(lexer, errorMessage);

        if (node == nullptr)
        {
            // TODO: Do we need to restore??
            lexer.restore(snapshot);

            node = parseExpressionStatement(lexer, errorMessage);
        }

        // Make sure no other tokens are left in the lexer's token stream.
        if (!lexer.completedOrWhitespaceOnly())
        {
            if (errorMessage != nullptr)
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
    std::unique_ptr<Node> parseExpressionStatement(Lexer& lexer, std::string* errorMessage)
    {
        auto expression = parseExpression(lexer, errorMessage);

        return expectSemicolon(lexer, errorMessage) ?
               std::make_unique<ExpressionStatementNode>(std::move(expression)) :
               nullptr;
    }

    /* static */
    std::optional<std::vector<std::string>> parseFunctionParameters(Lexer& lexer, std::string* errorMessage)
    {
        auto snapshot = lexer.snap();
        auto token = newToken();
        auto result = lexer.peekNextToken(token);

        if (result && token.isCharSymbol() && token.asCharSymbol() == CHAR_LPAREN)
        {
            // Capture the token
            lexer.nextToken(token, errorMessage);

            // Lookahead to the next token
            result = lexer.peekNextToken(token);

            // TODO: support arguments.  For now, just expect closing 'CHAR_RPAREN'.
            if (result && token.isCharSymbol() && token.asCharSymbol() == CHAR_RPAREN)
            {
                // Capture the token
                lexer.nextToken(token, errorMessage);
                return std::make_optional(std::vector<std::string>());
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
        auto variableName = parseVariableNode(lexer, errorMessage);

        if (variableName == nullptr)
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        // Lookahead to see if this is a function declaration or definition before
        // committing to this being a variable.
        auto optioanlFunctionArgs = parseFunctionParameters(lexer, errorMessage);
        if (optioanlFunctionArgs.has_value())
        {
            return expectSemicolon(lexer, errorMessage) ?
                std::make_unique<FunctionDeclarationStatementNode>(*type, variableName->getName()) :
                nullptr;
        }

        return expectSemicolon(lexer, errorMessage) ?
               std::make_unique<VariableDeclarationStatementNode>(*type, std::move(variableName)) :
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
            node = parseParenExpression(lexer, errorMessage);
        }

        return node;
    }

    /* static */
    std::unique_ptr<ParenExpressionNode> parseParenExpression(Lexer& lexer, std::string* errorMessage)
    {
        const auto snapshot = lexer.snap();

        auto token = newToken();
        bool lexResult = lexer.peekNextToken(token);

        // Expect opening '('
        if (!lexResult || !token.isCharSymbol() || token.asCharSymbol() != CHAR_LPAREN)
        {
            // TODO: is restore necessary??
            lexer.restore(snapshot);
            return nullptr;
        }

        // Accept the '(' token after lookahead.
        lexResult = lexer.nextToken(token, errorMessage);

        // wrapped expression i.e. (expr)
        auto expression = parseExpression(lexer, errorMessage);
        lexResult = lexer.peekNextToken(token);

        // Expect closing ')'
        if (!lexResult || !token.isCharSymbol() || token.asCharSymbol() != CHAR_RPAREN)
        {
            // TODO: is restore necessary??
            lexer.restore(snapshot);

            if (errorMessage != nullptr)
            {
                std::ostringstream os;
                os << "[PARSER]: Error: Expected closing parenthesis at "
                   << lexer.getLocation();
                *errorMessage = os.str();
            }

            return nullptr;
        }

        // Accept the ')' token after lookahead.
        lexResult = lexer.nextToken(token, errorMessage);

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
            // TODO: Do we want to just return 'nullptr' going forward??
#if 0
            std::ostringstream os;
            os << "unexpected token type received '"
               << toString(token.getType()) << '\'';
            unimplementedAbort(os.str());
            return nullptr;
#else
            return nullptr;
#endif
        }

        return nullptr;
    }

    /* static */
    std::unique_ptr<VariableNode> parseVariableNode(Lexer& lexer, std::string* errorMessage)
    {
        auto token = newToken();
        const bool lexResult = lexer.nextToken(token, errorMessage);

        if (!lexResult)
        {
            return nullptr;
        }

        return std::make_unique<VariableNode>(token.asStringSymbol());
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

