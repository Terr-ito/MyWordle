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

// Counts the number of occurences of a char in a string
int countCharacter(std::string s, char c) {
    int count = 0;
 
    for(unsigned i = 0; i < s.length(); i++) {
        if(s[i] == c) count++;
    }

    return count;
}

void showKeyboard(unsigned letters[]) {
    for(int i = 0; i < 26; i++) {
        std::cout << char('a' + i) << " - ";

        // Unsigned type to color
        if(letters[i] == 0) std::cout << "grey";
        else if(letters[i] == 1) std::cout << "black";
        else if(letters[i] == 2) std::cout << "yellow";
        else std::cout << "green";

        std::cout << " | ";
        if(i % 4 == 3) std::cout << "\n";
    }
    std::cout << "\n";
}

// To be valid, check if it's 5 letters 
// and then search for it in the database
void isValid(std::string &validWord, std::vector<std::string> database) {
    int valid = 0;

    while(valid == 0) {
        getline(std::cin, validWord);
        if(validWord.length() == 5) {
            for(unsigned i = 0; i <= database.size() - 1; i++) {
                if(validWord.compare(database[i]) == 0) {
                    valid = 1;
                    break;
                }
                else if(validWord.compare(database[i]) <= 0) {
                    std::cout << "Invalid guess (word is not in the database). Try again\n";
                    break;
                }
            }
        }
        else std::cout << "Invalid guess (not 5 letter word). Try again\n";
    }
}

// 1st "for" is a surface level check
// 2nd "for" is for duplicate letter edge cases
void checkGuess(std::string word, wordle &guess) {
    // 1st
    for(int l = 0; l < 5; l++) {
        // Letter is green
        if(guess.word[l] == word[l]) guess.type[l] = 3;
        // Letter is yellow (for now)
        else if(word.find(guess.word[l]) != std::string::npos) guess.type[l] = 2;
        // Letter is black
        else guess.type[l] = 1;
    }

    // 2nd - yellow letter edge cases
    for(int l = 0; l < 5; l++) {
        if(guess.type[l] == 2) {
            // Checks if there are more duplicate letter guesses than in the word
            if(countCharacter(guess.word, guess.word[l]) > countCharacter(word, guess.word[l])) {
                guess.word[l] = toupper(guess.word[l]);
                // Letter is black
                guess.type[l] = 1;
            }
        }
    }

    // Display guess results
    for(int l = 0; l < 5; l++) {
        guess.word[l] = tolower(guess.word[l]);
        std::cout << guess.word[l] << " - " << guess.type[l] << "\n";
    }

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
    while(std::getline(wordDB, text_line)) {
        database.push_back(text_line);
    }
    std::uniform_int_distribution<> distr(0, database.size() - 1);

    // Store random word and check if it's valid
    int random = distr(gen);
    word = database[random];
    if(word.length() != 5) {
        std::cerr << "Word pulled from database in not a valid word. (not 5 letter word)\n" <<
                    "Corrupt database. (line: " << random << ")\n";
        return 1;
    }

    // Pulled word
    // std::cout << word << "\n";
    std::cout << "You can begin guessing." << "\n" << "5 letter words only!" << "\n";
    
    // Start guessing loop (i = guess count) - 6 guesses
    unsigned i = 0;
    while(i < 6) {
        // Get guessed word from stdin and check if it's valid
        isValid(guess[i].word, database);

        // Check each letter of the word (l = letter)
        checkGuess(word, guess[i]);

        // Update letter type to wordle keyboard
        letters[guess[i].word[0] - 'a'] = guess[i].type[0];
        letters[guess[i].word[1] - 'a'] = guess[i].type[1];
        letters[guess[i].word[2] - 'a'] = guess[i].type[2];
        letters[guess[i].word[3] - 'a'] = guess[i].type[3];
        letters[guess[i].word[4] - 'a'] = guess[i].type[4];
        showKeyboard(letters);

        // All characters green = we found the word
        if(guess[i].type[0] == 3 && guess[i].type[1] == 3 && guess[i].type[2] == 3 && guess[i].type[3] == 3 && guess[i].type[4] == 3) {
            std::cout << "Congrats!";
            return 0;
        }

        // If not found, increment guess count and continue
        i += 1;
    }
    
    // Failed to guess word
    std::cout << "The word was \"" << word << "\"." << "\n";
    std::cout << "Better luck next time.";
    return 0;
}