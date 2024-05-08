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
    
    std::string line;
    while (std::getline(file, line)) { // Read the file line by line
        std::cout << line << "\n";    // Output the line to the console
    }
    
    file.close(); // Close the file when done
    return 0;     // Successful execution
}

