/**
 * The parser to our compiler.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

// Our includes
#include <cmm/Parser.h>
#include <cmm/Keyword.h>
#include <cmm/NodeList.h>
#include <cmm/ParserPredictor.h>
#include <cmm/Reporter.h>
#include <cmm/Snapshot.h>
#include <cmm/Token.h>

// std includes
#include <cstdlib>
#include <cstring>
#include <iostream>
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

    static bool expectChar(Lexer& lexer, std::string* errorMessage, const char ch);
    static bool expectSemicolon(Lexer& lexer, std::string* errorMessage);

    [[maybe_unused]]
    static bool testExpectChar(Lexer& lexer, std::string* errorMessage, const char ch);

    static TranslationUnitNode parseTranslationUnit(Lexer& lexer, std::string* errorMessage);

    // Statements:
    static std::unique_ptr<StatementNode> parseDeclarationStatement(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<StatementNode> parseExpressionStatement(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<StatementNode> parseIfElseStatement(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<StatementNode> parseReturnStatement(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ReturnStatementNode> parseReturnStatementStrict(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<StatementNode> parseWhileStatement(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<StatementNode> parseStatement(Lexer& lexer, std::string* errorMessage);
    static std::optional<std::vector<std::unique_ptr<StatementNode>>> parseOneOrMoreStatements(Lexer& lexer, std::string* errorMessage);

    // Other utility parsing functions
    static std::optional<BlockNode> parseBlockStatement(Lexer& lexer, std::string* errorMessage);
    static std::optional<BlockNode> parseBlockStatement(Lexer& lexer, std::string* errorMessage, const std::optional<std::unordered_set<EnumNodeType>>& validNodeTypes);
    static std::optional<BlockNode> parseStructBlockStatement(Lexer& lexer, std::string* errorMessage);
    static std::optional<std::vector<ArgNode>> parseFunctionCallArgs(Lexer& lexer, std::string* errorMessage);
    static std::optional<std::vector<ParameterNode>> parseFunctionParameters(Lexer& lexer, std::string* errorMessage);
    static std::optional<u32> parsePointerInderectionCount(Lexer& lexer, std::string* errorMessage, Location* location); // TODO: Make location not optional?
    static std::unique_ptr<ExpressionNode> buildDerefNode(const u32 count, std::unique_ptr<ExpressionNode>&& expr);

    // Expression types:
    static std::unique_ptr<ExpressionNode> parseExpression(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseFunctionCallOrVariable(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseCastExpression(Lexer& lexer, std::string* errorMessage);
    // TODO: Consider refactoring/reworking this??
    static std::unique_ptr<ExpressionNode> parseParenExpression(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseParenExpression2(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseMultiplyDivideBinOpNode(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseAddSubBinOpNode(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseAssignmentBinOpNode(Lexer& lexer, std::string* errorMessage);

    // Terminal nodes:
    static std::unique_ptr<ExpressionNode> parseLitteralOrLRValueNode(Lexer& lexer, std::string* errorMessage);
    static std::unique_ptr<ExpressionNode> parseUnaryExpression(Lexer& lexer, std::string* errorMessage);
    static std::optional<VariableNode> parseVariableNode(Lexer& lexer, std::string* errorMessage);

    static std::optional<TypeNode> parseTypeNode(Lexer& lexer, std::string* errorMessage);

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
            return nullptr;
        }

        auto translationUnit = parseTranslationUnit(lexer, errorMessage);

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

        return std::make_unique<CompilationUnitNode>(std::move(translationUnit));
    }

    /* static */
    bool expectChar(Lexer& lexer, std::string* errorMessage, const char ch)
    {
        auto token = newToken();
        const bool lexResult = lexer.nextToken(token, errorMessage);

        return lexResult && token.getType() == TokenType::CHAR_SYMBOL &&
               token.asCharSymbol() == ch;
    }

    /* static */
    bool expectSemicolon(Lexer& lexer, std::string* errorMessage)
    {
        return expectChar(lexer, errorMessage, CHAR_SEMI_COLON);
    }

    /* static */
    bool testExpectChar(Lexer& lexer, std::string* errorMessage, const char ch)
    {
        const auto snapshot = lexer.snap();
        const bool result = expectChar(lexer, errorMessage, ch);

        if (!result)
        {
            lexer.restore(snapshot);
        }

        return result;
    }

    /* static */
    std::optional<BlockNode> parseBlockStatement(Lexer& lexer, std::string* errorMessage)
    {
        return parseBlockStatement(lexer, errorMessage, std::nullopt);
    }

    /* static */
    std::optional<BlockNode> parseBlockStatement(Lexer& lexer, std::string* errorMessage, const std::optional<std::unordered_set<EnumNodeType>>& validNodeTypes)
    {
        auto snapshot = lexer.snap();
        auto token = newToken();
        bool result = lexer.peekNextToken(token);

        if (!result || !token.isCharSymbol() || token.asCharSymbol() != CHAR_LCURLY_BRACKET)
        {
            lexer.restore(snapshot);
            return std::nullopt;
        }

        // Save the location as the start of the BlockNode.
        Location beginLoc;

        // Consume token
        lexer.nextToken(token, errorMessage, &beginLoc);

        BlockNode::StatementList statements;
        statements.reserve(0x10);

        //{
        while (true)
        {
            auto currentStatement = parseStatement(lexer, errorMessage);

            if (currentStatement != nullptr)
            {
                // Check if there are restrictions and if there are, make sure the types match.
                if (validNodeTypes == std::nullopt || validNodeTypes->find(currentStatement->getType()) != validNodeTypes->end())
                {
                    statements.push_back(std::unique_ptr<StatementNode>(static_cast<StatementNode*>(currentStatement.release())));
                }

                // Invalid, bail out as invalid
                else
                {
                    lexer.restore(snapshot);
                    return std::nullopt;
                }
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

        Location endLoc;

        // Consume token
        lexer.nextToken(token, errorMessage, &endLoc);

        return std::make_optional<BlockNode>(beginLoc, endLoc, std::move(statements));
    }

    /* static */
    std::optional<BlockNode> parseStructBlockStatement(Lexer& lexer, std::string* errorMessage)
    {
        static std::unordered_set<EnumNodeType> validNodeTypes = { EnumNodeType::VARIABLE_DECLARATION_STATEMENT, EnumNodeType::STRUCT_DEFINITION };
        static auto opt = std::make_optional(validNodeTypes);
        return parseBlockStatement(lexer, errorMessage, opt);
    }

    /* static */
    std::unique_ptr<StatementNode> parseExpressionStatement(Lexer& lexer, std::string* errorMessage)
    {
        auto expression = parseExpression(lexer, errorMessage);

        return expectSemicolon(lexer, errorMessage) ?
               std::make_unique<ExpressionStatementNode>(expression->getLocation(), std::move(expression)) :
               nullptr;
    }

    /* static */
    std::unique_ptr<StatementNode> parseIfElseStatement(Lexer& lexer, std::string* errorMessage)
    {
        const auto snapshot = lexer.snap();
        auto token = newToken();

        // Save the location as the start of the BlockNode.
        Location location;
        bool result = lexer.nextToken(token, errorMessage, &location);

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

        auto* rawIfStatement = static_cast<StatementNode*>(statement.release());
        std::unique_ptr<StatementNode> ifStatementPtr(rawIfStatement);

        // Lookahead to see if we have an 'else' part.
        const auto elseSnapshot = lexer.snap();
        result = lexer.nextToken(token, errorMessage);

        // Expect else statement next
        if (result && token.isStringSymbol() && token.asStringSymbol() == "else")
        {
            statement = parseStatement(lexer, errorMessage);

            auto* rawElseStatement = static_cast<StatementNode*>(statement.release());
            std::unique_ptr<StatementNode> elseStatementPtr(rawElseStatement);
            return std::make_unique<IfElseStatementNode>(location, std::move(expression), std::move(ifStatementPtr), std::move(elseStatementPtr));
        }

        // No else block, restore and continue with just the if block.
        else
        {
            lexer.restore(elseSnapshot);
        }

        return std::make_unique<IfElseStatementNode>(location, std::move(expression), std::move(ifStatementPtr));
    }

    /* static */
    std::unique_ptr<StatementNode> parseReturnStatement(Lexer& lexer, std::string* errorMessage)
    {
        return parseReturnStatementStrict(lexer, errorMessage);
    }

    /* static */
    std::unique_ptr<ReturnStatementNode> parseReturnStatementStrict(Lexer& lexer, std::string* errorMessage)
    {
        const auto snapshot = lexer.snap();
        auto token = newToken();

        // Save the location as the start of the BlockNode.
        Location location;
        bool result = lexer.nextToken(token, errorMessage, &location);

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
                std::make_unique<ReturnStatementNode>(location) :
                nullptr;
        }

        // Else, must contain an expression
        auto expression = parseExpression(lexer, errorMessage);

        return expectSemicolon(lexer, errorMessage) ?
               std::make_unique<ReturnStatementNode>(location, std::move(expression)) :
               nullptr;
    }

    /* static */
    std::unique_ptr<StatementNode> parseWhileStatement(Lexer& lexer, std::string* errorMessage)
    {
        static Reporter& reporter = Reporter::instance();

        const auto snapshot = lexer.snap();
        auto token = newToken();

        // Save the location as the start of the BlockNode.
        Location location;
        bool result = lexer.nextToken(token, errorMessage, &location);

        // Look for 'while'
        if (!result || !token.isStringSymbol() || token.asStringSymbol() != "while")
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        // Look for '('
        result = lexer.nextToken(token, errorMessage);

        if (!result || !token.isCharSymbol() || token.asCharSymbol() != CHAR_LPAREN)
        {
            if (canWriteErrorMessage(errorMessage))
            {
                *errorMessage = "expected '(' following the start of a while statement's conditional expression.";
                reporter.error(*errorMessage, lexer.getLocation());
            }

            lexer.restore(snapshot);
            return nullptr;
        }

        // Expect conditional expression
        auto conditional = parseExpression(lexer, errorMessage);

        if (conditional == nullptr)
        {
            if (canWriteErrorMessage(errorMessage))
            {
                *errorMessage = "expected an expression as part of the while loop's conditional expression.";
                reporter.error(*errorMessage, lexer.getLocation());
            }

            lexer.restore(snapshot);
            return nullptr;
        }

        // Expect closing paren
        result = lexer.nextToken(token, errorMessage);

        if (!result || !token.isCharSymbol() || token.asCharSymbol() != CHAR_RPAREN)
        {
            if (canWriteErrorMessage(errorMessage))
            {
                *errorMessage = "expected ')' following the end of a while statement's conditional expression.";
                reporter.error(*errorMessage, lexer.getLocation());
            }

            lexer.restore(snapshot);
            return nullptr;
        }

        // Expect some statement to loop on
        auto statementPtr = parseStatement(lexer, errorMessage);

        if (statementPtr == nullptr)
        {
            if (canWriteErrorMessage(errorMessage))
            {
                *errorMessage = "expected a statement following while loop's conditional expression.";
                reporter.error(*errorMessage, lexer.getLocation());
            }

            lexer.restore(snapshot);
            return nullptr;
        }

        return std::make_unique<WhileStatementNode>(location, std::move(conditional), std::move(statementPtr));
    }

    /* static */
    std::unique_ptr<StatementNode> parseStatement(Lexer& lexer, std::string* errorMessage)
    {
        static ParserPredictor<std::unique_ptr<StatementNode>(Lexer&, std::string*)> predictor;

        if (predictor.empty())
        {
            auto token = newToken();
            token.setStringSymbol(Keyword::RETURN.getName());
            predictor.registerFunction(token, parseReturnStatement);

            token.setStringSymbol(Keyword::IF.getName());
            predictor.registerFunction(token, parseIfElseStatement);

            token.setStringSymbol(Keyword::WHILE.getName());
            predictor.registerFunction(token, parseWhileStatement);

            token.setCharSymbol(CHAR_LCURLY_BRACKET);

            static const auto parseBlockStatementWrapper = [](Lexer& lexer, std::string* errorMessage) -> std::unique_ptr<StatementNode>
            {
                auto optionalBlockNode = parseBlockStatement(lexer, errorMessage);
                return optionalBlockNode.has_value() ? std::make_unique<BlockNode>(std::move(*optionalBlockNode)) : nullptr;
            };

            predictor.registerFunction(token, parseBlockStatementWrapper);

            Keyword::registerPrimitiveKeywordsByName([&](const std::string& keywordName)
                    {
                        token.setStringSymbol(keywordName);
                        predictor.registerFunction(token, parseDeclarationStatement);
                    });
        }

        auto tokenLookahead = newToken();
        const bool result = lexer.peekNextToken(tokenLookahead);

        if (result)
        {
            auto predictionContext = predictor.predict(tokenLookahead);

            if (predictionContext.has_value())
            {
                return predictor.call<std::unique_ptr<StatementNode>>(*predictionContext, lexer, errorMessage);
            }
        }

        // TODO: See if we can get this in the predictor as well.  Leaving for now...
        const auto snapshot = lexer.snap();
        std::unique_ptr<StatementNode> node = parseExpressionStatement(lexer, errorMessage);

        if (node == nullptr)
        {
            lexer.restore(snapshot);
        }

        return node;
    }

    /* static */
    std::optional<std::vector<std::unique_ptr<StatementNode>>> parseOneOrMoreStatements(Lexer& lexer, std::string* errorMessage)
    {
        auto statement = parseStatement(lexer, errorMessage);

        if (statement == nullptr)
        {
            return std::nullopt;
        }

        std::vector<std::unique_ptr<StatementNode>> results;

        while (statement != nullptr)
        {
            results.emplace_back(std::move(statement));
            statement = parseStatement(lexer, errorMessage);
        }

        return std::make_optional(std::move(results));
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
            // Save the starting location.
            Location location;

            // Capture the token
            lexer.nextToken(token, errorMessage, &location);

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

                    else if (!token.isCharSymbol())
                    {
                        // This could be 'func()' or 'func(x)', so we check if the variable was parsed or not.
                        auto litteralPtr = parseLitteralOrLRValueNode(lexer, errorMessage);

                        if (litteralPtr != nullptr)
                        {
                            args.emplace_back(litteralPtr->getLocation(), std::move(litteralPtr));
                        }
                    }

                    // Peek ahead
                    result = lexer.peekNextToken(token);

                    // Check to see if the next char is a comma and if so consume that now.
                    if (result && token.isCharSymbol() && token.asCharSymbol() == CHAR_COMMA)
                    {
                        // Consume the comma
                        lexer.nextToken(token, errorMessage);

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
    std::optional<u32> parsePointerInderectionCount(Lexer& lexer, std::string* errorMessage, Location* location)
    {
        auto snapshot = lexer.snap();
        auto token = newToken();
        bool result = lexer.peekNextToken(token);

        if (!result || !token.isCharSymbol() || token.asCharSymbol() != CHAR_ASTERISK)
        {
            lexer.restore(snapshot);
            return std::nullopt;
        }

        Location* tempLocation = nullptr;
        u32 count = 0;

        while (result && token.isCharSymbol() && token.asCharSymbol() == CHAR_ASTERISK)
        {
            if (tempLocation == nullptr)
            {
                tempLocation = location;
                lexer.nextToken(token, errorMessage, tempLocation);
            }

            else
            {
                lexer.nextToken(token, errorMessage);
            }

            ++count;
            result = lexer.peekNextToken(token);
        }

        return std::make_optional(count);
    }

    /* static */
    std::unique_ptr<ExpressionNode> buildDerefNode(const u32 count, std::unique_ptr<ExpressionNode>&& expr)
    {
        auto result = std::make_unique<DerefNode>(expr->getLocation(), std::move(expr));

        for (u32 i = 1; i < count; ++i)
        {
            result = std::make_unique<DerefNode>(result->getLocation(), std::move(result));
        }

        return result;
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
            // Save the starting location.
            Location location;

            // Capture the token
            lexer.nextToken(token, errorMessage, &location);

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
                        auto typeOpt = parseTypeNode(lexer, errorMessage);

                        result = lexer.peekNextToken(token);

                        // We expect a variable or comma.  Bail out if it's anything else.
                        if (!result || (!token.isStringSymbol() && !token.isCharSymbol()) || (token.isCharSymbol() && token.asCharSymbol() != CHAR_COMMA && token.asCharSymbol() != CHAR_RPAREN))
                        {
                            if (canWriteErrorMessage(errorMessage))
                            {
                                std::ostringstream builder;
                                builder << "Expected a ',' or ')', but found ";

                                if (token.isCharSymbol())
                                {
                                    builder << "char '" << token.asCharSymbol() << "'";
                                }

                                // Must be a string symbol
                                else
                                {
                                    builder << "symbol '" << token.asStringSymbol() << "'";
                                }

                                *errorMessage = builder.str();
                            }

                            return std::nullopt;
                        }

                        auto variableNodeOpt = parseVariableNode(lexer, errorMessage);

                        // This is the 'func(int x)' case.
                        if (variableNodeOpt.has_value())
                        {
                            // TODO: Should probably verify it is safe to do this
                            // without checking the optional for 'has_value'.
                            params.emplace_back(typeOpt->getLocation(), std::move(*typeOpt), std::move(*variableNodeOpt));
                        }

                        // This is the 'func(int)' case.
                        else
                        {
                            // TODO: Should probably verify it is safe to do this
                            // without checking the optional for 'has_value'.
                            params.emplace_back(typeOpt->getLocation(), std::move(*typeOpt));
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
                        lexer.nextToken(token, errorMessage);

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
    TranslationUnitNode parseTranslationUnit(Lexer& lexer, std::string* errorMessage)
    {
        auto statements = parseOneOrMoreStatements(lexer, errorMessage);

        if (statements.has_value())
        {
            Location location;

            if (!statements->empty())
            {
                const auto& statementVec = *statements;
                location = statementVec[0]->getLocation();
            }

            TranslationUnitNode translationUnit(location, std::move(*statements));
            return translationUnit;
        }

        return TranslationUnitNode();
    }

    /* static */
    std::unique_ptr<StatementNode> parseDeclarationStatement(Lexer& lexer, std::string* errorMessage)
    {
        static auto& reporter = Reporter::instance();
        auto snapshot = lexer.snap();
        auto type = parseTypeNode(lexer, errorMessage);

        if (!type.has_value())
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        // If it is a struct, we need to get the name of the struct.
        const bool wasStructType = type->getDatatype().type == EnumCType::STRUCT;
        auto optVariableName = parseVariableNode(lexer, errorMessage);

        if (!optVariableName.has_value())
        {
            // No variable name.  If it was a struct, then it was a forward declaration
            // or struct definition.
            if (wasStructType)
            {
                // Struct forward declarations or defintions must not contain any '*'.
                if (type->getDatatype().pointers == 0)
                {
                    const auto startLoc = type->getLocation();
                    snapshot = lexer.snap();
                    auto token = newToken();
                    const bool lexerResult = lexer.peekNextToken(token);

                    if (!lexerResult || !token.isCharSymbol())
                    {
                        lexer.restore(snapshot);
                        return nullptr;
                    }

                    // Struct forward declaration
                    if (token.asCharSymbol() == CHAR_SEMI_COLON)
                    {
                        // Consume the token
                        lexer.nextToken(token, errorMessage);
                        return std::make_unique<StructFwdDeclarationStatementNode>(startLoc, std::move(*type));
                    }

                    // Struct definition
                    else if (token.asCharSymbol() == CHAR_LCURLY_BRACKET)
                    {
                        auto blockNode = parseStructBlockStatement(lexer, errorMessage);
                        return expectSemicolon(lexer, errorMessage) ?
                            std::make_unique<StructDefinitionStatementNode>(startLoc, *type->getDatatype().optStructName, std::move(*blockNode)) :
                            nullptr;
                    }
                }

                else
                {
                    std::ostringstream builder;
                    builder << "Expected qualified identifier ';' to complete a forward declaration or '{' to indicate the start of a struct definition for struct "
                            << type->getDatatype().optStructName.value();
                    reporter.error(builder.str(), type->getLocation());

                    if (canWriteErrorMessage(errorMessage))
                    {
                        *errorMessage = builder.str();
                    }
                }

                return nullptr;
            }

            // Bad parse, bail out
            else
            {
                lexer.restore(snapshot);
                return nullptr;
            }
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
                return std::make_unique<FunctionDefinitionStatementNode>(type->getLocation(), *type,
                        optVariableName->getName(), std::move(*optionalBlockStatement), std::move(*optionalFunctionArgs));
            }

            // Function declaration
            else
            {
                return expectSemicolon(lexer, errorMessage) ?
                    std::make_unique<FunctionDeclarationStatementNode>(type->getLocation(), *type, optVariableName->getName(), std::move(*optionalFunctionArgs)) :
                    nullptr;
            }
        }

        return expectSemicolon(lexer, errorMessage) ?
               std::make_unique<VariableDeclarationStatementNode>(type->getLocation(), *type, std::move(*optVariableName)) :
               nullptr;
    }

    /* static */
    std::unique_ptr<ExpressionNode> parseExpression(Lexer& lexer, std::string* errorMessage)
    {
        static ParserPredictor<std::unique_ptr<ExpressionNode>(Lexer&, std::string*)> predictor;

        if (predictor.empty())
        {
            auto token = newToken();
            token.setCharSymbol(CHAR_LPAREN);

            // TODO: This is very naive, but it works.  Maybe we can come up with something better later.
            static const auto parenPredictor = [](Lexer& lexer, std::string* errorMessage) -> std::unique_ptr<ExpressionNode>
            {
                auto expr = parseCastExpression(lexer, errorMessage);
#if 0
                // return expr != nullptr ? std::move(expr) : parseParenExpression(lexer, errorMessage);
                return expr != nullptr ? std::move(expr) : parseParenExpression2(lexer, errorMessage);
                // return expr != nullptr ? std::move(expr) : nullptr;
#else
                if (expr != nullptr)
                {
                    return expr;
                }

                else if (errorMessage != nullptr)
                {
                    errorMessage->clear();
                }

                return nullptr;
#endif
            };

            // @@@
            predictor.registerFunction(token, parenPredictor);
        }

        auto tokenLookahead = newToken();
        bool result = lexer.peekNextToken(tokenLookahead);

        if (result)
        {
            auto predictionContext = predictor.predict(tokenLookahead);

            if (predictionContext.has_value())
            {
                // @@@
                // return predictor.call<std::unique_ptr<ExpressionNode>>(*predictionContext, lexer, errorMessage);
                auto result = predictor.call<std::unique_ptr<ExpressionNode>>(*predictionContext, lexer, errorMessage);
                if (result != nullptr)
                {
                    return result;
                }
            }
        }

        const auto snapshot = lexer.snap();
        auto node = parseAssignmentBinOpNode(lexer, errorMessage);

        if (node == nullptr)
        {
            lexer.restore(snapshot);
            return nullptr;
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

        static auto& reporter = Reporter::instance();

        auto snapshot = lexer.snap();

        // TODO: Try to use ParserPredictor somehow here.
        auto unaryOrCastOrDerefOrVarExprPtr = parseUnaryExpression(lexer, errorMessage);

        // Did not get a name of the function, early exit.
        if (!unaryOrCastOrDerefOrVarExprPtr)
        {
            lexer.restore(snapshot);
            unaryOrCastOrDerefOrVarExprPtr = parseCastExpression(lexer, errorMessage);

            // Try parse a cast expression
            if (!unaryOrCastOrDerefOrVarExprPtr)
            {
                lexer.restore(snapshot);
                Location ptrLocation;
                auto optionalPtrInderection = parsePointerInderectionCount(lexer, errorMessage, &ptrLocation);
                auto optionalVariable = parseVariableNode(lexer, errorMessage);

                if (!optionalVariable.has_value())
                {
                    lexer.restore(snapshot);
                    return nullptr;
                }

                // DerefNode
                else if (optionalPtrInderection.has_value())
                {
                    auto varPtr = std::make_unique<VariableNode>(std::move(*optionalVariable));
                    unaryOrCastOrDerefOrVarExprPtr = buildDerefNode(*optionalPtrInderection, std::move(varPtr));
                }

                // Normal variable
                else
                {
                    unaryOrCastOrDerefOrVarExprPtr = std::make_unique<VariableNode>(std::move(*optionalVariable));
                }
            }
        }

        auto optionalArgList = parseFunctionCallArgs(lexer, errorMessage);

        // Has args
        if (optionalArgList.has_value())
        {
            auto nodeType = unaryOrCastOrDerefOrVarExprPtr->getType();

            if (nodeType == EnumNodeType::ADDRESS_OF)
            {
                const char* theErrorMessage = "cannot take the address of a function call";

                if (canWriteErrorMessage(errorMessage))
                {
                    *errorMessage = theErrorMessage;
                }

                reporter.error(theErrorMessage, unaryOrCastOrDerefOrVarExprPtr->getLocation());
                lexer.restore(snapshot);
                return nullptr;
            }

            else if (nodeType == EnumNodeType::UNARY_OP)
            {
                auto* unaryOpPtr = static_cast<UnaryOpNode*>(unaryOrCastOrDerefOrVarExprPtr.get());
                const auto unaryOpType = unaryOpPtr->getOpType();

                if (unaryOpType != EnumUnaryOpType::NEGATIVE && unaryOpType != EnumUnaryOpType::POSITIVE)
                {
                    const char* theErrorMessage = "invalid unary operator for a function call";

                    if (canWriteErrorMessage(errorMessage))
                    {
                        *errorMessage = theErrorMessage;
                    }

                    reporter.error(theErrorMessage, unaryOpPtr->getLocation());
                    lexer.restore(snapshot);
                    return nullptr;
                }

                else
                {
                    const auto* variablePtr = static_cast<VariableNode*>(unaryOpPtr->getExpression());
                    auto funcName = variablePtr->getName();

                    std::unique_ptr<ExpressionNode> funcCallPtr = std::make_unique<FunctionCallNode>(unaryOrCastOrDerefOrVarExprPtr->getLocation(), std::move(funcName), std::move(*optionalArgList));
                    unaryOpPtr->setExpression(std::move(funcCallPtr));
                    return unaryOrCastOrDerefOrVarExprPtr;
                }
            }

            else if (nodeType == EnumNodeType::CAST)
            {
                return unaryOrCastOrDerefOrVarExprPtr;
            }

            else if (nodeType == EnumNodeType::DEREF)
            {
                const auto* derefPtr = static_cast<DerefNode*>(unaryOrCastOrDerefOrVarExprPtr.get());
                nodeType = derefPtr->getRootType();
            }

            if (nodeType == EnumNodeType::VARIABLE)
            {
                auto* variablePtr = static_cast<VariableNode*>(unaryOrCastOrDerefOrVarExprPtr.get());
                auto funcName = variablePtr->getName();

                return std::make_unique<FunctionCallNode>(unaryOrCastOrDerefOrVarExprPtr->getLocation(), std::move(funcName), std::move(*optionalArgList));
            }

            // else
            std::ostringstream builder;
            builder << "Un-expected EnumNodeType of " << *reinterpret_cast<int*>(&nodeType);
            unimplementedAbort(builder.str());

            // Not reachable
            return nullptr;
        }

        // No args present, normal variable regardless if there is a DerefNode on it or not.
        else
        {
            return unaryOrCastOrDerefOrVarExprPtr;
        }

        // Should be unreachable...
        return nullptr;
    }

    /* static */
    std::unique_ptr<ExpressionNode> parseCastExpression(Lexer& lexer, std::string* errorMessage)
    {
        auto snapshot = lexer.snap();
        auto token = newToken();
        Location startLocation;
        bool lexResult = lexer.nextToken(token, errorMessage, &startLocation);

        if (!lexResult || !token.isCharSymbol() || token.asCharSymbol() != CHAR_LPAREN)
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        // Expect some type:
        auto optionalDimensionCount = parsePointerInderectionCount(lexer, errorMessage, nullptr);
        auto optionalTypeNode = parseTypeNode(lexer, errorMessage);

        // Make sure our cast has a type!
        if (!optionalTypeNode.has_value())
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        if (optionalDimensionCount.has_value())
        {
            auto& theType = optionalTypeNode->getDatatype();
            theType.pointers = *optionalDimensionCount;
        }

        // Expect closing paren:
        lexResult = lexer.nextToken(token, errorMessage, nullptr);

        if (!lexResult || !token.isCharSymbol() || token.asCharSymbol() != CHAR_RPAREN)
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        // Expect expression being casted:
        const auto tempLocation = lexer.snap().getLocation();
        auto exprToCast = parseExpression(lexer, errorMessage);

        if (exprToCast == nullptr)
        {
            if (canWriteErrorMessage(errorMessage))
            {
                std::ostringstream builder;
                builder << "Expected an expression to cast at ("
                        << tempLocation.getLine() << ", " << tempLocation.getPosition()
                        << ").";
                *errorMessage = builder.str();
            }

            lexer.restore(snapshot);
            return nullptr;
        }

        auto castExpr = std::make_unique<CastNode>(startLocation, optionalTypeNode->getDatatype(), std::move(exprToCast));
        return castExpr;
    }

    /* static */
    std::unique_ptr<ExpressionNode> parseParenExpression(Lexer& lexer, std::string* errorMessage)
    {
        static Reporter& reporter = Reporter::instance();
        const auto snapshot = lexer.snap();

        auto token = newToken();
        Location location;
        bool lexResult = lexer.nextToken(token, errorMessage, &location);

        // Expect opening '('
        if (!lexResult || !token.isCharSymbol() || token.asCharSymbol() != CHAR_LPAREN)
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        // wrapped expression i.e. (expr)
        auto expression = parseExpression(lexer, errorMessage);
        lexResult = lexer.nextToken(token, nullptr);

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

        return std::make_unique<ParenExpressionNode>(expression->getLocation(), std::move(expression));
    }

    /* static */
    std::unique_ptr<ExpressionNode> parseParenExpression2(Lexer& lexer, std::string* errorMessage)
    {
        static Reporter& reporter = Reporter::instance();
        const auto snapshot = lexer.snap();

        auto token = newToken();
        bool lexResult = lexer.peekNextToken(token);

        // Expect opening '('. If not, 'reduce' by parsing variable, litteral, etc.
        if (!lexResult || !token.isCharSymbol() || token.asCharSymbol() != CHAR_LPAREN)
        {
            auto left = parseLitteralOrLRValueNode(lexer, errorMessage);
            return left;
        }

        // Capture the token if is an CHAR_LPAREN
        Location startLocation;
        lexer.nextToken(token, errorMessage, &startLocation);

        // Else, expect a "general" expression.
        auto subexpression = parseExpression(lexer, errorMessage);

        Location endLocation;
        lexer.nextToken(token, errorMessage, &endLocation);

        // Lastly, expect a closing paren. Error if not.
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

        return std::make_unique<ParenExpressionNode>(subexpression->getLocation(), std::move(subexpression));
    }

    /* static */
    std::unique_ptr<ExpressionNode> parseMultiplyDivideBinOpNode(Lexer& lexer, std::string* errorMessage)
    {
        // auto left = parseLitteralOrLRValueNode(lexer, errorMessage);
        auto left = parseParenExpression2(lexer, errorMessage);

        if (left == nullptr)
        {
            return nullptr;
        }

        auto token = newToken();
        bool lexResult = lexer.peekNextToken(token);

        // For saving the starting token.
        Location location;
        Location* locationPtr = nullptr;

        while (lexResult)
        {
            if (token.isCharSymbol() && (token.asCharSymbol() == CHAR_ASTERISK || token.asCharSymbol() == CHAR_FORWARD_SLASH))
            {
                // Valid bin type accept the token

                if (locationPtr == nullptr)
                {
                    locationPtr = &location;
                    lexer.nextToken(token, errorMessage, locationPtr);
                }

                else
                {
                    lexer.nextToken(token, errorMessage);
                }
            }

            else
            {
                return left;
            }

            const auto actualBinOp = token.asCharSymbol() == CHAR_ASTERISK ? EnumBinOpNodeType::MULTIPLY : EnumBinOpNodeType::DIVIDE;
            auto right = parseExpression(lexer, errorMessage);
            left = std::make_unique<BinOpNode>(left->getLocation(), actualBinOp, std::move(left), std::move(right));

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

        // For saving the starting token.
        Location location;
        Location* locationPtr = nullptr;

        while (lexResult)
        {
            if (token.isCharSymbol() && (token.asCharSymbol() == CHAR_PLUS || token.asCharSymbol() == CHAR_MINUS))
            {
                // Valid bin type accept the token

                if (locationPtr == nullptr)
                {
                    locationPtr = &location;
                    lexResult = lexer.nextToken(token, errorMessage, locationPtr);
                }

                else
                {
                    lexResult = lexer.nextToken(token, errorMessage);
                }
            }

            else
            {
                return left;
            }

            const auto actualBinOp = token.asCharSymbol() == CHAR_PLUS ? EnumBinOpNodeType::ADD: EnumBinOpNodeType::SUBTRACT;
            auto right = parseExpression(lexer, errorMessage);
            left = std::make_unique<BinOpNode>(left->getLocation(), actualBinOp, std::move(left), std::move(right));

            // Lookahead for next iteration.
            lexer.peekNextToken(token);
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

        // For saving the starting token.
        Location location;
        Location* locationPtr = nullptr;

        while (lexResult)
        {
            if (token.isCharSymbol() && token.asCharSymbol() == CHAR_EQUALS)
            {
                // Valid bin type accept the token

                if (locationPtr == nullptr)
                {
                    locationPtr = &location;
                    lexResult = lexer.nextToken(token, errorMessage, locationPtr);
                }

                else
                {
                    lexResult = lexer.nextToken(token, errorMessage);
                }
            }

            else
            {
                return left;
            }

            auto right = parseExpression(lexer, errorMessage);
            left = std::make_unique<BinOpNode>(left->getLocation(), EnumBinOpNodeType::ASSIGNMENT, std::move(left), std::move(right));

            // Lookahead for next iteration.
            lexer.peekNextToken(token);
        }

        return left;
    }

    /* static */
    std::unique_ptr<ExpressionNode> parseLitteralOrLRValueNode(Lexer& lexer, std::string* errorMessage)
    {
        const auto snapshot = lexer.snap();
        auto token = newToken();

        // For saving the starting token.
        Location location;
        const bool lexResult = lexer.nextToken(token, errorMessage, &location);

        if (!lexResult)
        {
            return nullptr;
        }

        switch (token.getType())
        {
        case TokenType::BOOL:
            return std::make_unique<LitteralNode>(location, token.asBool());
        case TokenType::CHAR:
            return std::make_unique<LitteralNode>(location, token.asChar());
        case TokenType::FLOAT:
            return std::make_unique<LitteralNode>(location, token.asFloat());
        case TokenType::DOUBLE:
            return std::make_unique<LitteralNode>(location, token.asDouble());
        case TokenType::INT16:
            return std::make_unique<LitteralNode>(location, token.asInt16());
        case TokenType::INT32:
            return std::make_unique<LitteralNode>(location, token.asInt32());
        case TokenType::INT64:
            return std::make_unique<LitteralNode>(location, token.asInt64());
        case TokenType::STRING:
        {
            const auto size = token.asCString().size();

            // Note: +1 for null-terminating character
            char* copyStr = new char[size + 1];
            std::strncpy(copyStr, token.asCString().c_str(), size);

            // Ensure last char is null-terminated
            copyStr[size] = '\0';

            return std::make_unique<LitteralNode>(location, copyStr);
        }
        case TokenType::NULL_T:
            return std::make_unique<LitteralNode>(location);
        case TokenType::CHAR_SYMBOL:
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
    std::unique_ptr<ExpressionNode> parseUnaryExpression(Lexer& lexer, std::string* errorMessage)
    {
        [[maybe_unused]]
        static Reporter& reporter = Reporter::instance();

        const auto snapshot = lexer.snap();
        Location unaryOpLocation;
        auto token = newToken();

        std::optional<EnumUnaryOpType> optionalEnumUnaryOpType;

        if (lexer.nextToken(token, errorMessage, &unaryOpLocation) && (token.isCharSymbol() || token.isStringSymbol()))
        {
            optionalEnumUnaryOpType = getOpType(token);

            // Invalid EnumUnaryOpType present, restore.
            if (!optionalEnumUnaryOpType.has_value())
            {
                // Token was a successfully lex'd symbol that is a char or string symbol, but is NOT a unary operator.  Do nothing and continue...
                lexer.restore(snapshot);
                // TODO: test
                return nullptr;
            }
        }

        // Unsuccessful lex'd symbol or is not a candidate as a EnumUnaryOpType, restore and continue parsing.
        else
        {
            lexer.restore(snapshot);
            // TODO: test
            return nullptr;
        }

        // Check to see if this expression is being dereferenced (i.e. '**x').
        auto optionalDimensionCount = parsePointerInderectionCount(lexer, errorMessage, nullptr);
        auto optionalVariable = parseVariableNode(lexer, errorMessage);

        // Not a VariableNode, bail out of this parse.
        if (!optionalVariable.has_value())
        {
            lexer.restore(snapshot);
            return nullptr;
        }

        // Note: there are a few cases to consider.  We should consider postfix
        // case as well, but we'll leave this for future implementation (TODO).
        // Also, any incompatibilities must be left for the Analyzer to verify
        // this expression is valid or not.
        // 1. *--X
        // 2. --*X
        // 3. --x
        // 4. *x

        std::unique_ptr<ExpressionNode> result = std::make_unique<VariableNode>(std::move(*optionalVariable));

        // DerefNode '*x' case:
        if (optionalDimensionCount.has_value())
        {
            result = buildDerefNode(*optionalDimensionCount, std::move(result));
        }

        // If there was a unary op involved, wrap whatever the current working expression is with a unary op node.
        if (optionalEnumUnaryOpType.has_value())
        {
            result = std::make_unique<UnaryOpNode>(unaryOpLocation, *optionalEnumUnaryOpType, std::move(result));
        }

        return result;
    }

    /* static */
    std::optional<VariableNode> parseVariableNode(Lexer& lexer, std::string* errorMessage)
    {
        const auto snapshot = lexer.snap();
        auto token = newToken();

        // For saving the starting token.
        Location location;
        const bool lexResult = lexer.nextToken(token, errorMessage, &location);

        if (!lexResult || !token.isStringSymbol())
        {
            lexer.restore(snapshot);
            return std::nullopt;
        }

        // else
        return std::make_optional<VariableNode>(location, token.asStringSymbol());
    }

    /* static */
    std::optional<TypeNode> parseTypeNode(Lexer& lexer, std::string* errorMessage)
    {
        auto token = newToken();

        // For saving the starting token.
        Location location;
        bool lexResult = lexer.nextToken(token, errorMessage, &location);

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
                const bool wasStruct = *enumType == EnumCType::STRUCT;
                std::optional<std::string> structName;

                // If it was a struct, we need to get the name of the struct.
                if (wasStruct)
                {
                    Location structNameLoc;
                    lexResult = lexer.nextToken(token, errorMessage, &structNameLoc);

                    if (!lexResult || !token.isStringSymbol())
                    {
                        if (canWriteErrorMessage(errorMessage))
                        {
                            *errorMessage = "Failed to get the name of the struct being parsed";
                        }

                        return std::nullopt;
                    }

                    structName = std::make_optional(token.asStringSymbol());
                }

                Location dimLocation;
                auto optionalDimensionCount = parsePointerInderectionCount(lexer, errorMessage, &dimLocation);

                if (optionalDimensionCount.has_value())
                {
                    return std::make_optional<TypeNode>(dimLocation, CType(enumType.value(), *optionalDimensionCount, std::move(structName)));
                }

                // else
                return std::make_optional<TypeNode>(location, CType(enumType.value(), 0, std::move(structName)));
            }
        }

        return std::nullopt;
    }
}

#if OS_WIN
#pragma warning(pop)
#endif

