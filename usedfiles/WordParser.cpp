#include <vector>
#include <string>
#include <fstream>
#include "ncurseracer.hpp"
#include <random>
#include <ctime>

/**
 * This class is used to parse files containing words used for typing in the game.
 */

/**
 * Creates the vector of words taken from the file in the specified filepath.
 */
WordParser::WordParser(std::string filePath) {
    std::ifstream charstream(filePath);
    std::string word;
    while(getline(charstream, word)) {
        words.push_back(word);    
    }
}

/**
 * Get a string containing the words required, space-separated
 */
std::string WordParser::getString(int wordCount) {
    std::string str = "";
    std::mt19937 generator(difftime(time(NULL), 0));
    std::uniform_int_distribution<int> distribution(0, (int)words.size() - 1);
    for(int i = 0; i < wordCount; i++) {
        std::string word = words[ distribution(generator) ];
        str += (str.size() ? " " : "") + word;
    }
    return str;
}
