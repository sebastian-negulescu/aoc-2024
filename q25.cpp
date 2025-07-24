#include "questions.h"

#include <string>
#include <iostream>
#include <vector>

void q25(std::ifstream &input_file) {
    std::string line; 

    std::vector<std::vector<int>> keys;
    std::vector<std::vector<int>> locks;

    do {
        std::vector<std::string> device;
        for (size_t i = 0; i < 7; ++i) {
            std::getline(input_file, line);
            device.push_back(line);
        }

        if (device[0] == "#####" && device[6] == ".....") {
            // lock 
            std::vector<int> translated(5, -1);
            for (size_t i = 0; i < 7; ++i) {
                for (size_t j = 0; j < 5; ++j) {
                    if (device[i][j] != '#' && translated[j] == -1) {
                        translated[j] = i - 1;
                    }
                }
            }
            locks.push_back(translated);
        }

        if (device[0] == "....." && device[6] == "#####") {
            // key 
            std::vector<int> translated(5, -1);
            for (size_t i = 0; i < 7; ++i) {
                for (size_t j = 0; j < 5; ++j) {
                    if (device[i][j] != '.' && translated[j] == -1) {
                        translated[j] = 6 - i;
                    }
                }
            }
            keys.push_back(translated);
        }
    } while (std::getline(input_file, line));

    unsigned int count = 0;
    for (auto &lock : locks) {
        for (auto &key : keys) {
            bool compatible = true;
            for (size_t i = 0; i < 5; ++i) {
                if (lock[i] + key[i] >= 6) {
                    // not compatible
                    compatible = false;
                    break;
                }
            }
            if (compatible) {
                ++count;
            }
        }
    }
    std::cout << count << std::endl;
}
