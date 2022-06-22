/**
 * The parser to our compiler.
 *
 * @author hockeyhurd
 * @version 2022-06-14
 */

#include <cmm/Parser.h>
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

    static std::shared_ptr<LitteralNode> parseLitteralNode(Lexer& lexer, std::string* errorMessage);

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

        const auto node = parseLitteralNode(lexer, errorMessage);
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
    std::shared_ptr<LitteralNode> parseLitteralNode(Lexer& lexer, std::string* errorMessage)
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
            return expectSemicolon(lexer, errorMessage) ? std::make_shared<LitteralNode>(token.asBool()) : nullptr;
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

