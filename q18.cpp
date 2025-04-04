#include "questions.h"

#include <climits>
#include <cassert>

#include <iostream>
#include <string>
#include <queue>
#include <vector>

constexpr size_t MEMORY_SIZE = 71;

typedef struct node {
    std::pair<size_t, size_t> position;
    unsigned long f_score;

    bool operator <(const node &rhs) const {
        return f_score < rhs.f_score;
    }

    bool operator >(const node &rhs) const {
        return f_score > rhs.f_score;
    }
} node;

unsigned long h(std::pair<size_t, size_t> position, std::pair<size_t, size_t> goal) {
    return (goal.first - position.first) + (goal.second - position.second);
}

unsigned int steps(char memory_space[][MEMORY_SIZE]) {

    std::pair<size_t, size_t> start = std::make_pair(0, 0);
    std::pair<size_t, size_t> end = std::make_pair(MEMORY_SIZE - 1, MEMORY_SIZE - 1);

    std::vector<std::vector<std::pair<size_t, size_t>>> backtrack;
    unsigned long g_score[MEMORY_SIZE][MEMORY_SIZE];
    unsigned long f_score[MEMORY_SIZE][MEMORY_SIZE];
    for (size_t i = 0; i < MEMORY_SIZE; ++i) {
        std::vector<std::pair<size_t, size_t>> backtrack_row;
        for (size_t j = 0; j < MEMORY_SIZE; ++j) {
            backtrack_row.push_back(std::make_pair(0, 0));
            g_score[i][j] = ULONG_MAX;
            f_score[i][j] = ULONG_MAX;
        }
        backtrack.push_back(backtrack_row);
    }

    g_score[start.first][start.second] = 0;
    f_score[start.first][start.second] = h(start, end);

    std::queue<node> discovered;
    discovered.push({start, h(start, end)});

    while (!discovered.empty()) {
        node n = discovered.front();
        discovered.pop();

        if (n.position == end) {
            return g_score[end.first][end.second];
        }

        for (unsigned long dir = 0; dir < 4; ++dir) {
            node neighbour = n;
            bool valid = true;
            switch (dir) {
                case 0: // up
                    if (neighbour.position.first == 0) {
                        valid = false;
                        break;
                    }
                    neighbour.position.first--;
                    break;
                case 1: // right
                    if (neighbour.position.second == MEMORY_SIZE - 1) {
                        valid = false;
                        break;
                    }
                    neighbour.position.second++;
                    break;
                case 2: // down 
                    if (neighbour.position.first == MEMORY_SIZE - 1) {
                        valid = false;
                        break;
                    }
                    neighbour.position.first++;
                    break;
                case 3: // left
                    if (neighbour.position.second == 0) {
                        valid = false;
                        break;
                    }
                    neighbour.position.second--;
                    break;
                default: 
                    valid = false;
                    break;
            }

            if (!valid || (memory_space[neighbour.position.first][neighbour.position.second] == '#')) {
                continue; 
            }

            unsigned long tentative_g_score = g_score[n.position.first][n.position.second] + 1;
            neighbour.f_score = tentative_g_score + h(neighbour.position, end);

            if (tentative_g_score < g_score[neighbour.position.first][neighbour.position.second]) {
                g_score[neighbour.position.first][neighbour.position.second] = tentative_g_score;
                f_score[neighbour.position.first][neighbour.position.second] = neighbour.f_score;

                backtrack[neighbour.position.first][neighbour.position.second].first = n.position.first;
                backtrack[neighbour.position.first][neighbour.position.second].second = n.position.second;

                discovered.push(neighbour);
            }
        }
    }

    return UINT_MAX;
}

void q18(std::ifstream &input_file) {
    char memory_space[MEMORY_SIZE][MEMORY_SIZE];
    for (size_t i = 0; i < MEMORY_SIZE; ++i) {
        for (size_t j = 0; j < MEMORY_SIZE; ++j) {
            memory_space[i][j] = '.';
        }
    }

    const std::string delimiter = ",";
    std::string line;
    for (unsigned int corrupted = 0; getline(input_file, line); ++corrupted) {
        ssize_t location = line.find(delimiter);
        assert(location != std::string::npos);

        size_t x = std::stoul(line.substr(0, location));
        size_t y = std::stoul(line.substr(location+delimiter.size(), line.size()-location));

        if (x >= MEMORY_SIZE || y >= MEMORY_SIZE) {
            std::cout << "problem!" << std::endl;
        }

        memory_space[y][x] = '#';

        unsigned int num_steps = steps(memory_space);
        if (num_steps == UINT_MAX) {
            std::cout << corrupted << std::endl;
            std::cout << x << "," << y << std::endl;
            break;
        }
    }
}

