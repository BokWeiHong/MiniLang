#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>

// Enum class representing the different types of tokens
enum class TokenType {
    IDENTIFIER, 
    INT_LITERAL, 
    OPERATOR, 
    KEYWORD, 
    END_OF_FILE, 
    DATA_TYPE, 
    STRING_LITERAL, 
    BOOLEAN_LITERAL,
    COMMENT, 
    DELIMITER,
    UNKNOWN
};

// Overload the << operator for TokenType to print the token type as a string
static std::ostream& operator<<(std::ostream& os, TokenType tt) {
    switch (tt) {
        case TokenType::IDENTIFIER: return os << "IDENTIFIER";
        case TokenType::INT_LITERAL: return os << "INT_LITERAL";
        case TokenType::OPERATOR: return os << "OPERATOR";
        case TokenType::KEYWORD: return os << "KEYWORD";
        case TokenType::END_OF_FILE: return os << "END_OF_FILE";
        case TokenType::DATA_TYPE: return os << "DATA_TYPE";
        case TokenType::STRING_LITERAL: return os << "STRING_LITERAL";
        case TokenType::BOOLEAN_LITERAL: return os << "BOOLEAN_LITERAL";
        case TokenType::COMMENT: return os << "COMMENT";
        case TokenType::DELIMITER: return os << "DELIMITER";
        default: return os << "UNKNOWN";
    }
}

struct Token {
    public:
        TokenType type;
        std::string value;
        int line;

        Token(TokenType type, const std::string& value, int line)
            : type(type), value(value), line(line) {}

        // Getter methods
        TokenType getType() const{
            return type;
        }

        // Getter method for the token value
        const std::string& getValue() const {
            return value;
        }

        // Getter method for the token line
        int getLine() const {
            return line;
        }
};

#endif