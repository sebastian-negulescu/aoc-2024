#include "questions.h"

#include <vector>
#include <queue>
#include <iostream>
#include <string>
#include <algorithm>

const std::vector<std::vector<char>> num_keypad = {
    {'7', '8', '9'},
    {'4', '5', '6'},
    {'1', '2', '3'},
    {' ', '0', 'A'}};
constexpr std::pair<size_t, size_t> num_start = std::make_pair(3, 2);

const std::vector<std::vector<char>> dir_keypad = {
    {' ', '^', 'A'},
    {'<', 'v', '>'}};
constexpr std::pair<size_t, size_t> dir_start = std::make_pair(0, 2);

void get_ners(
    const std::vector<std::vector<char>> &keypad,
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
    const std::vector<std::vector<char>> &keypad,
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


int shortest_path(
    const std::vector<std::vector<char>> &keypad,
    std::pair<size_t, size_t> pos, 
    char dest,
    std::vector<char> &dirs
) {
    // bfs
    std::vector<std::vector<std::pair<size_t, size_t>>> parent;
    std::pair<size_t, size_t> no_parent = std::make_pair(keypad.size(), keypad[0].size());
    for (size_t i = 0 ; i < keypad.size(); ++i) {
        std::vector<std::pair<size_t, size_t>> line;
        for (size_t j = 0; j < keypad[i].size(); ++j) { 
            line.push_back(no_parent); 
        }
        parent.push_back(line);
    }

    std::queue<std::pair<size_t, size_t>> fringe;
    fringe.push(pos);
    parent[pos.first][pos.second] = pos;

    std::pair<size_t, size_t> end;

    while (!fringe.empty()) {
        std::pair<size_t, size_t> node = fringe.front();
        fringe.pop();

        if (keypad[node.first][node.second] == dest) {
            end = node;
            break;
        }

        std::vector<std::pair<size_t, size_t>> ners;
        get_ners(keypad, node, ners);
        for (std::pair<size_t, size_t> ner : ners) {
            if (parent[ner.first][ner.second] == no_parent) {
                fringe.push(ner);
                parent[ner.first][ner.second] = node;
            }
        }
    }

    std::pair<size_t, size_t> p = end;
    unsigned int length = 0;
    while (p != pos) {
        dirs.push_back(get_dir(parent[p.first][p.second], p));
        p = parent[p.first][p.second];
        length++;
    }

    for (size_t i = 0; i < dirs.size() / 2; ++i) {
        std::swap(dirs[i], dirs[dirs.size() - 1 - i]);
    }

    return length;
}

void permutation(int k, std::string &s) {
    for(int j = 1; j < s.size(); ++j) 
    {
        std::swap(s[k % (j + 1)], s[j]); 
        k = k / (j + 1);
    }
}

unsigned int factorial(unsigned int i) {
    unsigned int f = 0;
    for (size_t j = 0; j < i; ++j) {
        f *= (j + 1);
    }
    return f;
}

void presses(
    std::string &sequence, 
    unsigned int depth, 
    unsigned int max_depth,
    std::vector<char> &dirs
) {
    char p = 'A';
    unsigned int total_presses = 0;
    for (char c : sequence) {
        std::pair<size_t, size_t> start;
        std::pair<size_t, size_t> pos; 
        std::vector<char> ds;

        // std::cout << p << " " << c << std::endl;
        if (depth == 0) {
            start = num_start;
            pos = find_pos(num_keypad, p);
            total_presses += shortest_path(num_keypad, pos, c, ds);
        } else {
            start = dir_start;
            pos = find_pos(dir_keypad, p);
            total_presses += shortest_path(dir_keypad, pos, c, ds);
        }

        /*
        for (char d : ds) {
            std::cout << d << std::endl;
        }
        std::cout << std::endl;
        */
        
        if (depth < max_depth) {
            std::string seq(ds.begin(), ds.end());
            // seq += "A";
            // std::cout << "recursing " << seq << std::endl;
            int lowest = -1;
            std::vector<char> new_new_dirs;
            for (size_t i = 0; i < factorial(seq.size()) - 1; ++i) {
                std::vector<char> new_dirs;
                presses(seq, depth + 1, max_depth, new_dirs);
                if (lowest == -1 || new_dirs.size() < lowest) {
                    lowest = new_dirs.size();
                    new_new_dirs = new_dirs; 
                }
            }
            char a = new_new_dirs[new_new_dirs.size() - 1];
            ds.clear();
            if (depth == 0) {
                std::pair<size_t, size_t> pa = find_pos(num_keypad, a);
                shortest_path(num_keypad, pa, 'A', ds);
            } else {
                std::pair<size_t, size_t> pa = find_pos(dir_keypad, a);
                shortest_path(dir_keypad, pa, 'A', ds);
            }

            dirs.insert(dirs.end(), new_new_dirs.begin(), new_new_dirs.end());
            dirs.insert(dirs.end(), ds.begin(), ds.end());
        } else {
            // std::cout << "no recurse" << std::endl;
            dirs.insert(dirs.end(), ds.begin(), ds.end());
            dirs.push_back('A');
        }

        p = c;
    }
    return;
}


void q21(std::ifstream &input_file) {
    std::string line;
    while (std::getline(input_file, line)) {
        std::vector<char> dirs;
        presses(line, 0, 1, dirs);
        for (char c : dirs) {
            std::cout << c << std::endl;
        }
        std::cout << dirs.size() << std::endl;
    }
}

