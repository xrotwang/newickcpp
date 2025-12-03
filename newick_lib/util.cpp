#include <fstream>
#include <iostream>
#include <string>
#include <vector>


/*
 * Read file into vector of characters.
 */
std::vector<char> read_file(const std::string &filename) {
    std::ifstream file;

    file.open(filename);
    file.unsetf(std::ios_base::skipws);

    char inputChar;
    std::vector<char> vec;

    while (file.good()) {
        file.get(inputChar);
        vec.push_back(inputChar);
    }
    return vec;
}
