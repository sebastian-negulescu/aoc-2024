#include "questions.h"

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <unordered_map>

unsigned long long can_be_made(std::string pattern, std::vector<std::string> &towels, std::unordered_map<std::string, unsigned long long> &makeable) {
    if (pattern.empty()) {
        return 1;
    }

    if (makeable.find(pattern) != makeable.end()) {
        return makeable[pattern];
    }

    unsigned long long total_ways = 0;
    for (const std::string &towel : towels) {
        if (pattern.rfind(towel, 0) == 0) {
            std::string sub_pattern = pattern.substr(towel.size(), pattern.size() - towel.size());
            unsigned long long ways = can_be_made(sub_pattern, towels, makeable);
            if (ways > 0) {
                total_ways += ways;
            }
        }
    }
    makeable[pattern] = total_ways;
    return total_ways;
}

void q19(std::ifstream &input_file) {
    std::string line;
    getline(input_file, line);

    std::vector<std::string> towels;
    std::string current_towel = "";
    std::istringstream reader(line);
    char c;
    while (reader >> c) {
        switch (c) {
            case ' ':
                break;
            case ',':
                towels.emplace_back(current_towel);
                current_towel.clear();
                break;
            default:
                current_towel += c;
        }
    }
    towels.push_back(current_towel);

    for (std::string &towel : towels) {
        std::cout << towel << std::endl;
    }
    std::cout << std::endl;

    getline(input_file, line);
    unsigned long long count = 0;
    while (getline(input_file, line)) {
        std::unordered_map<std::string, unsigned long long> makeable;
        unsigned long long ways = can_be_made(line, towels, makeable);
        std::cout << line << " " << ways << std::endl;
        count += ways;
    }

    std::cout << count << std::endl;
}

