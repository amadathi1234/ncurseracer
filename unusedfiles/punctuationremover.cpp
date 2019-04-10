#include <iostream>
#include <fstream>
#include <cctype>

int main(int argc,char* argv[]) {
    std::ifstream streamer (argv[1]);
    std::string output = "";
    std::string next = "";
    while(!streamer.eof()) {
        streamer >> next;
        for(int i = 0; i < next.size(); i++) {
            if(!std::isalpha(next[i]) && !std::isdigit(next[i])) {
                next.erase(i, 1);
            }
        }
        output += next + "\n";
    }
    std::cout << output;
    return 0;
}
