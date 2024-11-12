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
    {"in", TokenType::KEYWORD},
    {"out", TokenType::KEYWORD},
    {"class", TokenType::KEYWORD},
    {"Main", TokenType::KEYWORD}
};

Lexer::Lexer(const std::string& sourceCode) : sourceCode(sourceCode) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    std::vector<std::string> errors;

    std::regex commentPattern(R"(/\*(.|[\r\n])*?\*/|//[^\n]*)");
    std::regex stringPattern(R"("(?:[^"\\]|\\.)*")");
    std::regex keywordPattern(R"(\b(?:class|Main|in|out|if|else|for|while)\b)");
    std::regex datatypePattern(R"(\b(?:int|float|String|bool|double)\b)");
    std::regex identifierPattern(R"(\b[a-zA-Z_]\w*\b)");
    std::regex intLiteralPattern(R"(\b\d+\b)");
    std::regex operatorPattern(R"([+\-*/=<>!]|and|or|not)");
    std::regex delimiterPattern(R"([()\[\]{};:,.])");
    std::regex errorPattern(R"(^[_!@#%^&*]\w*|^\d+[a-zA-Z_]\w*)");

    auto searchStart = sourceCode.cbegin();
    std::smatch match;

    int lineCount = 1;
    while (searchStart != sourceCode.cend()) {
        if (*searchStart == '\n') {
            lineCount++;
            ++searchStart;
            continue;
        }
        
        if (std::isspace(*searchStart)) {
            ++searchStart;
            continue;
        }
        
        if (std::regex_search(searchStart, sourceCode.cend(), match, errorPattern) && match.position() == 0) {
            errors.push_back("Error at line " + std::to_string(lineCount) + ": Invalid identifier '" + match.str() + "'. Identifiers cannot start with a special character or digits.");
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
            errors.push_back("Error at line " + std::to_string(lineCount) + ": Unknown token '" + std::string(1, *searchStart) + "'");
            ++searchStart;
            continue;
        }

        std::string matchStr = match.str();
        lineCount += std::count(matchStr.begin(), matchStr.end(), '\n');
        searchStart = match.suffix().first;
    }

    if (!errors.empty()) {
        std::cerr << "Found " << errors.size() << " error(s):" << std::endl;
        for (const auto& error : errors) {
            std::cerr << error << std::endl;
        }
        std::exit(EXIT_FAILURE);
    }

    tokens.emplace_back(TokenType::END_OF_FILE, "");
    return tokens;
}





