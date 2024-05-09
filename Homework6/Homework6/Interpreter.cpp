//
//  Interpreter.cpp
//  Homework6
//
//  Created by Jaden Zaleski on 5/8/24.
//

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <variant>
#include "Lexer.cpp"

using VariableValue = std::variant<int, std::string>;

class Interpreter {
public:
    Interpreter(const std::vector<Token>& tokens) : tokens(tokens), currentIndex(0) {}
    
    void run() {
        structureTokens();
    }
    
private:
    const std::vector<Token>& tokens;
    size_t currentIndex;
    std::unordered_map<std::string, VariableValue> variables;
    
    
    void structureTokens() {
        // We want to create a list of tokens for each statement (line)
        // 2D vector array
        std::vector<std::vector<Token>> linedTokens;
        int y = 0;
        for (int i = 0; i < tokens.size(); i++) {
            if (tokens[i].type != TokenType::Semicolon) {
                linedTokens[y].push_back(tokens[i]);
            } else {
                linedTokens[y].push_back(tokens[i]);
                y++;
            }
        }
        
    }
    
    void parseAssignment() {
        std::string varName = tokens[currentIndex].value;
        currentIndex++; // Move past the identifier
        
        if (tokens[currentIndex].value == "=") {
            currentIndex++; // Move past '='
            int value = parseExpression(); // This could be more complex for full expressions
            variables[varName] = value;
            std::cout << varName << " = " << value << std::endl;
        }
    }
    
    int parseExpression() {
        // In a simple case, parse a single number. A more complex implementation
        // would handle arithmetic and precedence.
        const auto& token = tokens[currentIndex];
        if (token.type == TokenType::Number) {
            int value = std::stoi(token.value);
            currentIndex++;
            return value;
        }
        
        std::cerr << "Expected a number, got: " << token.value << std::endl;
        return 0;
    }
};
