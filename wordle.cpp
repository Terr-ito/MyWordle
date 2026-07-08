#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <chrono>

// Struct for a wordle type word
// type = 1 = black ---- letter does not appear
// type = 2 = yellow --- letter appears but in a different location
// type = 3 = green ---- letter appears in that location
typedef struct {
    char a;
    unsigned type;
}wordle;

int main(int argc, char** argv) {
    // Open wordle words text file
    std::ifstream wordDB("valid-wordle-words.txt");
    if(!wordDB.is_open()) {
        std::cerr << "Error opening file.";
        return 1;
    }

    // Generate seed using clock
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);

    // Declare all variables and store database
    wordle guess[6];
    std::string word, text_line;
    std::vector<std::string> database;
    while (std::getline(wordDB, text_line))
    {
        database.push_back(text_line);
    }
    std::uniform_int_distribution<> distr(0, database.size() - 1);

    // Store random word and check if it's valid
    word = database[distr(gen)];
    if(word.length() != 5) {
        std::cerr << "Word pulled from database in not a valid word. (not 5 character word)";
        return 1;
    }

    std::cout << word;


    return 0;
}