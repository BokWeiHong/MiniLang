#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>

enum class TokenType {
    IDENTIFIER, 
    INT_LITERAL, 
    OPERATOR, 
    KEYWORD, 
    END_OF_FILE, 
    DATA_TYPE, 
    STRING_LITERAL, 
    COMMENT, 
    DELIMITER,
    UNKNOWN
};

static std::ostream& operator<<(std::ostream& os, TokenType tt) {
    switch (tt) {
        case TokenType::IDENTIFIER: return os << "IDENTIFIER";
        case TokenType::INT_LITERAL: return os << "INT_LITERAL";
        case TokenType::OPERATOR: return os << "OPERATOR";
        case TokenType::KEYWORD: return os << "KEYWORD";
        case TokenType::END_OF_FILE: return os << "END_OF_FILE";
        case TokenType::DATA_TYPE: return os << "DATA_TYPE";
        case TokenType::STRING_LITERAL: return os << "STRING_LITERAL";
        case TokenType::COMMENT: return os << "COMMENT";
        case TokenType::DELIMITER: return os << "DELIMITER";
        default: return os << "UNKNOWN";
    }
}

struct Token {
    public:
        TokenType type;
        std::string value;

        Token(TokenType type, const std::string& value)
            : type(type), value(value) {}

        TokenType getType() const{
            return type;
        }

        const std::string& getValue() const {
            return value;
        }
};

#endif