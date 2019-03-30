#include <fstream>
#include <random>
#include <iostream>

int main() {
    std::ifstream streamer ("wordsMedium.txt");
    std::string newString = "";
    std::string nextWord = "";
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0, 2);
    while(!streamer.eof()) {
        streamer >> nextWord;
        if(distribution(generator) > 1) nextWord[0] = toupper(nextWord[0]);
        newString += nextWord + "\n";
    }
    std::cout << newString;
    return 0;
}
