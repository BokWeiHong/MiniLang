#include "Parser.hpp"

// remember to add ASTNode functions

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
                            return 0;
                        } else {
                            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected '}' after statement list" << " at line " << currentToken().getLine() << std::endl;
                            std::exit(EXIT_FAILURE);
                        }
                    } else {
                        std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected '{' " << currentToken().getValue() << " at line " << currentToken().getLine() << std::endl;
                        std::exit(EXIT_FAILURE);
                    }
                } else {
                    std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected ')' after '(' keyword but found '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
                    std::exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected '(' after 'Main' keyword but found '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
                std::exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected 'Main' keyword but found '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected 'class' keyword but found '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseStatementList() {
    auto statement = parseStatement();
    if (statement) {
        return 0;
    } else {
        return nullptr;
    }
}

std::unique_ptr<ASTNode> Parser::parseStatement() {
    if (match(TokenType::DATA_TYPE)) {
        return parseAssignment();
    } else if (match(TokenType::KEYWORD) && currentToken().getValue() == "out") {
        return parseOutput();
    } else if (match(TokenType::KEYWORD) && currentToken().getValue() == "in") {
        return parseInput();
    } else if (match(TokenType::KEYWORD) && currentToken().getValue() == "if") {
        return parseIfStatement();
    } else if (match(TokenType::KEYWORD) && currentToken().getValue() == "while") {
        return parseLoopStatement();
    } else {
        std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Unexpected token '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

std::unique_ptr<ASTNode> Parser::parseAssignment() {
    if (match(TokenType::DATA_TYPE)) {
        advanceToken();
        if (match(TokenType::IDENTIFIER)) {
            advanceToken();
            if (match(TokenType::OPERATOR) && currentToken().getValue() == "=") {
                advanceToken();
                auto expression = parseExpression();
                if (expression && match(TokenType::DELIMITER) && currentToken().getValue() == ";") {
                    advanceToken();
                    return 0;
                } else {
                    std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected ';' after expression" << " at line " << currentToken().getLine() << std::endl;
                    std::exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected '=' after identifier" << " at line " << currentToken().getLine() << std::endl;
                std::exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected identifier after data type" << " at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseOutput() {
    if (match(TokenType::KEYWORD) && currentToken().getValue() == "out") {
        advanceToken();
        if (match(TokenType::DELIMITER) && currentToken().getValue() == ":") {
            advanceToken();
            auto expression = parseExpression();
            if (match(TokenType::DELIMITER) && currentToken().getValue() == ";") {
                advanceToken();
                return 0;
            } else {
                std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected ';' after expression" << " at line " << currentToken().getLine() << std::endl;
                std::exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected ':' after 'out' " << " at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseInput() {
    if (match(TokenType::KEYWORD) && currentToken().getValue() == "in") {
        advanceToken();
        if (match(TokenType::DELIMITER) && currentToken().getValue() == ":") {
            advanceToken();
            if (match(TokenType::IDENTIFIER)) {
                std::string identifier = currentToken().getValue();
                advanceToken();
                if (match(TokenType::DELIMITER) && currentToken().getValue() == ";") {
                    advanceToken();
                    return 0;
                } else {
                    std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected ';' after identifier" << " at line " << currentToken().getLine() << std::endl;
                    std::exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected identifier after ':'" << " at line " << currentToken().getLine() << std::endl;
                std::exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected ':' after 'in'" << " at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseIfStatement() {
    if (match(TokenType::KEYWORD) && currentToken().getValue() == "if") {
        advanceToken();
        if (match(TokenType::DELIMITER) && currentToken().getValue() == "(") {
            advanceToken();
            auto expression = parseBooleanExpression();
            if (match(TokenType::DELIMITER) && currentToken().getValue() == ")") {
                advanceToken();
                if (match(TokenType::DELIMITER) && currentToken().getValue() == "{") {
                    advanceToken();
                    auto statements = parseStatementList();
                    if (match(TokenType::DELIMITER) && currentToken().getValue() == "}") {
                        advanceToken();
                        return 0;
                    } else {
                        std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected '}' after statement list" << " at line " << currentToken().getLine() << std::endl;
                        std::exit(EXIT_FAILURE);
                    }
                } else {
                    std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected '{' after 'if' keyword" << " at line " << currentToken().getLine() << std::endl;
                    std::exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected ')' after expression" << " at line " << currentToken().getLine() << std::endl;
                std::exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected '(' after 'if' keyword'" << " at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else {
        std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected 'if' keyword but found '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseLoopStatement() {
    if (match(TokenType::KEYWORD) && currentToken().getValue() == "while") {
        advanceToken();
        if (match(TokenType::DELIMITER) && currentToken().getValue() == "(") {
            advanceToken();
            auto expression = parseBooleanExpression();
            if (match(TokenType::DELIMITER) && currentToken().getValue() == ")") {
                advanceToken();
                if (match(TokenType::DELIMITER) && currentToken().getValue() == "{") {
                    advanceToken();
                    auto statements = parseStatementList();
                    if (match(TokenType::DELIMITER) && currentToken().getValue() == "}") {
                        advanceToken();
                        return 0;
                    } else {
                        std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected '}' after statement list" << " at line " << currentToken().getLine() << std::endl;
                        std::exit(EXIT_FAILURE);
                    }
                } else {
                    std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected '{' after ')'" << " at line " << currentToken().getLine() << std::endl;
                    std::exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected ')' after expression" << " at line " << currentToken().getLine() << std::endl;
                std::exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected '(' after 'while' keyword" << " at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else {
        std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected 'while' keyword but found '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseRelationalStatement() {
    auto expression1 = parseExpression();
    if (!expression1) {
        std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected an expression but found '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
        return nullptr;
    }
    if (match(TokenType::OPERATOR) && currentToken().getValue() == "==") {
        advanceToken();
        auto expression2 = parseExpression();
        if (expression2) {
            return 0;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Unknown token '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else if (match(TokenType::OPERATOR) && currentToken().getValue() == ">=") {
        advanceToken();
        auto expression2 = parseExpression();
        if (expression2) {
            return 0;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Unknown token '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else if (match(TokenType::OPERATOR) && currentToken().getValue() == "<=") {
        advanceToken();
        auto expression2 = parseExpression();
        if (expression2) {
            return 0;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Unknown token '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else if (match(TokenType::OPERATOR) && currentToken().getValue() == ">") {
        advanceToken();
        auto expression2 = parseExpression();
        if (expression2) {
            return 0;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Unknown token '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else if (match(TokenType::OPERATOR) && currentToken().getValue() == "<") {
        advanceToken();
        auto expression2 = parseExpression();
        if (expression2) {
            return 0;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Unknown token '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else {
        return expression1;
    }
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseBooleanExpression() {
    if (match(TokenType::BOOLEAN_LITERAL)) {
        advanceToken();
        return 0;
    } else if (match(TokenType::KEYWORD) && currentToken().getValue() == "NOT") {
        advanceToken();
        auto relationalStatement = parseRelationalStatement();
        if (relationalStatement) {
            return 0;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected a relational statement after 'NOT' at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else {
        auto leftRelationalStatement = parseRelationalStatement();
        if (match(TokenType::OPERATOR) && (currentToken().getValue() == "AND" || currentToken().getValue() == "OR")) {
            std::string operatorValue = currentToken().getValue();
            advanceToken();
            auto rightRelationalStatement = parseRelationalStatement();
            if (rightRelationalStatement) {
                return 0;
            } else {
                std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected a relational statement after '" << operatorValue << "' at line " << currentToken().getLine() << std::endl;
                std::exit(EXIT_FAILURE);
            }
        } else if (!leftRelationalStatement) {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected a relational statement" << " at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        } else {
            return 0;
        }
    }
    return nullptr;
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    auto term = parseTerm();
    if (!term) {
        std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected a term" << " at line " << currentToken().getLine() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    if (match(TokenType::OPERATOR) && (currentToken().getValue() == "+" || currentToken().getValue() == "-" 
        || currentToken().getValue() == "*" || currentToken().getValue() == "/")) {
        std::string operatorValue = currentToken().getValue();
        advanceToken();
        auto expression2 = parseExpression();
        if (expression2) {
            return 0;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected an expression after '" << operatorValue << "' at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    return term;
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    if (match(TokenType::IDENTIFIER)) {
        advanceToken();
        return 0;
    } else if (match(TokenType::BOOLEAN_LITERAL)) {
        advanceToken();
        return 0;
    } else if (match(TokenType::INT_LITERAL)) {
        advanceToken();
        return 0;
    } else if (match(TokenType::STRING_LITERAL)) {
        advanceToken();
        return 0;
    }
    return nullptr;
}


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