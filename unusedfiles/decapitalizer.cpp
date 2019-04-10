#include <iostream>
#include <fstream>

int main() {
    std::ifstream streamer ("wordsEasy.txt");
    std::string output = "";
    std::string next = "";
    while(!streamer.eof()) {
        streamer >> next;
        for(int i = 0; i < next.size(); i++) {
            next[i] = tolower(next[i]);
        }
        output += next + "\n";
    }
    std::cout << output;
    return 0;
}
