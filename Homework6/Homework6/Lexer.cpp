//
//  Lexer.cpp
//  Homework6
//
//  Created by Jaden Zaleski on 5/8/24.
//

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>

enum class TokenType {
    Identifier,
    Number,
    String,
    Assign,
    PlusAssign,
    MinusAssign,
    MultAssign,
    Semicolon,
    Print,
    For,
    EndFor,
    EndOfFile
};

struct Token {
    TokenType type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& source) : source(source), index(0) {}
    
    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (index < source.length()) {
            char ch = source[index];
            if (std::isspace(ch)) {
                index++;
                continue;
            }
            
            if (std::isalpha(ch) || ch == '_') { // Identifiers
                tokens.push_back(parseIdentifier());
            } else if (std::isdigit(ch) || (ch == '-' && isNextDigit())) { // Numbers
                tokens.push_back(parseNumber());
            } else if (ch == '"') { // Strings
                tokens.push_back(parseString());
            } else if (std::string("+-*=").find(ch) != std::string::npos) { // Operators and compound operators
                auto operatorToken = parseOperator();
                if (operatorToken.type != TokenType::EndOfFile) {
                    tokens.push_back(operatorToken);
                } else {
                    std::cerr << "Unexpected operator: " << source[index] << std::endl;
                    index++;
                }
            } else if (ch == ';') {
                tokens.push_back({ TokenType::Semicolon, ";"});
                index++;
            } else {
                std::cerr << "Unexpected character: " << ch << std::endl;
                index++;
            }
        }
        
        tokens.push_back({ TokenType::EndOfFile, "" });
        return tokens;
    }
    
    void printTokens(const std::vector<Token>& tokens) {
        for (const auto& token : tokens) {
            std::cout << "Type: ";
            switch (token.type) {
                case TokenType::Identifier:
                    std::cout << "Identifier";
                    break;
                case TokenType::For:
                    std::cout << "For";
                    break;
                case TokenType::EndFor:
                    std::cout << "EndFor";
                    break;
                case TokenType::Number:
                    std::cout << "Number";
                    break;
                case TokenType::Assign:
                    std::cout << "Assign";
                    break;
                case TokenType::MultAssign:
                    std::cout << "MultAssign";
                    break;
                case TokenType::PlusAssign:
                    std::cout << "PlusAssign";
                    break;
                case TokenType::MinusAssign:
                    std::cout << "MinusAssign";
                    break;
                case TokenType::String:
                    std::cout << "String";
                    break;
                case TokenType::Semicolon:
                    std::cout << "Semicolon";
                    break;
                case TokenType::EndOfFile:
                    std::cout << "EndOfFile";
                    break;
                case TokenType::Print:
                    std::cout << "Print";
                    break;
                default:
                    std::cout << "Unknown";
            }
            std::cout << " --> " << token.value << std::endl;
        }
    }
    
private:
    std::string source;
    size_t index;
    
    bool isNextDigit() {
        return (index + 1 < source.length() && std::isdigit(source[index + 1]));
    }
    
    Token parseIdentifier() {
        std::ostringstream oss;
        while (index < source.length() && (std::isalnum(source[index]) || source[index] == '_')) {
            oss << source[index++];
        }
    
        std::string identifier = oss.str();
        
        if (identifier == "PRINT") {
            return { TokenType::Print, identifier };
        } else if (identifier == "FOR") {
            return { TokenType::For, "FOR" };
        } else if (identifier == "ENDFOR") {
            return { TokenType::EndFor, "ENDFOR" };
        }
        
        return { TokenType::Identifier, identifier };

    }
    
    Token parseNumber() {
        std::ostringstream oss;
        if (source[index] == '-') {
            oss << source[index++]; // Add the negative sign
        }
        while (index < source.length() && std::isdigit(source[index])) {
            oss << source[index++];
        }
        return { TokenType::Number, oss.str() };
    }
    
    Token parseString() {
        std::ostringstream oss;
        index++; // Move past the opening double quote
        while (index < source.length() && source[index] != '"') {
            oss << source[index++];
        }
        index++; // Move past the closing double quote
        return { TokenType::String, oss.str() };
    }
    
    Token parseOperator() {
        if (index >= source.length()) {
            return { TokenType::EndOfFile, "" };
        }
        
        std::string op = std::string(1, source[index]);
        if (op == "+" || op == "-" || op == "*") {
            // Check if there's a compound operator (like +=, -=, *=)
            if (index + 1 < source.length() && source[index + 1] == '=') {
                op += "="; // Now it's a compound operator
                index += 2; // Move past the operator
                if (op == "+=") {
                    return { TokenType::PlusAssign, op };
                } else if (op == "-=") {
                    return { TokenType::MinusAssign, op };
                } else {
                    return { TokenType::MultAssign, op };
                }
            }
        }
        
        // If not a compound operator, it's a regular operator
        index++;
        return { TokenType::Assign, op };
    }
};
