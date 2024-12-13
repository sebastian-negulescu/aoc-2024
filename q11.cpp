#include "questions.h"

#include <string>
#include <sstream>
#include <list>
#include <iostream>
#include <unordered_map>

void blink(std::list<unsigned long long> &stones) {
    for (std::list<unsigned long long>::iterator stone_it = stones.begin(); stone_it != stones.end(); ++stone_it) {
        if (*stone_it == 0) {
            *stone_it = 1;
            continue;
        }

        std::string stone_str = std::to_string(*stone_it);
        if (stone_str.size() % 2 == 0) {
            unsigned long long left_stone = std::stoull(stone_str.substr(0, stone_str.size() / 2));
            unsigned long long right_stone = std::stoull(stone_str.substr(stone_str.size() / 2));

            *stone_it = left_stone;
            stones.insert(std::next(stone_it), right_stone);
            stone_it++;
            continue;
        }

        *stone_it *= 2024;
    }
}

// dynamic programming approach
unsigned long long explore(
    const unsigned long long stone, 
    const unsigned int epochs,
    std::unordered_map<std::string, unsigned long long> &stone_map
) {
    if (epochs <= 0) {
        // std::cout << stone << " ";
        return 1;
    }

    std::string hashed_stone = std::to_string(stone) + "," + std::to_string(epochs);
    if (stone_map.contains(hashed_stone)) {
        std::cout << hashed_stone << " " << stone_map[hashed_stone] << std::endl;
        return stone_map[hashed_stone];
    }

    unsigned long long num_stones;
    if (stone == 0) {
        num_stones = explore(1, epochs - 1, stone_map);
        stone_map[hashed_stone] = num_stones;
        return num_stones;
    }

    std::string stone_str = std::to_string(stone);
    if (stone_str.size() % 2 == 0) {
        unsigned long long left_stone = std::stoull(stone_str.substr(0, stone_str.size() / 2));
        unsigned long long right_stone = std::stoull(stone_str.substr(stone_str.size() / 2));

        unsigned long long num_left_stones = explore(left_stone, epochs - 1, stone_map);
        unsigned long long num_right_stones = explore(right_stone, epochs - 1, stone_map);
        stone_map[hashed_stone] = num_left_stones + num_right_stones;

        return num_left_stones + num_right_stones;
    }

    num_stones = explore(stone * 2024, epochs - 1, stone_map);
    stone_map[hashed_stone] = num_stones;
    return num_stones;
}

void q11(std::ifstream &input_file) {
    std::string line;
    getline(input_file, line);

    std::list<unsigned long long> stones;

    std::istringstream stone_reader(line);
    unsigned long long stone;
    while (stone_reader >> stone) {
        stones.push_back(stone);
    }

    std::unordered_map<std::string, unsigned long long> stone_map;
    unsigned long long count = 0;
    const unsigned int epochs = 75;
    for (const auto &stone : stones) {
        count += explore(stone, epochs, stone_map);        
    }

    std::cout << std::endl << count << std::endl;
}


