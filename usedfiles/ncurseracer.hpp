/**
 * The header file used to define elements outside the driver class
 */
#include <string>
#include <vector>


class WordParser {
    public:
        std::string getString(int wordCount);
        // Creates the vector of words taken from the file given during construction..
        WordParser(std::string filePath);
        // Constructs the WordParser and initializes words based on the given filepath.
    private:
        std::vector<std::string> words; // this vector stores words from the given file in vector form
};

