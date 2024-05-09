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
            if (line.size() == 3 && line[0].type == TokenType::Print) {
                // Handle PRINT statements
                const auto& variableName = line[1].value;
                if (variables.find(variableName) != variables.end()) {
                    printVariable(variableName);
                } else {
                    std::cerr << "Variable not found: " << variableName << std::endl;
                }
            } else if (line.size() == 4 && (line[1].type == TokenType::Assign ||
                                            line[1].type == TokenType::PlusAssign ||
                                            line[1].type == TokenType::MinusAssign ||
                                            line[1].type == TokenType::MultAssign)) {
                // Handle assignment and compound assignment
                const std::string varName = line[0].value;
                const auto& operatorToken = line[1];
                const auto& valueToken = line[2];
                
                if (operatorToken.type == TokenType::Assign) {
                    if (valueToken.type == TokenType::Number && isNumeric(valueToken.value)) {
                        // Assign a number to a variable
                        int newValue = std::stoi(valueToken.value);
                        variables[varName] = newValue;
                    } else if (valueToken.type == TokenType::String) {
                        // Assign a string to a variable
                        variables[varName] = valueToken.value;
                    } else if (valueToken.type == TokenType::Identifier) {
                        // Check if the identifier refers to an existing variable
                        if (variables.find(valueToken.value) != variables.end()) {
                            const VariableValue& sourceValue = variables.at(valueToken.value);
                            variables[varName] = sourceValue; // Assign the source variable's value
                        } else {
                            std::cerr << "Error: Variable '" << valueToken.value << "' not found." << std::endl;
                        }
                    } else {
                        std::cerr << "Error: Invalid assignment value." << std::endl;
                    }
                } else if (operatorToken.type == TokenType::PlusAssign) {
                    if (variables.find(varName) != variables.end()) {
                        if (std::holds_alternative<int>(variables[varName])) {
                            int existingValue = std::get<int>(variables[varName]);
                            
                            if (isNumeric(valueToken.value)) {
                                int additionValue = std::stoi(valueToken.value);
                                variables[varName] = existingValue + additionValue;
                            } else if (variables.find(valueToken.value) != variables.end() && std::holds_alternative<int>(variables[valueToken.value])) {
                                int additionValue = std::get<int>(variables[valueToken.value]);
                                variables[varName] = existingValue + additionValue;
                            } else {
                                std::cerr << "Invalid addition value: " << valueToken.value << std::endl;
                            }
                        } else if (std::holds_alternative<std::string>(variables[varName])) {
                            // String concatenation
                            std::string existingValue = std::get<std::string>(variables[varName]);
                            
                            if (valueToken.type == TokenType::Identifier) {
                                if (variables.find(valueToken.value) != variables.end() && std::holds_alternative<std::string>(variables[valueToken.value])) {
                                    // If the addition value is a valid existing string variable
                                    std::string additionValue = std::get<std::string>(variables[valueToken.value]);
                                    variables[varName] = existingValue + additionValue;
                                } else {
                                    // If the variable is not found or it's not a string
                                    std::cerr << "Error: Cannot concatenate non-string or non-existent variable '" << valueToken.value << "' to string '" << varName << "'." << std::endl;
                                }
                            } else if (valueToken.type == TokenType::String) {
                                // Concatenation with a string literal
                                std::string newValue = valueToken.value;
                                variables[varName] = existingValue + newValue;
                            } else {
                                // If the addition value is not a valid string
                                std::cerr << "Error: Invalid addition value for string concatenation." << std::endl;
                            }
                        }
                    } else {
                        std::cerr << "Variable not found: " << varName << std::endl;
                    }
                } else if (operatorToken.type == TokenType::MinusAssign) {
                    // Check if the variable is an integer
                    if (std::holds_alternative<int>(variables[varName])) {
                        int existingValue = std::get<int>(variables[varName]);
                        
                        if (valueToken.type == TokenType::Number && isNumeric(valueToken.value)) {
                            // Subtract a numeric literal
                            int subtractionValue = std::stoi(valueToken.value);
                            variables[varName] = existingValue - subtractionValue;
                        } else if (valueToken.type == TokenType::Identifier) {
                            if (variables.find(valueToken.value) != variables.end() && std::holds_alternative<int>(variables[valueToken.value])) {
                                // Subtraction with another integer variable
                                int subtractionValue = std::get<int>(variables[valueToken.value]);
                                variables[varName] = existingValue - subtractionValue;
                            } else {
                                std::cerr << "Error: Invalid variable for subtraction." << std::endl;
                            }
                        } else {
                            std::cerr << "Error: Invalid value for subtraction." << std::endl;
                        }
                    } else {
                        std::cerr << "Error: Variable '" << varName << "' is not an integer." << std::endl;
                    }
                } else if (operatorToken.type == TokenType::MultAssign) {
                    // Check if the variable is an integer
                    if (std::holds_alternative<int>(variables[varName])) {
                        int existingValue = std::get<int>(variables[varName]);
                        
                        if (valueToken.type == TokenType::Number && isNumeric(valueToken.value)) {
                            // Subtract a numeric literal
                            int multiplicationValue = std::stoi(valueToken.value);
                            variables[varName] = existingValue * multiplicationValue;
                        } else if (valueToken.type == TokenType::Identifier) {
                            if (variables.find(valueToken.value) != variables.end() && std::holds_alternative<int>(variables[valueToken.value])) {
                                // Subtraction with another integer variable
                                int multiplicationValue = std::get<int>(variables[valueToken.value]);
                                variables[varName] = existingValue * multiplicationValue;
                            } else {
                                std::cerr << "Error: Invalid variable for multiplication." << std::endl;
                            }
                        } else {
                            std::cerr << "Error: Invalid value for multiplication." << std::endl;
                        }
                    } else {
                        std::cerr << "Error: Variable '" << varName << "' is not an integer." << std::endl;
                    }
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
    
    // Function to print the variable's value
    void printVariable(const std::string& variableName) {
        const auto& variableValue = variables.at(variableName);
        if (std::holds_alternative<int>(variableValue)) {
            std::cout << variableName << "=" << std::get<int>(variableValue) << std::endl;
        } else if (std::holds_alternative<std::string>(variableValue)) {
            std::cout << variableName << "=\"" << std::get<std::string>(variableValue) << "\"" << std::endl;
        }
    }
    
    // Function to check if a string is numeric
    bool isNumeric(const std::string& str) {
        if (str.empty()) return false;
        
        size_t start = 0;
        if (str[0] == '-' || str[0] == '+') {
            start = 1; // Account for negative/positive signs
        }
        
        for (size_t i = start; i < str.size(); i++) {
            if (!std::isdigit(str[i])) {
                return false; // If any non-digit is found, it's not numeric
            }
        }
        
        return true;
    }
};
