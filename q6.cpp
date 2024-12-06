#include "questions.h"

#include <vector>
#include <string>

void q6(std::ifstream &input_file) {
    std::string line;
    std::vector<std::string> map;

    while (getline(input_file, line)) {
        size_t guard_pos = line.find('^');
        if (guard_pos == std::string::npos) {

        }
        map.push_back(line);
    }
}

