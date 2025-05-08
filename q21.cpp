#include "questions.h"

#include <vector>
#include <queue>
#include <iostream>
#include <string>
#include <algorithm>

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

void get_ners(
    const std::vector<std::string> &keypad,
    std::pair<size_t, size_t> pos,
    std::vector<std::pair<size_t, size_t>> &ners) {
    const size_t rows = keypad.size();
    const size_t cols = keypad[0].size();

    if (pos.first > 0) {
        std::pair<size_t, size_t> ner = std::make_pair(pos.first - 1, pos.second);
        if (keypad[ner.first][ner.second] != ' ') {
            ners.push_back(ner);
        }
    }
    if (pos.second < cols - 1) {
        std::pair<size_t, size_t> ner = std::make_pair(pos.first, pos.second + 1);
        if (keypad[ner.first][ner.second] != ' ') {
            ners.push_back(ner);
        }
    }
    if (pos.first < rows - 1) {
        std::pair<size_t, size_t> ner = std::make_pair(pos.first + 1, pos.second);
        if (keypad[ner.first][ner.second] != ' ') {
            ners.push_back(ner);
        }
    }
    if (pos.second > 0) {
        std::pair<size_t, size_t> ner = std::make_pair(pos.first, pos.second - 1);
        if (keypad[ner.first][ner.second] != ' ') {
            ners.push_back(ner);
        }
    }
}

std::pair<size_t, size_t> find_pos(
    const std::vector<std::string> &keypad,
    char c
) {
    for (size_t i = 0; i < keypad.size(); ++i) {
        for (size_t j = 0; j < keypad[i].size(); ++j) {
            if (keypad[i][j] == c) {
                return std::make_pair(i, j);
            }
        }
    }

    return std::make_pair(keypad.size(), keypad[0].size());
}


char get_dir(
    const std::pair<size_t, size_t> from,
    const std::pair<size_t, size_t> to
) {
    if (from.first < to.first) {
        return 'v';
    }
    if (from.first > to.first) {
        return '^';
    }
    if (from.second < to.second) {
        return '>';
    }
    if (from.second > to.second) {
        return '<';
    }
    return '=';
}

std::pair<size_t, size_t> get_pos(const std::vector<std::string> &keypad, char key) {
    for (size_t i = 0; i < keypad.size(); ++i) {
        for (size_t j = 0; j < keypad[i].size(); ++j) {
            if (keypad[i][j] == key) {
                return std::make_pair(i, j);
            }
        }
    }

    return std::make_pair(keypad.size(), keypad[0].size());
}

void get_path(const std::vector<std::string> &keypad, char from, char to, std::vector<char> &path) {
    std::pair<size_t, size_t> from_pos = get_pos(keypad, from);
    std::pair<size_t, size_t> to_pos = get_pos(keypad, to);

    // get height difference
    int height = (int)from_pos.first - (int)to_pos.first;
    char height_sym;
    if (height > 0) {
        height_sym = '^';
    } else if (height < 0) {
        height_sym = 'v';
    }

    // get width difference
    int width = (int)from_pos.second - (int)to_pos.second;
    char width_sym;
    if (width > 0) {
        width_sym = '<';
    } else if (width < 0) {
        width_sym = '>';
    }

    // default ordering will be to move height then width
    // if this is bad, we flip
    bool default_good = true;
    std::pair<size_t, size_t> cur_pos = from_pos;
    for (size_t i = 0; i < std::abs(height) && default_good; ++i) {
    
    }
}

void presses(unsigned int depth, unsigned int max_depth, std::string &sequence, std::vector<char> &dirs) {
    if (depth > max_depth) {
        return;
    }

    if (depth == 0) {
        char from = 'A';
        for (char to : sequence) {
            std::vector<char> path;
            get_path(num_keypad, from, to, path);
        }
    }
}

void q21(std::ifstream &input_file) {
    std::string line;
    while (std::getline(input_file, line)) {
        std::vector<char> dirs;

        presses(0, 0, line, dirs);
        
        for (char c : dirs) {
            std::cout << c << std::endl;
        }
        std::cout << dirs.size() << std::endl;
    }
}

