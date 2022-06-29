/**
 * The parser to our compiler.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#include <cmm/Parser.h>
#include <cmm/BinOpNode.h>
#include <cmm/CompilationUnitNode.h>
#include <cmm/DeclarationStatementNode.h>
#include <cmm/ExpressionNode.h>
#include <cmm/ExpressionStatementNode.h>
#include <cmm/LitteralNode.h>
#include <cmm/ParenExpressionNode.h>
#include <cmm/Snapshot.h>
#include <cmm/Token.h>
#include <cmm/TypeNode.h>
#include <cmm/VariableNode.h>

#include <iostream>
#include <sstream>

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

    static bool expectSemicolon(Lexer& lexer, std::string* errorMessage);

    // Statements:
    static std::shared_ptr<Node> parseDeclarationStatement(Lexer& lexer, std::string* errorMessage);
    static std::shared_ptr<Node> parseExpressionStatement(Lexer& lexer, std::string* errorMessage);

    // Expression types:
    static std::shared_ptr<ExpressionNode> parseExpression(Lexer& lexer, std::string* errorMessage);
    static std::shared_ptr<ParenExpressionNode> parseParenExpression(Lexer& lexer, std::string* errorMessage);
    // static std::shared_ptr<ExpressionNode> parseParenExpression(Lexer& lexer, std::string* errorMessage);
    static std::shared_ptr<ExpressionNode> parseMultiplyDivideBinOpNode(Lexer& lexer, std::string* errorMessage);
    static std::shared_ptr<ExpressionNode> parseAddSubBinOpNode(Lexer& lexer, std::string* errorMessage);
    static std::shared_ptr<ExpressionNode> parseAssignmentBinOpNode(Lexer& lexer, std::string* errorMessage);

    // Terminal nodes:
    static std::shared_ptr<ExpressionNode> parseLitteralOrVariableNode(Lexer& lexer, std::string* errorMessage);
    static std::shared_ptr<ExpressionNode> parseVariableNode(Lexer& lexer, std::string* errorMessage);

    static std::shared_ptr<Node> parseType(Lexer& lexer, std::string* errorMessage);

    Parser::Parser(const std::string& input) : lexer(input)
    {
    }

    Parser::Parser(std::string&& input) : lexer(std::move(input))
    {
    }

    std::shared_ptr<CompilationUnitNode> Parser::parseCompilationUnit(std::string* errorMessage)
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

        // For now since we don't have proper statements, expect a semi-colon here.
        return std::make_shared<CompilationUnitNode>(node);
    }

    /* static */
    bool expectSemicolon(Lexer& lexer, std::string* errorMessage)
    {
        Token token('\0', false);
        const bool lexResult = lexer.nextToken(token, errorMessage);

        return lexResult && token.getType() == TokenType::CHAR_SYMBOL &&
               token.asCharSymbol() == CHAR_SEMI_COLON;
    }

    /* static */
    std::shared_ptr<Node> parseExpressionStatement(Lexer& lexer, std::string* errorMessage)
    {
        auto expression = parseExpression(lexer, errorMessage);

        return expectSemicolon(lexer, errorMessage) ?
               std::make_shared<ExpressionStatementNode>(std::move(expression)) :
               nullptr;
    }

    /* static */
    std::shared_ptr<Node> parseDeclarationStatement(Lexer& lexer, std::string* errorMessage)
    {
        auto snapshot = lexer.snap();
        auto type = parseType(lexer, errorMessage);

        if (type == nullptr)
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

        auto declType = std::dynamic_pointer_cast<TypeNode>(type);
        auto declVar = std::dynamic_pointer_cast<VariableNode>(type);
        return expectSemicolon(lexer, errorMessage) ?
               std::make_shared<DeclarationStatementNode>(std::move(declType), std::move(declVar)) :
               nullptr;
    }

    /* static */
    std::shared_ptr<ExpressionNode> parseExpression(Lexer& lexer, std::string* errorMessage)
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
    std::shared_ptr<ParenExpressionNode> parseParenExpression(Lexer& lexer, std::string* errorMessage)
    // std::shared_ptr<ExpressionNode> parseParenExpression(Lexer& lexer, std::string* errorMessage)
    {
        const auto snapshot = lexer.snap();

        Token token('\0', false);
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
            return nullptr;
        }

        // Accept the ')' token after lookahead.
        lexResult = lexer.nextToken(token, errorMessage);

        return std::make_shared<ParenExpressionNode>(std::move(expression));
    }

    /* static */
    std::shared_ptr<ExpressionNode> parseMultiplyDivideBinOpNode(Lexer& lexer, std::string* errorMessage)
    {
        auto left = parseLitteralOrVariableNode(lexer, errorMessage);

        if (left == nullptr)
        {
            return nullptr;
        }

        Token token('\0', false);
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
            left = std::make_shared<BinOpNode>(actualBinOp, std::move(left), std::move(right));

            // Lookahead for next iteration.
            lexResult = lexer.peekNextToken(token);
        }

        return left;
    }

    /* static */
    std::shared_ptr<ExpressionNode> parseAddSubBinOpNode(Lexer& lexer, std::string* errorMessage)
    {
        auto left = parseMultiplyDivideBinOpNode(lexer, errorMessage);

        if (left == nullptr)
        {
            return nullptr;
        }

        Token token('\0', false);
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
            left = std::make_shared<BinOpNode>(actualBinOp, std::move(left), std::move(right));

            // Lookahead for next iteration.
            lexResult = lexer.peekNextToken(token);
        }

        return left;
    }

    /* static */
    std::shared_ptr<ExpressionNode> parseAssignmentBinOpNode(Lexer& lexer, std::string* errorMessage)
    {
        auto left = parseAddSubBinOpNode(lexer, errorMessage);

        if (left == nullptr)
        {
            return nullptr;
        }

        Token token('\0', false);
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
            left = std::make_shared<BinOpNode>(EnumBinOpNodeType::ASSIGNMENT, std::move(left), std::move(right));

            // Lookahead for next iteration.
            lexResult = lexer.peekNextToken(token);
        }

        return left;
    }

    /* static */
    std::shared_ptr<ExpressionNode> parseLitteralOrVariableNode(Lexer& lexer, std::string* errorMessage)
    {
        Token token('\0', false);
        const bool lexResult = lexer.nextToken(token, errorMessage);

        if (!lexResult)
        {
            return nullptr;
        }

        switch (token.getType())
        {
        case TokenType::BOOL:
            return std::make_shared<LitteralNode>(token.asBool());
        case TokenType::CHAR:
            return std::make_shared<LitteralNode>(token.asChar());
        case TokenType::FLOAT:
            return std::make_shared<LitteralNode>(token.asFloat());
        case TokenType::DOUBLE:
            return std::make_shared<LitteralNode>(token.asDouble());
        case TokenType::INT16:
            return std::make_shared<LitteralNode>(token.asInt16());
        case TokenType::INT32:
            return std::make_shared<LitteralNode>(token.asInt32());
        case TokenType::INT64:
            return std::make_shared<LitteralNode>(token.asInt64());
        case TokenType::SYMBOL:
            return std::make_shared<VariableNode>(token.asStringSymbol());
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
    std::shared_ptr<ExpressionNode> parseVariableNode(Lexer& lexer, std::string* errorMessage)
    {
        Token token('\0', false);
        const bool lexResult = lexer.nextToken(token, errorMessage);

        if (!lexResult)
        {
            return nullptr;
        }

        return std::make_shared<VariableNode>(token.asStringSymbol());
    }

    /* static */
    std::shared_ptr<Node> parseType(Lexer& lexer, std::string* errorMessage)
    {
        Token token('\0', false);
        const bool lexResult = lexer.nextToken(token, errorMessage);

        if (!lexResult || !token.isStringSymbol())
        {
            return nullptr;
        }

        auto type = token.asStringSymbol();

        if (isCType(type))
        {
            const auto enumType = getCType(token.asStringSymbol());

            if (enumType.has_value())
            {
                return std::make_shared<TypeNode>(enumType.value());
            }
        }

        return nullptr;
    }
}

