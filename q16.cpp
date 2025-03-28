#include "questions.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <vector>
#include <list>
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

    bool operator ==(const node &rhs) const {
        return p == rhs.p && d == rhs.d;
    }
} node;


void mark_tiles_on_path(std::vector<std::vector<bool>> &path, std::vector<std::vector<std::vector<std::list<node>>>> &reference, node n) {
    path[n.p.first][n.p.second] = true;
    if (reference[n.p.first][n.p.second][n.d].empty()) {
        return;
    }

    reference[n.p.first][n.p.second][n.d].unique();
    for (node next : reference[n.p.first][n.p.second][n.d]) {
        mark_tiles_on_path(path, reference, next);
    }
}


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
    std::vector<std::vector<std::vector<std::list<node>>>> reference;
    std::vector<std::vector<bool>> path;
    for (size_t i = 0; i < maze.size(); ++i) {
        std::vector<std::vector<std::list<node>>> row_ref;
        std::vector<bool> row_path;
        for (size_t j = 0; j < maze.size(); ++j) {
            std::vector<std::list<node>> col_ref;
            for(size_t k = 0; k < 4; ++k) {
                col_ref.push_back({});
                distances[i][j][k] = -1;
            }
            row_ref.push_back(col_ref);
            row_path.push_back(false);
        }
        reference.push_back(row_ref);
        path.push_back(row_path);
    }
    distances[start.first][start.second][EAST] = 0;

    std::priority_queue<node, std::vector<node>, std::greater<node>> unvisited;
    unvisited.push({start, EAST, 0});

    std::cout << "starting Djikstra..." << std::endl;
    long min_distance = -1;
    while (!unvisited.empty()) {
        node n = unvisited.top();
        unvisited.pop();

        if (n.p == end) {
            std::cout << n.distance << std::endl;
            if (min_distance == -1) {
                min_distance = n.distance;
            }
            continue;
        }

        node next = n;
        next.move();
        long current_distance = distances[next.p.first][next.p.second][next.d];
        // std::cout << "next: " << next.p.first << " " << next.p.second << " " << next.d << ", dist: " << current_distance << std::endl;
        if (maze[next.p.first][next.p.second] != '#') {
            if (current_distance == -1 || current_distance >= next.distance) {
                if (current_distance != next.distance) {
                    reference[next.p.first][next.p.second][next.d].clear();
                }
                reference[next.p.first][next.p.second][next.d].push_back(n);
                distances[next.p.first][next.p.second][next.d] = next.distance;
                unvisited.push(next);
            }
        }

        next = n;
        next.turn(true);
        current_distance = distances[next.p.first][next.p.second][next.d];
        if (current_distance == -1 || current_distance >= next.distance) {
            if (current_distance != next.distance) {
                reference[next.p.first][next.p.second][next.d].clear();
            }
            reference[next.p.first][next.p.second][next.d].push_back(n);
            distances[next.p.first][next.p.second][next.d] = next.distance;
            unvisited.push(next);
        }

        next = n;
        next.turn(false);
        current_distance = distances[next.p.first][next.p.second][next.d];
        if (current_distance == -1 || current_distance >= next.distance) {
            if (current_distance != next.distance) {
                reference[next.p.first][next.p.second][next.d].clear();
            }
            reference[next.p.first][next.p.second][next.d].push_back(n);
            distances[next.p.first][next.p.second][next.d] = next.distance;
            unvisited.push(next);
        }
    }

    
    for (size_t i = 0; i < 4; ++i) {
        node n = {end, (direction)i};
        if (distances[end.first][end.second][i] == min_distance) {
            mark_tiles_on_path(path, reference, n);
        }
    }

    long count = 0;
    for (size_t i = 0; i < path.size(); ++i) {
        for (size_t j = 0; j < path[i].size(); ++j) {
            std::cout << (path[i][j] ? '.' : ' ');
            count += path[i][j] ? 1 : 0;
        }
        std::cout << std::endl;
    }
    std::cout << count << std::endl;
}

