#include "questions.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

unsigned int num_routes(const std::vector<std::string> &map, std::pair<size_t, size_t> location, std::unordered_set<unsigned int> &seen) {
    size_t row = std::get<0>(location);
    size_t col = std::get<1>(location);
    size_t height = map[row][col];
    if (height == '9') {
        /*
        unsigned int hashed_location = row * map[row].size() + col;
        if (seen.contains(hashed_location)) {
            return 0;
        }
        seen.insert(hashed_location);
        */
        return 1;
    }

    unsigned int num = 0;
    for (size_t i = 0; i < 4; ++i) {
        ssize_t mod_row = row;
        ssize_t mod_col = col;
        switch (i) {
            case 0:
                mod_row--;
                break;
            case 1:
                mod_col++;
                break;
            case 2:
                mod_row++;
                break;
            case 3:
                mod_col--;
                break;
            default: break;
        }

        if (mod_row < 0 || mod_col < 0 || mod_row >= map.size() || mod_col >= map[mod_row].size()) {
            continue;
        }

        if (map[mod_row][mod_col] == height + 1) {
            // std::cout << row << " " << col << " " << height << " " << mod_row << " " << mod_col << std::endl;
            num += num_routes(map, std::make_pair(mod_row, mod_col), seen);
        }
    }

    return num;
}

void q10(std::ifstream &input_file) {
    std::string line;
    std::vector<std::string> map;
    while (getline(input_file, line)) {
        map.emplace_back(line);
    }

    std::vector<std::pair<size_t, size_t>> trail_heads;
    for (size_t i = 0; i < map.size(); ++i) {
        for (size_t j = 0; j < map[i].size(); ++j) {
            if (map[i][j] == '0') {
                trail_heads.push_back(std::make_pair(i, j));
            }
        }
    }

    unsigned int trail_scores = 0;
    for (auto trail_head : trail_heads) {
        std::unordered_set<unsigned int> seen;
        trail_scores += num_routes(map, trail_head, seen);
    }

    std::cout << trail_scores << std::endl;
}

