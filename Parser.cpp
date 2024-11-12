#include "Parser.hpp"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), currentTokenIndex(0) {}

std::unique_ptr<ASTNode> Parser::parse() {
    return parseProgram();
}

std::unique_ptr<ASTNode> Parser::parseProgram() {
    return parseMainMethodDeclaration();
}

std::unique_ptr<ASTNode> Parser::parseMainMethodDeclaration() {
    if (match(TokenType::KEYWORD) && currentToken().getValue() == "class") {
        advanceToken();
        if (match(TokenType::KEYWORD) && currentToken().getValue() == "Main") {
            advanceToken();
            if (match(TokenType::DELIMITER) && currentToken().getValue() == "(") {
                advanceToken();
                if (match(TokenType::DELIMITER) && currentToken().getValue() == ")") {
                    advanceToken();
                    if (match(TokenType::DELIMITER) && currentToken().getValue() == "{") {
                        advanceToken();
                        auto statements = parseStatementList();
                        if (match(TokenType::DELIMITER) && currentToken().getValue() == "}") {
                            advanceToken();
                            return std::make_unique<ASTNode>("MainMethodDeclaration", std::move(statements));
                        }
                    }
                }
            }
        }
    } else {
        std::cout << "Error: Expected 'class' keyword but found '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
    }
    return nullptr; // Error handling should be added
}

std::unique_ptr<ASTNode> Parser::parseStatementList() {
    auto node = std::make_unique<ASTNode>("StatementList");
    while (true) {
        auto statement = parseStatement();
        if (!statement) break;
        node->addChild(std::move(statement));
    }
    return node;
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    // Implement parsing logic for different types of statements
    return nullptr;
}

// Implement other parsing methods...

const Token& Parser::currentToken() const {
    return tokens[currentTokenIndex];
}

void Parser::advanceToken() {
    if (currentTokenIndex < tokens.size()) {
        currentTokenIndex++;
    }
}

bool Parser::match(TokenType type) {
    return currentToken().getType() == type;
} 