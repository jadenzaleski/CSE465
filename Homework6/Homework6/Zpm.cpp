//
//  Zpm.cpp
//  Homework6
//
//  Created by Jaden Zaleski on 4/26/24.
//
// => I’m competing for BONUS Points <=

#include <iostream>
#include <fstream>
#include <string>
#include "Interpreter.cpp"

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "";
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
    
    file.close();
    return content;
}

int main(int argc, const char *argv[]) {
    // AI helped with argument and file handling
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>\n";
        return 1; // Return with an error status
    }
    
    std::string filePath = argv[1];
    std::ifstream file(filePath); // Open the specified file for reading
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << "\n";
        return 1; // Return with an error status
    } else {
        std::cout << "Using: " << filePath << std::endl;
    }
    
    std::string sourceCode = readFile(filePath);
    
    Lexer lexer(sourceCode);
    auto tokens = lexer.tokenize();
    
    lexer.printTokens(tokens);
    
    Interpreter interpreter(tokens);
    interpreter.run();
    
    file.close(); // Close the file when done
    return 0;     // Successful execution
}

