#include "Lexer.hpp"

#include <iostream>
#include <vector>
#include <regex>

// Static constant unordered map of keywords and their corresponding token types
const std::unordered_map<std::string, TokenType> Lexer::KEYWORDS = {
    {"int", TokenType::DATA_TYPE},
    {"float", TokenType::DATA_TYPE},
    {"String", TokenType::DATA_TYPE},
    {"double", TokenType::DATA_TYPE},
    {"float", TokenType::DATA_TYPE},
    {"bool", TokenType::DATA_TYPE},
    {"char", TokenType::DATA_TYPE},
    {"true", TokenType::BOOLEAN_LITERAL},
    {"false", TokenType::BOOLEAN_LITERAL},
    {"if", TokenType::KEYWORD},
    {"else", TokenType::KEYWORD},
    {"while", TokenType::KEYWORD},
    {"in", TokenType::KEYWORD},
    {"out", TokenType::KEYWORD},
    {"class", TokenType::KEYWORD},
    {"Main", TokenType::KEYWORD}
};

// Constructor for Lexer class
Lexer::Lexer(const std::string& sourceCode) : sourceCode(sourceCode) {}

// Method to tokenize the source code
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    std::vector<std::string> errors;

    // Regular expressions for different token types
    std::regex commentPattern(R"(/\*(.|[\r\n])*?\*/|//[^\n]*)");
    std::regex stringPattern(R"("(?:[^"\\]|\\.)*")");
    std::regex keywordPattern(R"(\b(?:class|Main|in|out|if|else|while)\b)");
    std::regex datatypePattern(R"(\b(?:int|float|String|bool|double|char)\b)");
    std::regex identifierPattern(R"(\b[a-zA-Z_]\w*\b)");
    std::regex booleanPattern(R"(\b(?:true|false)\b)");
    std::regex intLiteralPattern(R"(\b\d+\b)");
    std::regex floatLiteralPattern(R"(\b\d+\.\d+\b)");
    std::regex operatorPattern(R"(<=|>=|==|[+\-*/=!<>])");
    std::regex delimiterPattern(R"([()\[\]{};:,.])");
    std::regex errorPattern(R"(^[_!@#%^&]\w*|^\d+[a-zA-Z_]\w*)");

    // Iterator to search through the source code
    auto searchStart = sourceCode.cbegin();
    // Match object to store the result of the regex search
    std::smatch match;

    // Line count for error reporting
    int lineCount = 1;
    // Loop through the source code
    while (searchStart != sourceCode.cend()) {
        if (*searchStart == '\n') {
            lineCount++;
            ++searchStart;
            continue;
        }
        
        // Skip whitespace characters
        if (std::isspace(*searchStart)) {
            ++searchStart;
            continue;
        }
        
        // Check for invalid identifiers
        if (std::regex_search(searchStart, sourceCode.cend(), match, errorPattern) && match.position() == 0) {
            errors.push_back("Error at line " + std::to_string(lineCount) + ": Invalid identifier '" + match.str() + "'. Identifiers cannot start with a special character or digits.");
            ++searchStart;
            continue;
        }

        // Check for comments
        if (std::regex_search(searchStart, sourceCode.cend(), match, commentPattern) && match.position() == 0) {
            // Do not emplace back comment pattern into tokens
        }
        // Check for string literals
        else if (std::regex_search(searchStart, sourceCode.cend(), match, stringPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::STRING_LITERAL, match.str(), lineCount);
        }
        // Check for keywords
        else if (std::regex_search(searchStart, sourceCode.cend(), match, keywordPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::KEYWORD, match.str(), lineCount);
        }
        // Check for data types
        else if (std::regex_search(searchStart, sourceCode.cend(), match, datatypePattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::DATA_TYPE, match.str(), lineCount);
        }
        // Check for float literals
        else if (std::regex_search(searchStart, sourceCode.cend(), match, floatLiteralPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::FLOAT_LITERAL, match.str(), lineCount);
        }
        // Check for integer literals
        else if (std::regex_search(searchStart, sourceCode.cend(), match, intLiteralPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::INT_LITERAL, match.str(), lineCount);
        }
        // Check for boolean literals
        else if (std::regex_search(searchStart, sourceCode.cend(), match, booleanPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::BOOLEAN_LITERAL, match.str(), lineCount);
        }
        // Check for identifiers
        else if (std::regex_search(searchStart, sourceCode.cend(), match, identifierPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::IDENTIFIER, match.str(), lineCount);
        }
        // Check for operators
        else if (std::regex_search(searchStart, sourceCode.cend(), match, operatorPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::OPERATOR, match.str(), lineCount);
        }
        // Check for delimiters
        else if (std::regex_search(searchStart, sourceCode.cend(), match, delimiterPattern) && match.position() == 0) {
            tokens.emplace_back(TokenType::DELIMITER, match.str(), lineCount);
        }
        // If no token matches, it's an unknown token
        else {
            errors.push_back("Error at line " + std::to_string(lineCount) + ": Unknown token '" + std::string(1, *searchStart) + "'");
            ++searchStart;
            continue;
        }

        // Update the search start to the end of the matched token
        std::string matchStr = match.str();
        lineCount += std::count(matchStr.begin(), matchStr.end(), '\n');
        searchStart = match.suffix().first;
    }

    // If there are any errors, print them and exit the program
    if (!errors.empty()) {
        std::cerr << "Found " << errors.size() << " error(s):" << std::endl;
        for (const auto& error : errors) {
            std::cerr << error << std::endl;
        }
        std::exit(EXIT_FAILURE);
    }

    // Add an END_OF_FILE token to the end of the tokens vector
    tokens.emplace_back(TokenType::END_OF_FILE, "", lineCount);
    return tokens;
}





