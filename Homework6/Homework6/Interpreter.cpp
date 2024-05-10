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
#include <fstream>

using VariableValue = std::variant<int, std::string>;

class Interpreter {
public:
    Interpreter(const std::vector<Token>& tokens, const std::string& filePath) : tokens(tokens), filePath(filePath), structuredTokens(structureTokens(tokens)), currentIndex(0) {}
    
    void run() {
        size_t loopCount = 0;
        size_t i = 0;
        // loop through all the tokens
        while (i < structuredTokens.size()) {
            const auto& line = structuredTokens[i];
            // if its a print
            if (line.size() == 3 && line[0].type == TokenType::Print) {
                handlePrint(line);
                // if its any kind of assign
            } else if (line.size() == 4 && (line[1].type == TokenType::Assign ||
                                            line[1].type == TokenType::PlusAssign ||
                                            line[1].type == TokenType::MinusAssign ||
                                            line[1].type == TokenType::MultAssign)) {
                handleAssignment(line);
                // if its a For
            } else if (line.size() == 2 && line[0].type == TokenType::For) {
                // Capture the FOR loop's starting point and loop count
                if (structuredTokens[i][1].type == TokenType::Number) {
                    // If loop count is a number
                    loopCount = std::stoi(structuredTokens[i][1].value);
                } else if (structuredTokens[i][1].type == TokenType::Identifier) {
                    const std::string& varName = structuredTokens[i][1].value;
                    // get the number of times to loop
                    if (variables.find(varName) != variables.end() && std::holds_alternative<int>(variables[varName])) {
                        loopCount = std::get<int>(variables[varName]); // Retrieve the loop count from a variable
                    } else {
                        //                        std::cerr << "Error: Loop count variable '" << varName << "' not found or is not an integer." << std::endl;
                        error(line);
                        break;
                    }
                }
                // loop body start after the For # and ends at EndFor.
                size_t startLoop = i + 1; // The loop body starts after "FOR" and the loop count
                size_t endLoop = findEndFor(structuredTokens, startLoop);
                // all the code that is being looped
                std::vector<std::vector<Token>> loopBody(structuredTokens.begin() + startLoop, structuredTokens.begin() + endLoop);
                executeLoop(loopBody, loopCount); // Run the loop
                i += loopBody.size(); // Skip over the loop content
            } 
            ++i; // Move to the next line
        }
    }
    
    
private:
    const std::vector<Token>& tokens;
    size_t currentIndex;
    std::unordered_map<std::string, VariableValue> variables;
    std::vector<std::vector<Token>> structuredTokens;
    std::string filePath;
    // Print the erros
    // It is easier just to search for the line in the file then to backtrace the entire
    // program to get the line number.
    void error(const std::vector<Token>& line) {
        std::string stringLine;
        // make the string
        for (const auto& token : line) {
            if (token.type == TokenType::String) {
                stringLine += "\"" + token.value + "\"";
            } else {
                stringLine += token.value;
            }
            stringLine += " "; // Add a space between tokens for readability
        }
        
        stringLine.erase(stringLine.end() - 1);
        std::ifstream file(filePath);
        // String to store each line of the file.
        std::string currentLine;
        // open the file and search for the string
        int i = 0;
        if (file.is_open()) {
            while (getline(file, currentLine)) {
                ++i;
                if (currentLine == stringLine) {
                    std::cout << "RUNTIME ERROR: line " << i << std::endl;
                    exit(1);
                }
            }
            
            file.close();
        }
        // there isnt a match but there is an error
        std::cout << "RUNTIME ERROR: line unknown " << std::endl;
        exit(1);
    }
    // find the EndFor
    size_t findEndFor(const std::vector<std::vector<Token>>& structuredTokens, size_t start) {
        for (size_t i = start; i < structuredTokens.size(); i++) {
            if (structuredTokens[i][0].type == TokenType::EndFor) {
                return i; // Return the index of "ENDFOR"
            }
        }
        return structuredTokens.size(); // If "ENDFOR" is not found, return the end of the vector
    }
    
    // Function to handle assignments
    void handleAssignment(const std::vector<Token>& line) {
        const std::string varName = line[0].value;
        const auto& operatorToken = line[1];
        const auto& valueToken = line[2];
        // =
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
                    //std::cerr << "Error: Variable '" << valueToken.value << "' not found." << std::endl;
                    error(line);
                }
            } else {
                //                std::cerr << "Error: Invalid assignment value." << std::endl;
                error(line);
            }
            // +=
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
                        //                        std::cerr << "Invalid addition value: " << valueToken.value << std::endl;
                        error(line);
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
                            //                            std::cerr << "Error: Cannot concatenate non-string or non-existent variable '" << valueToken.value << "' to string '" << varName << "'." << std::endl;
                            error(line);
                        }
                    } else if (valueToken.type == TokenType::String) {
                        // Concatenation with a string literal
                        std::string newValue = valueToken.value;
                        variables[varName] = existingValue + newValue;
                    } else {
                        // If the addition value is not a valid string
                        //                        std::cerr << "Error: Invalid addition value for string concatenation." << std::endl;
                        error(line);
                    }
                }
            } else {
                //                std::cerr << "Variable not found: " << varName << std::endl;
                error(line);
            }
            // -=
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
                        //                        std::cerr << "Error: Invalid variable for subtraction." << std::endl;
                        error(line);
                    }
                } else {
                    //                    std::cerr << "Error: Invalid value for subtraction." << std::endl;
                    error(line);
                }
            } else {
                //                std::cerr << "Error: Variable '" << varName << "' is not an integer." << std::endl;
                error(line);
            }
            // *=
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
                        //                        std::cerr << "Error: Invalid variable for multiplication." << std::endl;
                        error(line);
                    }
                } else {
                    //                    std::cerr << "Error: Invalid value for multiplication." << std::endl;
                    error(line);
                }
            } else {
                //                std::cerr << "Error: Variable '" << varName << "' is not an integer." << std::endl;
                error(line);
            }
        }
    }
    
    // Function to handle PRINT operations
    void handlePrint(const std::vector<Token>& line) {
        if (line[1].type == TokenType::Identifier) {
            const auto& variableName = line[1].value;
            if (variables.find(variableName) != variables.end()) {
                const auto& variableValue = variables.at(variableName);
                if (std::holds_alternative<int>(variableValue)) {
                    std::cout << variableName << "=" << std::get<int>(variableValue) << std::endl;
                } else if (std::holds_alternative<std::string>(variableValue)) {
                    std::cout << variableName << "=\"" << std::get<std::string>(variableValue) << "\"" << std::endl;
                }
            } else {
                //                std::cerr << "Variable not found: " << variableName << std::endl;
                error(line);
            }
        }
    }
    
    // Function to execute a FOR loop
    void executeLoop(const std::vector<std::vector<Token>>& loopBody, size_t loopCount) {
        for (size_t j = 0; j < loopCount; ++j) {
            for (const auto& line : loopBody) {
                // Execute the loop body according to its structure
                if (line.size() == 4 && (line[1].type == TokenType::Assign ||
                                         line[1].type == TokenType::PlusAssign ||
                                         line[1].type == TokenType::MinusAssign ||
                                         line[1].type == TokenType::MultAssign)) {
                    handleAssignment(line);
                } else if (line.size() == 3 && line[0].type == TokenType::Print) {
                    handlePrint(line);
                }
            }
        }
    }
    // create a list of tokens where each array of tokens is a line
    std::vector<std::vector<Token>> structureTokens(const std::vector<Token>& tokens) {
        std::vector<std::vector<Token>> linedTokens; // 2D vector
        std::vector<Token> currentLine; // Temporary vector to hold current line of tokens
        
        for (size_t i = 0; i < tokens.size(); i++) {
            const auto& token = tokens[i];
            if (token.type == TokenType::Semicolon || token.type == TokenType::EndFor) {
                // Add the current line to the list of lines
                currentLine.push_back(token); // Add the token to the end of the line
                linedTokens.push_back(currentLine); // Store the current line
                currentLine.clear(); // Clear the current line for next usage
            } else if (token.type == TokenType::For) {
                // Add the current line to the list of lines
                currentLine.push_back(token); // Add the token to the end of the line
                currentLine.push_back(tokens[i + 1]); // the number of for
                linedTokens.push_back(currentLine); // Store the current line
                currentLine.clear(); // Clear the current line for next usage
                ++i;
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
