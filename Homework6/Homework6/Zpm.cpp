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
    
    // Create an instance of the Interpreter class
    Interpreter interpreter(filePath);
    
    // Run the interpreter to process the file
    try {
        interpreter.run();
        // Print the variables after processing
        interpreter.print_variables();
    } catch (std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1; // Return with an error status
    }
    
    file.close(); // Close the file when done
    return 0;     // Successful execution
}

