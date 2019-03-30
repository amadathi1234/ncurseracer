#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "ncurseracer.hpp"
#include <iostream>
#include <random>

WordParser::WordParser(std::string filePath) {
    std::ifstream charstream(filePath);
    std::string word;
    while(getline(charstream, word)) {
        words.push_back(word);    
    }
}

std::string WordParser::getString(int wordCount) {
    std::string str = "";
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, (int)words.size() - 1);
    for(int i = 0; i < wordCount; i++) {
        std::string word = words[ distribution(generator) ];
        str += (str.size() ? " " : "") + word;
    }
    return str;
}
