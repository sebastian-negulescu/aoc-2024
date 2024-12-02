#include "questions.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

bool is_safe(const std::vector<int> &report, size_t absent) {
    if (report.size() <= 1) {
        return true;
    }
    int sign = 0;

    int prev, cur;

    size_t start = 0;
    if (absent == 0) {
        start = 1;
    }
    prev = report[start];

    for (size_t i = start + 1; i < report.size(); ++i) {
        if (i == absent) {
            continue;
        }
        cur = report[i];

        int diff = prev - cur;
        int diff_sign = (diff > 0) ? 1 : -1;

        if (diff_sign + sign == 0) {
            return false;
        }

        size_t abs_diff = abs(diff);
        if (abs_diff < 1 || abs_diff > 3) {
            return false;
        }

        sign = diff_sign;
        prev = cur;
    }

    return true;
}

void q2(std::ifstream &input_file) {
    std::string line;
    size_t count = 0;
    while (getline(input_file, line)) {
        std::istringstream report_ss(line);
        std::vector<int> report;
        int temp;
        while (report_ss >> temp) {
            report.push_back(temp);
        }

        for (size_t i = 0; i < report.size(); ++i) {
            if (is_safe(report, i)) {
                count++;
                break;
            }
        }
    }

    std::cout << count << std::endl;
}

