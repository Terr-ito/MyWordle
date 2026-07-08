#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <chrono>

// Struct for a wordle type word
// type = 0 = grey ----- letter has not been checked
// type = 1 = black ---- letter does not appear
// type = 2 = yellow --- letter appears but in a different location
// type = 3 = green ---- letter appears in that location
typedef struct {
    std::string word;
    unsigned type[5];
}wordle;

void showKeyboard(unsigned letters[]) {
    for(int i = 0; i < 26; i++) {
        std::cout << char('a' + i) << " - ";

        // Translate unsigned type to color
        if(letters[i] == 0) std::cout << "grey";
        else if(letters[i] == 1) std::cout << "black";
        else if(letters[i] == 2) std::cout << "yellow";
        else std::cout << "green";

        std::cout << " | ";
        if(i % 4 == 3) std::cout << "\n";
    }
    std::cout << "\n";
}

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
    unsigned letters[26] = {0};
    wordle guess[6];
    std::string word, text_line;
    std::vector<std::string> database;
    while(std::getline(wordDB, text_line))
    {
        database.push_back(text_line);
    }
    std::uniform_int_distribution<> distr(0, database.size() - 1);

    // Store random word and check if it's valid
    word = database[distr(gen)];
    if(word.length() != 5) {
        std::cerr << "Word pulled from database in not a valid word. (not 5 letter word)";
        return 1;
    }

    std::cout << word << "\n";
    // Start guessing loop (i = guess count)
    unsigned i = 0;
    while(i < 6) {
        // Get guessed word from stdin and check if it's valid
        int valid = 0;
        while(valid == 0) {
            getline(std::cin, guess[i].word);
            if(guess[i].word.length() == 5) valid = 1;
            else std::cout << "Invalid guess (not 5 letter word). Try again\n";
        }

        // Check each letter of the word (l = letter)
        for(int l = 0; l < 5; l++) {
            if(guess[i].word[l] == word[l]) {
                // letter is green
                guess[i].type[l] = 3;
            }
            else {
                if(word.find(guess[i].word[l]) != std::string::npos) {
                    //letter is yellow
                    guess[i].type[l] = 2;
                }
                //letter is black
                else guess[i].type[l] = 1;
            }
            std::cout << guess[i].word[l] << " - " << guess[i].type[l] << "\n";
        }

        // Add letter type to wordle keyboard
        letters[guess[i].word[0] - 'a'] = guess[i].type[0];
        letters[guess[i].word[1] - 'a'] = guess[i].type[1];
        letters[guess[i].word[2] - 'a'] = guess[i].type[2];
        letters[guess[i].word[3] - 'a'] = guess[i].type[3];
        letters[guess[i].word[4] - 'a'] = guess[i].type[4];
        showKeyboard(letters);

        // All characters green = we found the word
        if(guess[i].type[0] == 3 && guess[i].type[1] == 3 && guess[i].type[2] == 3 && guess[i].type[3] == 3 && guess[i].type[4] == 3) {
            std::cout << "Congrats.";
            return 0;
        }

        // If not found, increment guess count and continue
        i += 1;
    }
    
    // Failed to guess word
    std::cout << "Better luck next time.";
    return 0;
}