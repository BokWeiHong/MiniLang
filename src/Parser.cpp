#include "Parser.hpp"
#include "ASTNode.hpp"

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), currentTokenIndex(0) {}

std::unique_ptr<ASTNode> Parser::parse() {
    return parseProgram();
}

// Method to parse the entire program
std::unique_ptr<ASTNode> Parser::parseProgram() {
    int indentLevel = 0;
    auto programNode = std::make_unique<ASTNode>("StartProgram", -1);
    //std::cout << "Parsing Program" << std::endl;
    auto mainMethodDeclaration = parseMainMethodDeclaration(indentLevel);
    if (mainMethodDeclaration) {
        programNode->addChild(std::move(mainMethodDeclaration));
    }
    programNode->addChild(std::make_unique<ASTNode>("EndProgram", -1));
    return programNode;
}

// Method to parse the main method declaration
std::unique_ptr<ASTNode> Parser::parseMainMethodDeclaration(int& indentLevel) {
    int newIndentLevel = indentLevel;
    auto classNode = std::make_unique<ASTNode>("MainMethodDeclaration", newIndentLevel);
    //std::cout << "Parsing MainMethodDeclaration" << std::endl;
    if (match(TokenType::KEYWORD) && currentToken().getValue() == "class") {
        classNode->addChild(std::make_unique<ASTNode>("class", newIndentLevel));
        advanceToken();
        newIndentLevel++;
        if (match(TokenType::KEYWORD) && currentToken().getValue() == "Main") {
            classNode->addChild(std::make_unique<ASTNode>("Main", newIndentLevel));
            advanceToken();
            if (match(TokenType::DELIMITER) && currentToken().getValue() == "(") {
                classNode->addChild(std::make_unique<ASTNode>("(", newIndentLevel));
                advanceToken();
                if (match(TokenType::DELIMITER) && currentToken().getValue() == ")") {
                    classNode->addChild(std::make_unique<ASTNode>(")", newIndentLevel));
                    advanceToken();
                    if (match(TokenType::DELIMITER) && currentToken().getValue() == "{") {
                        classNode->addChild(std::make_unique<ASTNode>("{", newIndentLevel));
                        advanceToken();
                        auto statements = parseStatementList(newIndentLevel);
                        if (statements) {
                            classNode->addChild(std::move(statements));
                        }
                        if (match(TokenType::DELIMITER) && currentToken().getValue() == "}") {
                            classNode->addChild(std::make_unique<ASTNode>("}", newIndentLevel));
                            advanceToken();
                            return classNode;
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

// Method to parse the statement list
std::unique_ptr<ASTNode> Parser::parseStatementList(int& indentLevel) {
    int newIndentLevel = indentLevel;
    auto statementListNode = std::make_unique<ASTNode>("StatementList", newIndentLevel);
    //std::cout << "Parsing StatementList" << std::endl;
    while (true) {
        auto statement = parseStatement(newIndentLevel);
        if (statement != nullptr) {
            statementListNode->addChild(std::move(statement));
        } else {
            break;
        }
    }
    return statementListNode;
}

// Method to parse a statement
std::unique_ptr<ASTNode> Parser::parseStatement(int& indentLevel) {
    int newIndentLevel = indentLevel;
    newIndentLevel++;
    auto statementNode = std::make_unique<ASTNode>("Statement", newIndentLevel);
    //std::cout << "Parsing Statement" << std::endl;
    if (match(TokenType::DATA_TYPE) || match(TokenType::IDENTIFIER)) {
        statementNode->addChild(parseAssignment(newIndentLevel));
        return statementNode;
    } else if (match(TokenType::KEYWORD) && currentToken().getValue() == "out") {
        statementNode->addChild(parseOutput(newIndentLevel));
        return statementNode;
    } else if (match(TokenType::KEYWORD) && currentToken().getValue() == "in") {
        statementNode->addChild(parseInput(newIndentLevel));
        return statementNode;
    } else if (match(TokenType::KEYWORD) && currentToken().getValue() == "if") {
        statementNode->addChild(parseIfStatement(newIndentLevel));
        return statementNode;
    } else if (match(TokenType::KEYWORD) && currentToken().getValue() == "while") {
        statementNode->addChild(parseLoopStatement(newIndentLevel));
        return statementNode;
    } else if (match(TokenType::DELIMITER) && currentToken().getValue() == "}") {
        return nullptr;
    } else {
        std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Unexpected token '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

// Method to parse an assignment statement
std::unique_ptr<ASTNode> Parser::parseAssignment(int& indentLevel) {
    int newIndentLevel = indentLevel;
    auto assignmentNode = std::make_unique<ASTNode>("Assignment", newIndentLevel);
    //std::cout << "Parsing Assignment" << std::endl;
    if (match(TokenType::DATA_TYPE)) {
        newIndentLevel++;
        assignmentNode->addChild(std::make_unique<ASTNode>("DataType", newIndentLevel));
        assignmentNode->addChild(std::make_unique<ASTNode>(currentToken().getValue(), newIndentLevel + 1));
        advanceToken();
        if (match(TokenType::IDENTIFIER)) {
            assignmentNode->addChild(std::make_unique<ASTNode>("Identifier", newIndentLevel));
            assignmentNode->addChild(std::make_unique<ASTNode>(currentToken().getValue(), newIndentLevel + 1));
            advanceToken();
            if (match(TokenType::OPERATOR) && currentToken().getValue() == "=") {
                assignmentNode->addChild(std::make_unique<ASTNode>("=", newIndentLevel));
                advanceToken();
                auto expression = parseExpression(newIndentLevel);
                if (expression != nullptr) {
                    assignmentNode->addChild(std::move(expression));
                }
            }
            if (match(TokenType::DELIMITER) && currentToken().getValue() == ";") {
                    assignmentNode->addChild(std::make_unique<ASTNode>(";", newIndentLevel));
                    advanceToken();
                    return assignmentNode;
                } else {
                    std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected ';' after expression" << " at line " << currentToken().getLine() << std::endl;
                std::exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected identifier after data type" << " at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else if (match(TokenType::IDENTIFIER)) {
        assignmentNode->addChild(std::make_unique<ASTNode>("Identifier", newIndentLevel));
        assignmentNode->addChild(std::make_unique<ASTNode>(currentToken().getValue(), newIndentLevel + 1));
        advanceToken();
        if (match(TokenType::OPERATOR) && currentToken().getValue() == "=") {
            assignmentNode->addChild(std::make_unique<ASTNode>("=", newIndentLevel));
            advanceToken();
            auto expression = parseExpression(newIndentLevel);
            if (expression != nullptr) {
                assignmentNode->addChild(std::move(expression));
            }
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected '=' after identifier" << " at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
        if (match(TokenType::DELIMITER) && currentToken().getValue() == ";") {
            assignmentNode->addChild(std::make_unique<ASTNode>(";", newIndentLevel));
            advanceToken();
            return assignmentNode;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected ';' after expression" << " at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    return nullptr;
}

// Method to parse an output statement
std::unique_ptr<ASTNode> Parser::parseOutput(int& indentLevel) {
    int newIndentLevel = indentLevel;
    auto outputNode = std::make_unique<ASTNode>("OutputStatement", newIndentLevel);
    //std::cout << "Parsing OutputStatement" << std::endl;
    if (match(TokenType::KEYWORD) && currentToken().getValue() == "out") {
        newIndentLevel++;
        outputNode->addChild(std::make_unique<ASTNode>("out", newIndentLevel));
        advanceToken();
        if (match(TokenType::DELIMITER) && currentToken().getValue() == ":") {
            outputNode->addChild(std::make_unique<ASTNode>(":", newIndentLevel));
            advanceToken();
            auto expression = parseExpression(newIndentLevel);
            if (expression != nullptr) {
                outputNode->addChild(std::move(expression));
            }
            if (match(TokenType::DELIMITER) && currentToken().getValue() == ";") {
                outputNode->addChild(std::make_unique<ASTNode>(";", newIndentLevel));
                advanceToken();
                return outputNode;
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

// Method to parse an input statement
std::unique_ptr<ASTNode> Parser::parseInput(int& indentLevel) {
    int newIndentLevel = indentLevel;
    auto inputNode = std::make_unique<ASTNode>("InputStatement", newIndentLevel);
    //std::cout << "Parsing InputStatement" << std::endl;
    if (match(TokenType::KEYWORD) && currentToken().getValue() == "in") {
        newIndentLevel++;
        inputNode->addChild(std::make_unique<ASTNode>("in", newIndentLevel));
        advanceToken();
        if (match(TokenType::DELIMITER) && currentToken().getValue() == ":") {
            inputNode->addChild(std::make_unique<ASTNode>(":", newIndentLevel));
            advanceToken();
            if (match(TokenType::IDENTIFIER)) {
                inputNode->addChild(std::make_unique<ASTNode>("Identifier", newIndentLevel));
                inputNode->addChild(std::make_unique<ASTNode>(currentToken().getValue(), newIndentLevel + 1));
                advanceToken();
                if (match(TokenType::DELIMITER) && currentToken().getValue() == ";") {
                    inputNode->addChild(std::make_unique<ASTNode>(";", newIndentLevel));
                    advanceToken();
                    return inputNode;
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

// Method to parse an if statement
std::unique_ptr<ASTNode> Parser::parseIfStatement(int& indentLevel) {
    int newIndentLevel = indentLevel;
    auto ifNode = std::make_unique<ASTNode>("IfStatement", newIndentLevel);
    //std::cout << "Parsing IfStatement" << std::endl;
    if (match(TokenType::KEYWORD) && currentToken().getValue() == "if") {
        newIndentLevel++;
        ifNode->addChild(std::make_unique<ASTNode>("if", newIndentLevel));
        advanceToken();
        if (match(TokenType::DELIMITER) && currentToken().getValue() == "(") {
            ifNode->addChild(std::make_unique<ASTNode>("(", newIndentLevel));
            advanceToken();
            auto expression = parseBooleanExpression(newIndentLevel);
            if (expression != nullptr) {
                ifNode->addChild(std::move(expression));
            }
            if (match(TokenType::DELIMITER) && currentToken().getValue() == ")") {
                ifNode->addChild(std::make_unique<ASTNode>(")", newIndentLevel));
                advanceToken();
                if (match(TokenType::DELIMITER) && currentToken().getValue() == "{") {
                    ifNode->addChild(std::make_unique<ASTNode>("{", newIndentLevel));
                    advanceToken();
                    auto statements = parseStatementList(newIndentLevel);
                    if (statements != nullptr) {
                        ifNode->addChild(std::move(statements));
                    }
                    if (match(TokenType::DELIMITER) && currentToken().getValue() == "}") {
                        ifNode->addChild(std::make_unique<ASTNode>("}", newIndentLevel));
                        advanceToken();
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

    if (match(TokenType::KEYWORD) && currentToken().getValue() == "else") {
        newIndentLevel--;
        ifNode->addChild(std::make_unique<ASTNode>("else", newIndentLevel));
        advanceToken();
        if (match(TokenType::DELIMITER) && currentToken().getValue() == "{") {
            ifNode->addChild(std::make_unique<ASTNode>("{", newIndentLevel));
            advanceToken();
            auto statements = parseStatementList(newIndentLevel);
            if (statements != nullptr) {
                ifNode->addChild(std::move(statements));
            }
            if (match(TokenType::DELIMITER) && currentToken().getValue() == "}") {
                ifNode->addChild(std::make_unique<ASTNode>("}", newIndentLevel));
                advanceToken();
                return ifNode;
            } else {
                std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected '}' after statement list" << " at line " << currentToken().getLine() << std::endl;
                std::exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected '{' after 'if' keyword" << " at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else {
        return ifNode;
    }
}

// Method to parse a loop statement
std::unique_ptr<ASTNode> Parser::parseLoopStatement(int& indentLevel) {
    int newIndentLevel = indentLevel;
    auto loopNode = std::make_unique<ASTNode>("LoopStatement", newIndentLevel);
    //std::cout << "Parsing LoopStatement" << std::endl;
    if (match(TokenType::KEYWORD) && currentToken().getValue() == "while") {
        newIndentLevel++;
        loopNode->addChild(std::make_unique<ASTNode>("while", newIndentLevel));
        advanceToken();
        if (match(TokenType::DELIMITER) && currentToken().getValue() == "(") {
            loopNode->addChild(std::make_unique<ASTNode>("(", newIndentLevel));
            advanceToken();
            auto expression = parseBooleanExpression(newIndentLevel);
            if (expression != nullptr) {
                loopNode->addChild(std::move(expression));
            }
            if (match(TokenType::DELIMITER) && currentToken().getValue() == ")") {
                loopNode->addChild(std::make_unique<ASTNode>(")", newIndentLevel));
                advanceToken();
                if (match(TokenType::DELIMITER) && currentToken().getValue() == "{") {
                    loopNode->addChild(std::make_unique<ASTNode>("{", newIndentLevel));
                    advanceToken();
                    auto statements = parseStatementList(newIndentLevel);
                    if (statements != nullptr) {
                        loopNode->addChild(std::move(statements));
                        if (match(TokenType::DELIMITER) && currentToken().getValue() == "}") {
                            loopNode->addChild(std::make_unique<ASTNode>("}", newIndentLevel));
                            advanceToken();
                            return loopNode;
                        } else {
                            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected '}' after statement list" << " at line " << currentToken().getLine() << std::endl;
                            std::exit(EXIT_FAILURE);
                        }
                    } else {
                        std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected statement list after '{'" << " at line " << currentToken().getLine() << std::endl;
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

// Method to parse a relational statement
std::unique_ptr<ASTNode> Parser::parseRelationalStatement(int& indentLevel) {
    int newIndentLevel = indentLevel;
    auto relationalStatementNode = std::make_unique<ASTNode>("RelationalStatement", newIndentLevel);
    newIndentLevel++;
    //std::cout << "Parsing RelationalStatement" << std::endl;
    auto expression1 = parseExpression(newIndentLevel);
    if (expression1 == nullptr) {
        std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected an expression but found '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
        std::exit(EXIT_FAILURE);
    } else {
        relationalStatementNode->addChild(std::move(expression1));
    }
    if (match(TokenType::OPERATOR) && currentToken().getValue() == "==") {
        relationalStatementNode->addChild(std::make_unique<ASTNode>("==", newIndentLevel));
        advanceToken();
        auto expression2 = parseExpression(newIndentLevel);
        if (expression2 != nullptr) {
            relationalStatementNode->addChild(std::move(expression2));
            return relationalStatementNode;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Unknown token '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else if (match(TokenType::OPERATOR) && currentToken().getValue() == ">=") {
        relationalStatementNode->addChild(std::make_unique<ASTNode>(">=", newIndentLevel));
        advanceToken();
        auto expression2 = parseExpression(newIndentLevel);
        if (expression2 != nullptr) {
            relationalStatementNode->addChild(std::move(expression2));
            return relationalStatementNode;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Unknown token '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else if (match(TokenType::OPERATOR) && currentToken().getValue() == "<=") {
        relationalStatementNode->addChild(std::make_unique<ASTNode>("<=", newIndentLevel));
        advanceToken();
        auto expression2 = parseExpression(newIndentLevel);
        if (expression2 != nullptr) {
            relationalStatementNode->addChild(std::move(expression2));
            return relationalStatementNode;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Unknown token '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else if (match(TokenType::OPERATOR) && currentToken().getValue() == ">") {
        relationalStatementNode->addChild(std::make_unique<ASTNode>(">", newIndentLevel));
        advanceToken();
        auto expression2 = parseExpression(newIndentLevel);
        if (expression2 != nullptr) {
            relationalStatementNode->addChild(std::move(expression2));
            return relationalStatementNode;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Unknown token '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    } else if (match(TokenType::OPERATOR) && currentToken().getValue() == "<") {
        relationalStatementNode->addChild(std::make_unique<ASTNode>("<", newIndentLevel));
        advanceToken();
        auto expression2 = parseExpression(newIndentLevel);
        if (expression2 != nullptr) {
            relationalStatementNode->addChild(std::move(expression2));
            return relationalStatementNode;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Unknown token '" << currentToken().getValue() << "' at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    return nullptr;
}

// Method to parse a boolean expression
std::unique_ptr<ASTNode> Parser::parseBooleanExpression(int& indentLevel) {
    int newIndentLevel = indentLevel;
    auto booleanExpressionNode = std::make_unique<ASTNode>("BooleanExpression", newIndentLevel);
    //std::cout << "Parsing BooleanExpression" << std::endl;
    newIndentLevel++;
    if (match(TokenType::BOOLEAN_LITERAL)) {
        booleanExpressionNode->addChild(std::make_unique<ASTNode>(currentToken().getValue(), newIndentLevel));
        advanceToken();
        return booleanExpressionNode;
    } else {
        auto relationalStatement = parseRelationalStatement(newIndentLevel);
        if (relationalStatement != nullptr) {
            booleanExpressionNode->addChild(std::move(relationalStatement));
            return booleanExpressionNode;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected a relational statement" << " at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    return nullptr;
}

// Method to parse an expression
std::unique_ptr<ASTNode> Parser::parseExpression(int& indentLevel) {
    int newIndentLevel = indentLevel;
    auto expressionNode = std::make_unique<ASTNode>("Expression", newIndentLevel);
    //std::cout << "Parsing Expression" << std::endl;
    newIndentLevel++;
    auto term = parseTerm(newIndentLevel);
    if (term == nullptr) {
        std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected a term" << " at line " << currentToken().getLine() << std::endl;
        std::exit(EXIT_FAILURE);
    } else {
        expressionNode->addChild(std::move(term));
        if (match(TokenType::OPERATOR) && (currentToken().getValue() == "+" || currentToken().getValue() == "-" 
        || currentToken().getValue() == "*" || currentToken().getValue() == "/")) {
        std::string operatorValue = currentToken().getValue();
        expressionNode->addChild(std::make_unique<ASTNode>(currentToken().getValue(), newIndentLevel));
        advanceToken();
        newIndentLevel--;
        auto expression2 = parseExpression(newIndentLevel);
        if (expression2 != nullptr) {
            expressionNode->addChild(std::move(expression2));
            return expressionNode;
        } else {
            std::cerr << "Error in file " << __FILE__ << " at line " << __LINE__ << ": Expected an expression after '" << operatorValue << "' at line " << currentToken().getLine() << std::endl;
            std::exit(EXIT_FAILURE);
            }
        }
    }
    return expressionNode;
}

// Method to parse a term
std::unique_ptr<ASTNode> Parser::parseTerm(int& indentLevel) {
    int newIndentLevel = indentLevel;
    auto termNode = std::make_unique<ASTNode>("Term", newIndentLevel);
    //std::cout << "Parsing Term" << std::endl;
    newIndentLevel++;
    if (match(TokenType::IDENTIFIER)) {
        termNode->addChild(std::make_unique<ASTNode>(currentToken().getValue(), newIndentLevel));
        advanceToken();
        return termNode;
    } else if (match(TokenType::BOOLEAN_LITERAL)) {
        termNode->addChild(std::make_unique<ASTNode>(currentToken().getValue(), newIndentLevel));
        advanceToken();
        return termNode;
    } else if (match(TokenType::INT_LITERAL)) {
        termNode->addChild(std::make_unique<ASTNode>(currentToken().getValue(), newIndentLevel));
        advanceToken();
        return termNode;
    } else if (match(TokenType::FLOAT_LITERAL)) {
        termNode->addChild(std::make_unique<ASTNode>(currentToken().getValue(), newIndentLevel));
        advanceToken();
        return termNode;
    } else if (match(TokenType::STRING_LITERAL)) {
        termNode->addChild(std::make_unique<ASTNode>(currentToken().getValue(), newIndentLevel));
        advanceToken();
        return termNode;
    }
    return nullptr;
}

// Method to get the current token
const Token& Parser::currentToken() const {
    return tokens[currentTokenIndex];
}

// Method to advance to the next token
void Parser::advanceToken() {
    if (currentTokenIndex < tokens.size()) {
        currentTokenIndex++;
    }
}

// Method to check if the current token matches a given type
bool Parser::match(TokenType type) {
    return currentToken().getType() == type;
} 
