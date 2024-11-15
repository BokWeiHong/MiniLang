#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <unordered_map>
#include <regex>
#include <memory>

#include "Token.hpp"

// Define Lexer class
class Lexer {
    public:
        Lexer(const std::string& sourceCode);
        std::vector<Token> tokenize();

    private:
        std::string sourceCode;
        // Static constant unordered map of keywords and their corresponding token types
        static const std::unordered_map<std::string, TokenType> KEYWORDS;    
};

#endif