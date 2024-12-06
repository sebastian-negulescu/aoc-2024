#include "questions.h"

#include <string>
#include <iostream>
#include <vector>

// direction: 0 is North, 
bool check_direction(
    const std::vector<std::string> &crossword, 
    ssize_t row, 
    ssize_t col,
    const std::string &word,
    size_t word_ind,
    const unsigned int direction
) {
    if (word_ind >= word.size()) {
        return true;
    }

    // std::cout << row << " " << col << " from check" << std::endl;
    if (row < 0 || col < 0) {
        return false;
    }

    if (row >= crossword.size() || col >= crossword[row].size()) {
        return false;
    }

    if (crossword[row][col] != word[word_ind]) {
        return false;
    }

    if (direction == 0 || direction == 1 || direction == 7) {
        row -= 1;
    }
    if (direction == 3 || direction == 4 || direction == 5) {
        row += 1;
    }
    if (direction == 5 || direction == 6 || direction == 7) {
        col -= 1;
    }
    if (direction == 1 || direction == 2 || direction == 3) {
        col += 1;
    }

    return check_direction(crossword, row, col, word, word_ind + 1, direction);
}

void q4(std::ifstream &input_file) {
    std::string line;
    std::vector<std::string> crossword;

    std::string word = "XMAS";

    while (getline(input_file, line)) {
        crossword.push_back(line);
    }

    size_t total = 0;

    // bool res = check_direction(crossword, 0, 5, word, 0, 2);
    // std::cout << "result: " << res << std::endl;

    for (size_t row = 0; row < crossword.size(); ++row) {
        for (size_t col = 0; col < crossword[row].size(); ++col) {
            // std::cout << row << " " << col << std::endl;
            size_t count = 0;
            for (size_t dir = 0; dir < 8; ++dir) {
                if (check_direction(crossword, row, col, word, 0, dir)) {
                    count++;
                }
            }
            total += count;
        }
    }

    std::cout << total << std::endl;

    total = 0;
    for (size_t row = 0; row < crossword.size(); ++row) {
        for (size_t col = 0; col < crossword[row].size(); ++col) {
            if (row < 0 || col < 0) {
                continue;
            }

            if (row >= crossword.size() || col >= crossword[row].size()) {
                continue;
            }

            for (size_t dir = 0; dir < 8; ++dir) {
                if (dir % 2 == 0) {
                    continue;
                }
                bool tail = check_direction(crossword, row, col, "AS", 0, dir);
                bool tail_a = check_direction(crossword, row, col, "AS", 0, (dir + 2) % 8); 
                bool tail_b = check_direction(crossword, row, col, "AS", 0, (dir + 8 - 2) % 8); 

                bool head = check_direction(crossword, row, col, "AM", 0, (dir + 4) % 8);
                bool head_a = check_direction(crossword, row, col, "AM", 0, (dir + 4 + 2) % 8);
                bool head_b = check_direction(crossword, row, col, "AM", 0, (dir + 4 + 8 - 2) % 8); 

                if ((tail && tail_a && head && head_a) || 
                    (tail && tail_b && head && head_b)) {
                    // std::cout << row << " " << col << std::endl;
                    total++; 
                    break;
                }
            }
        }
    }

    std::cout << total << std::endl;
}

