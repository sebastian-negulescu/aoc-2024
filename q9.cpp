#include "questions.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

void add(std::string &num, unsigned int val) {
    std::string val_str = std::to_string(val);

    // std::cout << "routine start: " << num << " " << val << std::endl;
    
    if (num.size() > val_str.size()) {
        size_t val_zeros = num.size() - val_str.size(); 
        val_str = std::string(val_zeros, '0') + val_str;
    } else if (val_str.size() > num.size()) {
        size_t num_zeros = val_str.size() - num.size();
        std::cout << num_zeros << std::endl;
        num = std::string(num_zeros, '0') + num;
    }

    // std::cout << "formatted: " << num << " " << val << std::endl;

    bool carry = false;
    for (ssize_t i = num.size() - 1; i >= 0; --i) {
        char added = (num[i] - '0') + (val_str[i] - '0');
        if (carry) {
            added++;
            carry = false;
        }
        if (added > 9) {
            carry = true;
            added -= 10;
        }

        num[i] = added + '0';
    }

    if (carry) {
        num = "1" + num;
    }

    // std::cout << "routine end: " << num << " " << val << std::endl;
}

void q9(std::ifstream &input_file) {
    std::string file_layout;
    getline(input_file, file_layout);

    std::vector<int> expanded_layout;

    std::istringstream layout_reader(file_layout);
    char c;
    bool space = true;
    unsigned int index = 0;
    while(layout_reader >> c) {
        unsigned int digit = c - '0';
        for (size_t i = 0; i < digit; ++i) {
            if (space) {
                expanded_layout.push_back(index);
            } else {
                expanded_layout.push_back(-1);
            }
        }
        if (space) {
            index++;
        }

        space = !space;
    }

    for (auto i : expanded_layout) {
        if (i == -1) {
            std::cout << '.';
        } else {
            std::cout << i;
        }
    }
    std::cout << std::endl;

    size_t free_spot = 0;
    for (size_t i = expanded_layout.size() - 1; i >= 0; --i) {
        if (expanded_layout[i] == -1) {
            continue;
        }

        while (free_spot < expanded_layout.size()) {
            if (expanded_layout[free_spot] == -1) {
                break;
            }
            free_spot++;
        }

        // std::cout << free_spot << " " << i << std::endl;

        if (free_spot >= expanded_layout.size()) {
            break;
        }

        if (i < free_spot) {
            break;
        }

        expanded_layout[free_spot] = expanded_layout[i];
        expanded_layout[i] = -1;
    }

    unsigned long long checksum = 0;
    for (size_t i = 0; i < expanded_layout.size(); ++i) {
        if (expanded_layout[i] != -1) {
            checksum += expanded_layout[i] * i;
        }
    }

    std::cout << checksum << std::endl;
}

