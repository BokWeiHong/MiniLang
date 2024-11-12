#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <memory>
#include "Token.hpp"
#include "ASTNode.hpp"

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::unique_ptr<ASTNode> parse();

private:
    const std::vector<Token>& tokens;
    size_t currentTokenIndex;

    std::unique_ptr<ASTNode> parseProgram();
    std::unique_ptr<ASTNode> parseMainMethodDeclaration();
    std::unique_ptr<ASTNode> parseStatementList();
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseAssignment();
    std::unique_ptr<ASTNode> parseOutput();
    std::unique_ptr<ASTNode> parseInput();
    std::unique_ptr<ASTNode> parseIfStatement();
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();

    const Token& currentToken() const;
    void advanceToken();
    bool match(TokenType type);
};

#endif 