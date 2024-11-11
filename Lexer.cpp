#include "Lexer.hpp"

#include <iostream>
#include <vector>
#include <regex>

const std::unordered_map<std::string, TokenType> Lexer::KEYWORDS = {
    {"int", TokenType::DATA_TYPE},
    {"float", TokenType::DATA_TYPE},
    {"String", TokenType::DATA_TYPE},
    {"double", TokenType::DATA_TYPE},
    {"float", TokenType::DATA_TYPE},
    {"bool", TokenType::DATA_TYPE},
    {"if", TokenType::KEYWORD},
    {"else", TokenType::KEYWORD},
    {"for", TokenType::KEYWORD},
    {"while", TokenType::KEYWORD},
    {"return", TokenType::KEYWORD},
    {"start", TokenType::KEYWORD},
    {"end", TokenType::KEYWORD},
    {"print", TokenType::KEYWORD}
};

Lexer::Lexer(const std::string& sourceCode) : sourceCode(sourceCode) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    std::regex commentPattern(R"(/\*(.|[\r\n])*?\*/|//[^\n]*)");
    std::regex stringPattern(R"("(?:[^"\\]|\\.)*")");
    std::regex keywordPattern(R"(\b(?:start|end|func|print|if|else|for|while|return|and|or|not)\b)");
    std::regex datatypePattern(R"(\b(?:int|float|String|bool|double)\b)");
    std::regex identifierPattern(R"(\b[a-zA-Z_]\w*\b)");
    std::regex intLiteralPattern(R"(\b\d+\b)");
    std::regex operatorPattern(R"([+\-*/=<>!]+)");
    std::regex delimiterPattern(R"([()\[\]{};,.])");
    std::regex errorPattern(R"("(?:[^"\\]|\\.)*")");

    auto searchStart = sourceCode.cbegin();
    std::smatch match;

    while (searchStart != sourceCode.cend()) {
        if (std::isspace(*searchStart)) {
            ++searchStart;
            continue;
        }

        if (std::regex_search(searchStart, sourceCode.cend(), match, commentPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::COMMENT, match.str());
        }
        else if (std::regex_search(searchStart, sourceCode.cend(), match, stringPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::STRING_LITERAL, match.str());
        }
        else if (std::regex_search(searchStart, sourceCode.cend(), match, keywordPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::KEYWORD, match.str());
        }
        else if (std::regex_search(searchStart, sourceCode.cend(), match, datatypePattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::DATA_TYPE, match.str());
        }
        else if (std::regex_search(searchStart, sourceCode.cend(), match, intLiteralPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::INT_LITERAL, match.str());
        }
        else if (std::regex_search(searchStart, sourceCode.cend(), match, identifierPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::IDENTIFIER, match.str());
        }
        else if (std::regex_search(searchStart, sourceCode.cend(), match, operatorPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::OPERATOR, match.str());
        }
        else if (std::regex_search(searchStart, sourceCode.cend(), match, delimiterPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::DELIMITER, match.str());
        }
        else {
            tokens.emplace_back(TokenType::UNKNOWN, std::string(1, *searchStart));
            ++searchStart;
            continue;
        }
        searchStart = match.suffix().first;
    }

    tokens.emplace_back(TokenType::END_OF_FILE, "");

    return tokens;
}





