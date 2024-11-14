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
    std::unique_ptr<ASTNode> parseMainMethodDeclaration(int& indentLevel);
    std::unique_ptr<ASTNode> parseStatementList(int& indentLevel);
    std::unique_ptr<ASTNode> parseStatement(int& indentLevel);
    std::unique_ptr<ASTNode> parseAssignment(int& indentLevel);
    std::unique_ptr<ASTNode> parseOutput(int& indentLevel);
    std::unique_ptr<ASTNode> parseInput(int& indentLevel);
    std::unique_ptr<ASTNode> parseIfStatement(int& indentLevel);
    std::unique_ptr<ASTNode> parseLoopStatement(int& indentLevel);
    std::unique_ptr<ASTNode> parseRelationalStatement(int& indentLevel);
    std::unique_ptr<ASTNode> parseBooleanExpression(int& indentLevel);
    std::unique_ptr<ASTNode> parseExpression(int& indentLevel);
    std::unique_ptr<ASTNode> parseTerm(int& indentLevel);

    const Token& currentToken() const;
    void advanceToken();
    bool match(TokenType type);
    
};

#endif 