#include "questions.h"

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>


int move_guard(
    std::vector<std::string> &map, 
    std::pair<unsigned int, unsigned int> &guard_loc, 
    unsigned int guard_dir,
    std::vector<std::pair<unsigned int, unsigned int>> *guard_path
) {
    int marked = 1;
    std::pair<unsigned int, unsigned int> next_guard_loc = guard_loc;
    unsigned int steps = 0;
    while (true) {
        // get next loc
        switch (guard_dir) {
            case 0: // up
                next_guard_loc.first--; 
                break;
            case 1: // right
                next_guard_loc.second++;
                break;
            case 2: // down
                next_guard_loc.first++;
                break;
            case 3: // left
                next_guard_loc.second--;
                break;
            default:
                break;
        }

        // out of bounds, want to stop
        if (next_guard_loc.first < 0 || next_guard_loc.first >= map.size()) {
            marked = -1;
            break;
        }
        if (next_guard_loc.second < 0 || next_guard_loc.second >= map[next_guard_loc.first].size()) {
            marked = -1;
            break;
        }

        if (map[next_guard_loc.first][next_guard_loc.second] == '#') {
            break;
        }

        if (map[next_guard_loc.first][next_guard_loc.second] == '.') {
            map[next_guard_loc.first][next_guard_loc.second] = 'X';
            marked = 1;
        }

        if (guard_path) {
            guard_path->push_back(next_guard_loc);
        }
        guard_loc = next_guard_loc;
        steps++;
    }

    // std::cout << marked << std::endl;
    return marked;
}

void q6(std::ifstream &input_file) {
    std::string line;
    std::vector<std::string> map;

    std::pair<unsigned int, unsigned int> guard_loc;
    unsigned int guard_dir = 0; // starting out up

    unsigned int row = 0;
    while (getline(input_file, line)) {
        size_t col = line.find('^');
        if (col != std::string::npos) {
            guard_loc.first = row;
            guard_loc.second = col;
            line[col] = 'X';
        }
        map.push_back(line);
        ++row;
    }

    std::pair<unsigned int, unsigned int> initial_guard_loc = guard_loc;

    std::cout << "guard location " << guard_loc.first << " " << guard_loc.second << std::endl;

    // go until we have a traversal that does not unveil any new information
    
    std::vector<std::pair<unsigned int, unsigned int>> guard_path;
    while (move_guard(map, guard_loc, guard_dir, &guard_path) == 1) {
        guard_dir++;
        guard_dir %= 4;
    }

    std::unordered_set<unsigned int> barriers;
    for (auto path_guard_loc : guard_path) {
        const unsigned int hashed_path_guard_loc = 
            path_guard_loc.first * map[path_guard_loc.first].size() + path_guard_loc.second;

        if (barriers.contains(hashed_path_guard_loc)) {
            continue;
        }

        // clear previous map
        for (size_t i = 0; i < map.size(); ++i) {
            for (size_t j = 0; j < map.size(); ++j) {
                if (map[i][j] == 'X') {
                    map[i][j] = '.';    
                }
            }
        }

        // insert obstacle
        map[path_guard_loc.first][path_guard_loc.second] = '#';

        /*
        // output map
        for (auto map_line : map) {
            std::cout << map_line << std::endl;
        }
        */

        // run and try to detect loop
        guard_dir = 0;
        guard_loc = initial_guard_loc;
        int result = 1;
        std::unordered_set<unsigned int> guard_locs;
        while (result != -1) {
            result = move_guard(map, guard_loc, guard_dir, nullptr);
            const unsigned int hashed_guard_loc = 
                ((guard_loc.first * map[guard_loc.first].size() + guard_loc.second) << 4) | (1 << guard_dir);
            if (guard_locs.contains(hashed_guard_loc)) {
                barriers.insert(hashed_path_guard_loc);
                break;
            }
            guard_locs.insert(hashed_guard_loc);
            guard_dir++;
            guard_dir %= 4;
        }
        // std::cout << result << std::endl;
        /*
        if (result == 0) {
            barriers.insert(hashed_path_guard_loc);
        }
        */

        map[path_guard_loc.first][path_guard_loc.second] = '.';
    }
    

    std::cout << barriers.size() << std::endl;
}

