#include "filehandling.h"

std::string file::read(const char* file) {

    std::ifstream inputFile(file);
    std::stringstream fileContents;

    if (inputFile.is_open()) {

        fileContents << inputFile.rdbuf();
    }

    else {

        std::cout << "error reading file: " << file << std::endl;
    }

    inputFile.close();

    return fileContents.str();
}
