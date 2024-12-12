#include "questions.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

// only works for numbers under 100
bool is_square(unsigned int n) {
    for (size_t i = 0; i < 100; ++i) {
        if (i * i == n) {
            return true;
        }
    }

    return false;
}


bool is_in_bounds(int rows, int cols, const std::pair<int, int> &p) {
    if (p.first >= rows || p.second >= cols) {
        return false;
    }
    if (p.first < 0 || p.second < 0) {
        return false;
    }
    return true;
}


void q8(std::ifstream &input_file) {
    // had to space out the line due to some weird stuff with clang++
    std::unordered_map< char, std::vector< std::pair< int, int> > > frequency_map;

    unsigned int rows;
    unsigned int cols;

    std::vector<std::string> map;
    unsigned int num_antennae = 0;

    std::string line;
    size_t row = 0;
    while (getline(input_file, line)) {
        map.push_back(line);
        std::istringstream point(line);
        std::cout << line << std::endl;

        char c;
        size_t col = 0;
        while (point >> c) {
            if (c == '.') {
                col++;
                continue;
            }

            frequency_map[c].push_back(std::make_pair(row, col));
            col++;
        }
        row++;
        cols = col;
    }
    rows = row;

    std::unordered_set<int> unique_anodes;
    unsigned int count = 0;
    for (const auto &first : frequency_map) {
        char frequency = first.first;
        auto antennae = first.second;

        for (size_t i = 0; i < antennae.size(); ++i) {
            unique_anodes.insert(antennae[i].first * cols + antennae[i].second);
            for (size_t j = i + 1; j < antennae.size(); ++j) {
                int row_diff = antennae[i].first - antennae[j].first;
                int col_diff = antennae[i].second - antennae[j].second;

                std::pair<int, int> i_anode = {
                    row_diff + antennae[i].first,
                    col_diff + antennae[i].second
                };
                std::pair<int, int> j_anode = {
                    antennae[j].first - row_diff,
                    antennae[j].second - col_diff
                };

                while (is_in_bounds(rows, cols, i_anode)) {
                    map[i_anode.first][i_anode.second] = '#';
                    unique_anodes.insert(i_anode.first * cols + i_anode.second);
                    i_anode.first += row_diff;
                    i_anode.second += col_diff;
                }
                while (is_in_bounds(rows, cols, j_anode)) {
                    map[j_anode.first][j_anode.second] = '#';
                    unique_anodes.insert(j_anode.first * cols + j_anode.second);
                    j_anode.first -= row_diff;
                    j_anode.second -= col_diff;
                }
            }
        }
    }

    for (auto map_line : map) {
        std::cout << map_line << std::endl;
    }
    std::cout << unique_anodes.size() << std::endl;
}

