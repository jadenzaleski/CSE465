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
    Interpreter(const std::vector<Token>& tokens) : tokens(tokens), structuredTokens(structureTokens(unrollForLoops(tokens))), currentIndex(0) {}
    
    void run() {
        for (const auto& line : structuredTokens) {
            for (currentIndex = 0; currentIndex < line.size(); currentIndex++) {
                const auto& token = line[currentIndex];
                
                switch (token.type) {
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
            }
            
        }
    }
    
private:
    const std::vector<Token>& tokens;
    size_t currentIndex;
    std::unordered_map<std::string, VariableValue> variables;
    std::vector<std::vector<Token>> structuredTokens;
    
    std::vector<std::vector<Token>> structureTokens(const std::vector<Token>& tokens) {
        std::vector<std::vector<Token>> linedTokens; // 2D vector
        std::vector<Token> currentLine; // Temporary vector to hold current line of tokens
        
        for (const auto& token : tokens) {
            if (token.type == TokenType::Semicolon) {
                // Add the current line to the list of lines
                currentLine.push_back(token); // Add the semicolon to the end of the line
                linedTokens.push_back(currentLine); // Store the current line
                currentLine.clear(); // Clear the current line for next usage
            } else {
                // Add the token to the current line
                currentLine.push_back(token);
            }
        }
        
        // If there's any remaining line without a semicolon, add it to the list
        if (!currentLine.empty()) {
            linedTokens.push_back(currentLine);
        }
        
        return linedTokens;
    }
    
    std::vector<Token> unrollForLoops(const std::vector<Token>& tokens) {
        std::vector<Token> unrolledTokens;
        std::vector<Token> loopBody;
        bool inLoop = false;
        int loopCount = 0;
        
        for (size_t i = 0; i < tokens.size(); i++) {
            const auto& token = tokens[i];
            
            if (token.type == TokenType::For) {
                if (i + 1 < tokens.size() && tokens[i + 1].type == TokenType::Number) {
                    loopCount = std::stoi(tokens[i + 1].value); // Get the loop count
                    inLoop = true; // Start collecting the loop body
                    i++; // Skip the loop count token
                }
            } else if (inLoop && token.type == TokenType::EndFor) {
                // If we've reached the end of the loop, duplicate the loop body
                for (int j = 0; j < loopCount; j++) {
                    unrolledTokens.insert(unrolledTokens.end(), loopBody.begin(), loopBody.end());
                }
                loopBody.clear();
                inLoop = false; // Exit loop collection
            } else if (inLoop) {
                // If in a loop, add tokens to the loop body
                loopBody.push_back(token);
            } else {
                // If not in a loop, add tokens normally
                unrolledTokens.push_back(token);
            }
        }
        
        return unrolledTokens;
    }
};
