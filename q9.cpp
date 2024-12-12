#include "questions.h"

#include <iostream>
#include <vector>
#include <list>
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

    int id = -1;
    size_t block_size = 0;
    for (ssize_t i = expanded_layout.size() - 1; i >= 0; --i) {
        if (expanded_layout[i] == id) {
            block_size++;
        } else if (id != -1) {
            std::cout << i << " " << id << " " << block_size << std::endl;
            // look to move block
            int target_start = -1;
            size_t target_size = 0;
            for (size_t j = 0; j < expanded_layout.size(); ++j) {
                if (j > i) {
                    target_start = -1;
                    break;
                }

                if (expanded_layout[j] == -1) {
                    target_size++;
                } else {
                    target_size = 0;
                }
                if (target_size == block_size) {
                    std::cout << "target size " << target_size << std::endl;
                    target_start = j - (target_size - 1);
                    break;
                }
            }

            if (target_start != -1) {
                std::cout << "moving to " << target_start << std::endl;
                for (size_t j = 0; j < target_size; ++j) {
                    expanded_layout[target_start + j] = expanded_layout[i + j + 1];
                    expanded_layout[i + j + 1] = -1;
                }
            }

            id = expanded_layout[i];
            block_size = 1;
        } else {
            id = expanded_layout[i];
            block_size = 1;
        }
    }

    unsigned long long checksum = 0;
    for (size_t i = 0; i < expanded_layout.size(); ++i) {
        if (expanded_layout[i] != -1) {
            checksum += expanded_layout[i] * i;
        }
    }

    std::cout << checksum << std::endl;
}

