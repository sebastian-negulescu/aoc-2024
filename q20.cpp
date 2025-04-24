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

std::pair<char, std::pair<size_t, size_t>> get_adjacent_piece(
        std::vector<std::string> track,
        std::pair<size_t, size_t> position,
        unsigned int direction
) {
    std::pair<size_t, size_t> new_position = position;
    switch (direction) {
        case 0:
            if (position.first != 0) {
                new_position.first--;
                return std::make_pair(
                        track[new_position.first][new_position.second],
                        new_position
                );
            }
            break;
        case 1:
            if (position.second < track[position.first].size() - 1) {
                new_position.second++;
                return std::make_pair(
                        track[new_position.first][new_position.second],
                        new_position
                );
            }
            break;
        case 2:
            if (position.first < track.size() - 1) {
                new_position.first++;
                return std::make_pair(
                        track[new_position.first][new_position.second],
                        new_position
                );
            }
            break;
        case 3:
            if (position.second != 0) {
                new_position.second--;
                return std::make_pair(
                        track[new_position.first][new_position.second],
                        new_position
                );
            }
            break;
    }

    return std::make_pair(-1, position);
}

unsigned int tiles_remaining(std::vector<std::pair<size_t, size_t>> &path, std::pair<size_t, size_t> position) {
    for (ssize_t i = path.size() - 1; i >= 0; --i) {
        if (path[i] == position) {
            return path.size() - i - 1;
        }
    }

    return path.size();
}

void print_track(const std::vector<std::string> &track) {
    for (const std::string &track_line : track) {
        std::cout << track_line << std::endl;
    }
}

unsigned int cheats_at_tile(
        std::vector<std::string> &track,
        const std::pair<size_t, size_t> start,
        const std::pair<size_t, size_t> end, 
        std::vector<std::pair<size_t, size_t>> &path,
        size_t dist,
        std::pair<size_t, size_t> tile,
        unsigned int depth
) {
    if (depth == 0) {
        return 0;
    }

    unsigned int count = 0;
    for (size_t j = 0; j < 4; ++j) {
        std::pair<char, std::pair<size_t, size_t>> first_wall = get_adjacent_piece(track, tile, j);
        if (first_wall.first == '#') {
            track[first_wall.second.first][first_wall.second.second] = '.';
            for (size_t k = 0; k < 4; ++k) {
                // check if a tile that isn't the one we just came from is part of the track
                std::pair<char, std::pair<size_t, size_t>> p = get_adjacent_piece(track, first_wall.second, k);
                if (p.first != '#' && p.second != first_wall.second) {
                    // we have found our way onto the map!
                    // find the length to the finish
                    unsigned int remaining = tiles_remaining(path, p.second);
                    unsigned int total_tiles = dist + remaining;

                    if (total_tiles < path.size()) {
                        count ++;
                    }
                } else if (p.first == '#') {
                    count += cheats_at_tile(track, start, end, path, dist + 1, first_wall.second, depth - 1);
                }
            }
            track[first_wall.second.first][first_wall.second.second] = '#';
        }
    }

    return count;
}

unsigned int num_cheats(
        std::vector<std::string> track, 
        const std::pair<size_t, size_t> start,
        const std::pair<size_t, size_t> end, 
        std::vector<std::pair<size_t, size_t>> &path
) {
    unsigned int count = 0;
    for (size_t i = 0; i < path.size(); ++i) {
        count += cheats_at_tile(track, start, end, path, i + 1, path[i], 2);
    }
    return count;
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

    std::cout << num_cheats(track, start, end, path) << std::endl;
}

