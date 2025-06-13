#include "questions.h"

#include <vector>
#include <iostream>
#include <string>
#include <optional>
#include <unordered_set>

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

std::optional<std::pair<size_t, size_t>> get_pos(const std::vector<std::string> &keypad, char c) {
    for (size_t i = 0; i < keypad.size(); ++i) {
        for (size_t j = 0; j < keypad[i].size(); ++j) {
            if (keypad[i][j] == c) {
                return std::make_pair(i, j);
            }
        }
    }

    return {};
}

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

void gen_dirs(const std::vector<std::string> &keypad, char start, char end, std::string &dirs) {
    std::pair<ssize_t, ssize_t> start_pos = get_pos(keypad, start).value();
    std::pair<ssize_t, ssize_t> end_pos = get_pos(keypad, end).value();

    std::pair<ssize_t, ssize_t> diff = std::make_pair(end_pos.first - start_pos.first, end_pos.second - start_pos.second);

    char c = '<';
    if (diff.first > 0) {
        c = '>'; 
    }
    for (size_t i = 0; i < std::abs(diff.first); ++i) {
        dirs += c;
    }

    c = '^';
    if (diff.second > 0) {
        c = 'v';
    }
    for (size_t i = 0; i < std::abs(diff.second); ++i) {
        dirs += c;
    }
}

bool valid_sequence(const std::vector<std::string> &keypad, std::string &sequence, std::pair<size_t, size_t> pos) {
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
    unsigned int k,
    std::string &subseq,
    std::unordered_set<std::string> &unique_subseqs,
    std::unordered_set<std::string> &valid_subseqs) {
    if (k == 1) {
        if (!unique_subseqs.contains(subseq)) {
            unique_subseqs.emplace(subseq);
            if (valid_sequence(dir_keypad, subseq, get_pos(dir_keypad, subseq[0]).value())) {
                valid_subseqs.emplace(subseq);
            }
        }
        return;
    }

    for (unsigned int i = 0; i < k - 1; ++i) {
        if (k % 2 == 0) {
            std::swap(subseq[i], subseq[k - 1]); 
        } else {
            std::swap(subseq[0], subseq[k - 1]);
        }
        gen_subseqs(k - 1, subseq, unique_subseqs, valid_subseqs);
    }
}

void shortest_sequence(
    const unsigned int depth, 
    const unsigned int max_depth,
    std::string &dirs
) {
    if (depth == max_depth) {
        return;
    }

    std::unordered_set<std::string> unique_subseqs;
    std::unordered_set<std::string> valid_subseqs;
    gen_subseqs(dirs.size(), dirs, unique_subseqs, valid_subseqs);

    std::string shortest_subseq = "";

    for (const std::string &s : valid_subseqs) {
        // find the shortest one
        char c;
        for (char dir : s.substr(1)) {
            std::string sub_dirs;
            gen_dirs(num_keypad, c, dir, sub_dirs);
            shortest_sequence(0, 2, sub_dirs);
            c = dir;
        }
    }

    return;
}

void q21(std::ifstream &input_file) {
    std::string line;
    unsigned int total = 0;
    while (std::getline(input_file, line)) {
        char c = line[0];
        for (char dir : line.substr(1)) {
            std::string dirs;
            gen_dirs(num_keypad, c, dir, dirs);
            shortest_sequence(0, 2, dirs);
            c = dir;
        }
    }
    std::cout << total << std::endl;
}
