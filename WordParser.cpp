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

// TODO: fix spacing
std::string WordParser::getString(int size) {
    std::string str = "";
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, (int)words.size() - 1);
    while((int)str.size() < size) {
        std::string word = words[ distribution(generator) ];
        if((int)str.size() + (int)word.size() < size) str += word + " ";
        else break;
    }
    return str;
}
