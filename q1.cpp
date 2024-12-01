#include "questions.h"

#include <algorithm>
#include <iostream>
#include <vector>

void q1(std::ifstream &input_file) {
    int l, r;
    std::vector<int> left, right;
    while (input_file >> l >> r) {
        left.push_back(l);
        right.push_back(r);
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    int diff_sum = 0;
    for (size_t i = 0; i < left.size(); ++i) {
        diff_sum += abs(left[i] - right[i]);
    }

    std::cout << diff_sum << std::endl;

    int sim_sum = 0;
    size_t r_ind = 0;
    for (size_t i = 0; i < left.size(); ++i) {
        size_t count = 0;
        while (right[r_ind] <= left[i] && r_ind < right.size()) {
            if (right[r_ind] == left[i]) {
                count++;
            }
            r_ind++;
        }
        sim_sum += count * left[i];
    }

    std::cout << sim_sum << std::endl;
}

