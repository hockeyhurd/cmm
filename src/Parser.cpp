/**
 * The parser to our compiler.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#include <cmm/Parser.h>
#include <cmm/BinOpNode.h>
#include <cmm/CompilationUnitNode.h>
#include <cmm/LitteralNode.h>
#include <cmm/Token.h>

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

    static std::shared_ptr<Node> parseBinOpNode(Lexer& lexer, std::string* errorMessage);
    static std::shared_ptr<Node> parseLitteralNode(Lexer& lexer, std::string* errorMessage);

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

        const auto node = parseBinOpNode(lexer, errorMessage);

        // For now since we don't have proper statements, expect a semi-colon here.
        return expectSemicolon(lexer, errorMessage) ? std::make_shared<CompilationUnitNode>(node) : nullptr;
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
    std::shared_ptr<Node> parseBinOpNode(Lexer& lexer, std::string* errorMessage)
    {
        /*Token token('\0', false);
        bool lexResult = lexer.nextToken(token, errorMessage);

        if (!lexResult)
        {
            return nullptr;
        }*/

        auto first = parseLitteralNode(lexer, errorMessage);

        if (first == nullptr)
        {
            return nullptr;
        }

        Token token('\0', false);
        bool lexResult = lexer.peekNextToken(token);

        if (!lexResult)
        {
            return first;
        }

        auto optBinOpType = isEnumBinOpType(token);

        if (!optBinOpType.has_value())
        {
            // return expectSemicolon(lexer, errorMessage) ? first : nullptr;
            return first;
        }

        else
        {
            // Valid bin type accept the token
            lexResult = lexer.nextToken(token, errorMessage);
        }

        auto second = parseBinOpNode(lexer, errorMessage);

        if (first == nullptr)
        {
            // Parse error?
            if (errorMessage != nullptr)
            {
                std::ostringstream os;
                os << "[PARSER] error: expected litteral or expression at "
                    << lexer.getLocation();
            }

            return nullptr;
        }

        return std::make_shared<BinOpNode>(optBinOpType.value(), first, second);
    }

    /* static */
    std::shared_ptr<Node> parseLitteralNode(Lexer& lexer, std::string* errorMessage)
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
        // Unimplemented types
        default:
            std::ostringstream os;
            os << "unexpected token type received '"
               << toString(token.getType()) << '\'';
            unimplementedAbort(os.str());
            return nullptr;
        }

        return nullptr;
    }
}

