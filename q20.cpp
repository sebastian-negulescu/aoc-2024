#include "questions.h"

#include <cassert>
#include <climits>

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

void create_path(
        const std::vector<std::string> &track, 
        const std::pair<size_t, size_t> end, 
        std::vector<std::pair<size_t, size_t>> &path
) {
    if (path[path.size() - 1] == end) {
        return;
    }

    std::pair<size_t, size_t> position = path[path.size() - 1];
    for (size_t i = 0; i < 4; ++i) {
        std::pair<size_t, size_t> next = position;
        switch (i) {
            case 0: // up
                next.first--;
                break;
            case 1: // right
                next.second++;
                break;
            case 2:
                next.first++;
                break;
            case 3:
                next.second--;
                break;
            default: break;
        }

        if (track[next.first][next.second] == '#') continue;
        if (path.size() > 1) {
            if (path[path.size() - 2] == next) continue;
        }

        path.push_back(next);
        break;
    }

    create_path(track, end, path);
}

char get_adjacent_piece(
        std::vector<std::string> track,
        std::pair<size_t, size_t> position,
        unsigned int direction
) {
    switch (direction) {
        case 0:
            if (position.first != 0) {
                return track[position.first - 1][position.second];
            }
            break;
        case 1:
            if (position.second < track[position.first].size() - 1) {
                return track[position.first][position.second + 1];
            }
            break;
        case 2:
            if (position.first < track.size() - 1) {
                return track[position.first + 1][position.second];
            }
            break;
        case 3:
            if (position.second != 0) {
                return track[position.first][position.second - 1];
            }
            break;
    }

    return -1;
}

unsigned int num_cheats(
        std::vector<std::string> track, 
        const std::pair<size_t, size_t> start,
        const std::pair<size_t, size_t> end, 
        std::vector<std::pair<size_t, size_t>> &path
) {
    for (size_t i = 0; i < path.size(); ++i) {
        for (size_t j = 0; j < 4; ++j) {
            char adjacent_piece = get_adjacent_piece(track, path[i], j);
            if (adjacent_piece == '#') {
                // TODO: find cheat
            }
        }
    }
    return 0;
}

void q20(std::ifstream &input_file) {
    std::vector<std::string> track;

    std::pair<size_t, size_t> start;
    std::pair<size_t, size_t> end;

    std::string line;
    while (std::getline(input_file, line)) {
        std::istringstream reader(line);
        char c;
        size_t col = 0;
        while (reader >> c) {
            switch (c) {
                case 'S':
                    start.first = track.size();
                    start.second = col;
                    break;
                case 'E':
                    end.first = track.size();
                    end.second = col;
            }
            col++;
        }
        track.push_back(line);
    }

    assert(track[start.first][start.second] == 'S');
    assert(track[end.first][end.second] == 'E');

    std::vector<std::pair<size_t, size_t>> path;
    path.push_back(start);
    create_path(track, end, path);

    for (std::pair<size_t, size_t> position : path) {
        std::cout << position.first << " " << position.second << std::endl;
    }
}

