#include "questions.h"

#include <vector>
#include <iostream>
#include <string>
#include <optional>

const std::vector<std::string> num_keypad = {
    "789",
    "456",
    "123",
    " 0A"};
constexpr std::pair<size_t, size_t> num_start = std::make_pair(3, 2);

const std::vector<std::string> dir_keypad = {
    " ^A",
    "<v>"};
constexpr std::pair<size_t, size_t> dir_start = std::make_pair(0, 2);

std::optional<std::pair<size_t, size_t>> move(const std::vector<std::string> &keypad, std::pair<size_t, size_t> pos, char dir) {
    std::pair<size_t, size_t> old_pos = pos;
    switch (dir) {
        case '^':
            if (pos.first > 0) {
                pos.first--;
            }
            break;
        case '>':
            if (pos.second < keypad[0].size() - 1) {
                pos.second++;
            }
            break;
        case 'v':
            if (pos.first < keypad.size() - 1) {
                pos.first++;
            }
            break;
        case '<':
            if (pos.second > 0) {
                pos.second--;
            }
            break;
        default: break;
    }

    if (pos != old_pos) {
        return pos;
    }

    return {};
}

bool valid_sequence(const std::vector<std::string> &keypad, std::string sequence, std::pair<size_t, size_t> pos) {
    for (char c : sequence) {
        std::optional<std::pair<size_t, size_t>> potential_pos = move(keypad, pos, c);    
        if (!potential_pos.has_value()) {
            return false;
        } else {
            pos = potential_pos.value();
        }
        if (keypad[pos.first][pos.second] == ' ') {
            return false;
        }
    }

    return true;
}

void gen_subseqs(
    const std::vector<std::string> &keypad, 
    std::pair<size_t, size_t> from, 
    std::pair<size_t, size_t> to,
    std::vector<std::string> &subseqs) {

}

std::string shortest_sequence(
    const unsigned int depth, 
    const unsigned int max_depth) {
    return "";
}

void q21(std::ifstream &input_file) {
    std::string line;
    unsigned int total = 0;
    while (std::getline(input_file, line)) {
        std::vector<char> dirs;

        

        for (char c : dirs) {
            std::cout << c; 
        }
        std::cout << std::endl;
        std::cout << dirs.size() << std::endl;

        unsigned int complexity = dirs.size() * std::stoi(line.substr(0, line.size() - 1));
        std::cout << complexity << std::endl;
        total += complexity;
    }
    std::cout << total << std::endl;
}
