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
    // Vector of tokens to be parsed
    const std::vector<Token>& tokens;
    // Index of the current token
    size_t currentTokenIndex;

    /*    
    The parse methods are used to parse the source code and convert it into an abstract syntax tree (AST).
    Each method corresponds to a different type of statement or expression in the language.
    They take an indent level as a parameter, which is used to determine the indentation level of the current node in the AST.
    The parse methods are based on the grammar rules of the language in the Context-Free-Grammar.txt file.
    */
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
    
    // Methods to get the current token and advance to the next token 
    const Token& currentToken() const;
    void advanceToken();
    // Method to check if the current token matches a given type
    bool match(TokenType type);
    
};

#endif 