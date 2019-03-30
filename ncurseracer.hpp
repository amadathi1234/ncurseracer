#include <string>
#include <vector>

class WordParser {
    public:
        std::string getString(int size);
        WordParser(std::string filePath);
    private:
        std::vector<std::string> words;       
};

