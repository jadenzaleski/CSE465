//
//  Interpreter.cpp
//  Homework6
//
//  Created by Jaden Zaleski on 5/8/24.
//

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <stdexcept>  // For exception handling

class Interpreter {
public:
    Interpreter(const std::string& filename) : filename(filename), line_number(0) {}
    
    void run() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file");
        }
        
        std::string line;
        while (std::getline(file, line)) {
            ++line_number;
            auto tokens = lexical_analysis(line);
            parse(tokens);
        }
    }
    
    void print_variables() const {
        for (const auto& [key, value] : variables) {
            std::cout << key << ": " << value << std::endl;
        }
    }
    
private:
    std::string filename;
    int line_number;
    std::map<std::string, std::string> variables;
    
    std::vector<std::pair<std::string, std::string>> lexical_analysis(const std::string& line) {
        std::vector<std::pair<std::string, std::string>> tokens;
        
        const std::vector<std::pair<std::string, std::string>> TOKEN_SPECIFICATION = {
            {"PRINT",       R"(PRINT\s[a-zA-Z_][a-zA-Z_0-9]+)"},
            {"STR_VAR",     R"([a-zA-Z_][a-zA-Z_0-9]+\s)"},
            {"INT_VAR",     R"([a-zA-Z_][a-zA-Z_0-9]+\s)"},
            {"ASSIGN",      R"((\?<=\s)\=(\?=\s))"},
            {"PLUS_ASSIGN", R"((\?<=\s)\+=(\?=\s))"},
            {"MINUS_ASSIGN",R"((\?<=\s)-=(\?=\s))"},
            {"MULT_ASSIGN", R"((\?<=\s)\*=(\?=\s))"},
            {"INT_VAR_VAL", R"((\?<=\+=|-=|\*=)\s[a-zA-Z_][a-zA-Z_0-9]*)"},
            {"STR_VAR_VAL", R"((\?<=\+=)\s[a-zA-Z_][a-zA-Z_0-9]*)"},
            {"NUMBER",      R"((\?<=\s)-?\d+(\?=\s))"},
            {"STRING",      R"("[^"]*")"},
            {"SEMICOLON",   R"((\?<=\s);)"},
            {"WS",          R"(\s+)"},
            {"NEWLN",       R"(\n)"},
        };
        
        // Loop through the token specification and apply each regex pattern
        for (const auto& spec : TOKEN_SPECIFICATION) {
            std::regex regex(spec.second);
            std::smatch match;
            std::string::const_iterator search_start(line.cbegin());
            
            // Use a while loop to find all matches in the line
            while (std::regex_search(search_start, line.cend(), match, regex)) {
                if (spec.first != "WS" && spec.first != "NEWLN") { // Skip unwanted tokens
                    tokens.emplace_back(spec.first, match.str()); // Store token type and value
                }
                // Move the search start to after the current match
                search_start += match.position() + match.length();
            }
        }
        
        return tokens;
    }
    
    void parse(const std::vector<std::pair<std::string, std::string>>& tokens) {
        auto it = tokens.cbegin();
        while (it != tokens.cend()) {
            auto token = *it;
            
            if (token.first == "PRINT") {
                std::string var_name = token.second.substr(6);  // Extract variable name after "PRINT "
                if (variables.find(var_name) == variables.end()) {
                    throw std::runtime_error("Undefined variable: " + var_name + " at line " + std::to_string(line_number));
                }
                std::cout << var_name << ": " << variables[var_name] << std::endl;
            } else if (token.first == "INT_VAR" || token.first == "STR_VAR") {
                std::string var_name = token.second;
                
                ++it; // Skip INT_VAR or STR_VAR
                if (it == tokens.cend()) break;
                std::string op_token = it->second;  // Get the operator
                
                ++it; // Move to value
                if (it == tokens.cend()) break;
                auto value_token = *it;
                
                ++it; // Move to semicolon
                if (it == tokens.cend()) break;
                std::string semicolon = it->second;  // Ensure semicolon
                
                if (semicolon != ";") {
                    std::cout << semicolon << std::endl;
                    
                    throw std::runtime_error("Missing semicolon at line " + std::to_string(line_number));
                }
                
                std::string value_str;
                if (value_token.first == "NUMBER") {
                    value_str = value_token.second;
                } else if (value_token.first == "STRING") {
                    value_str = value_token.second;
                } else {
                    value_str = variables[value_token.second]; // Variable reference
                }
                
                try {
                    if (op_token == "=") {
                        variables[var_name] = value_str;
                    } else if (op_token == "+=") {
                        variables[var_name] += value_str;
                    } else if (op_token == "-=") {
                        int var_val = std::stoi(variables[var_name]);
                        int value = std::stoi(value_str);
                        var_val -= value;
                        variables[var_name] = std::to_string(var_val);
                    } else if (op_token == "*=") {
                        int var_val = std::stoi(variables[var_name]);
                        int value = std::stoi(value_str);
                        var_val *= value;
                        variables[var_name] = std::to_string(var_val);
                    }
                } catch (std::exception& ex) {
                    throw std::runtime_error("Error processing token at line " + std::to_string(line_number) + ": " + ex.what());
                }
            }
            
            ++it; // Move to the next token
        }
    }
};

//int main(int argc, char* argv[]) {
//    if (argc < 2) {
//        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
//        return 1;
//    }
//    
//    std::string filename = argv[1];
//    try {
//        Interpreter interpreter(filename);
//        interpreter.run();
//        interpreter.print_variables(); // Output the variable state at the end
//    } catch (std::runtime_error& e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//        return 1;
//    }
//    
//    return 0;
//}
