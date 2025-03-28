#include "questions.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>
#include <string>
#include <queue>

enum direction {
    NORTH,
    EAST,
    SOUTH,
    WEST
};

typedef struct node {
    std::pair<size_t, size_t> p;
    direction d;
    long distance;

    void move() {
        switch (d) {
            case NORTH:
                p.first--;
                break;
            case SOUTH:
                p.first++;
                break;
            case EAST:
                p.second++;
                break;
            case WEST:
                p.second--;
                break;
            default: break;
        }
        distance++;
    }

    void turn(bool left) {
        unsigned int new_d = d + 4;    
        new_d = left ? new_d - 1 : new_d + 1;
        new_d %= 4;
        d = (direction)new_d;
        distance += 1000;
    }

    bool operator >(const node &rhs) const {
        return distance > rhs.distance;
    }

    bool operator <(const node &rhs) const {
        return distance < rhs.distance;
    }
} node;

void q16(std::ifstream &input_file) {
    std::vector<std::string> maze;

    std::pair<size_t, size_t> start;
    std::pair<size_t, size_t> end;

    std::string line;
    size_t row = 0, col = 0;
    while (getline(input_file, line)) {
        col = 0;
        std::istringstream reader(line);
        char c;
        while (reader >> c) {
            switch (c) {
                case 'S':
                    start = std::make_pair(row, col);
                    break;
                case 'E':
                    end = std::make_pair(row, col);
                    break;
                default: break;
            }
            col++;
        }
        maze.push_back(line);
        row++;
    }

    std::cout << "start: " << start.first << " " << start.second << std::endl;
    std::cout << "end: " << end.first << " " << end.second << std::endl;

    long distances[maze.size()][maze[0].size()][4];
    for (size_t i = 0; i < maze.size(); ++i) {
        for (size_t j = 0; j < maze.size(); ++j) {
            for(size_t k = 0; k < 4; ++k) {
                distances[i][j][k] = -1;
            }
        }
    }
    distances[start.first][start.second][EAST] = 0;

    std::priority_queue<node, std::vector<node>, std::greater<node>> unvisited;
    unvisited.push({start, EAST, 0});

    while (!unvisited.empty()) {
        node n = unvisited.top();
        unvisited.pop();

        if (n.p == end) {
            std::cout << n.distance << std::endl;
        }

        node next = n;
        next.move();
        long current_distance = distances[next.p.first][next.p.second][next.d];
        // std::cout << "next: " << next.p.first << " " << next.p.second << " " << next.d << ", dist: " << current_distance << std::endl;
        if (maze[next.p.first][next.p.second] != '#') {
            if (current_distance == -1 || current_distance > next.distance) {
                distances[next.p.first][next.p.second][next.d] = next.distance;
                unvisited.push(next);
            }
        }

        next = n;
        next.turn(true);
        current_distance = distances[next.p.first][next.p.second][next.d];
        // std::cout << "next: " << next.p.first << " " << next.p.second << " " << next.d << ", dist: " << current_distance << std::endl;
        if (current_distance == -1 || current_distance > next.distance) {
            distances[next.p.first][next.p.second][next.d] = next.distance;
            unvisited.push(next);
        }

        next = n;
        next.turn(false);
        current_distance = distances[next.p.first][next.p.second][next.d];
        // std::cout << "next: " << next.p.first << " " << next.p.second << " " << next.d << ", dist: " << current_distance << std::endl;
        if (current_distance == -1 || current_distance > next.distance) {
            distances[next.p.first][next.p.second][next.d] = next.distance;
            unvisited.push(next);
        }
    }
}

