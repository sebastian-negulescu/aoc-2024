#include "questions.h"

#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <iomanip>

enum line_type {
    A,
    B,
    PRIZE,
    BLANK
};

line_type advance(line_type t) {
    if (t == A) {
        return B;
    }
    if (t == B) {
        return PRIZE;
    }
    if (t == PRIZE) {
        return BLANK;
    }
    if (t == BLANK) {
        return A;
    }
    return BLANK;
}

void q13(std::ifstream &input_file) {
    std::string line; 

    std::pair<unsigned int, unsigned int> a;
    std::pair<unsigned int, unsigned int> b;
    std::pair<unsigned long long, unsigned long long> prize;

    const float delta = 0.0001;

    unsigned long long total_tokens = 0;

    line_type t = A;
    while (getline(input_file, line)) {
        std::istringstream line_reader(line);
        char dummy;
        switch (t) {
            case A:
                for (size_t i = 0; i < 10; ++i) {
                    line_reader >> dummy;
                }
                line_reader >> a.first >> dummy >> dummy >> dummy >> a.second;
                break;
            case B:
                for (size_t i = 0; i < 10; ++i) {
                    line_reader >> dummy;
                }
                line_reader >> b.first >> dummy >> dummy >> dummy >> b.second;
                break;
            case PRIZE:
                for (size_t i = 0; i < 8; ++i) {
                    line_reader >> dummy;
                }
                line_reader >> prize.first >> dummy >> dummy >> dummy >> prize.second;
                // prize.first += 10000000000000;
                // prize.second += 10000000000000;
                break;
            default: break;
        }

        if (t == BLANK) {
            // calculate
            std::cout << a.first << " " << a.second << std::endl;
            std::cout << b.first << " " << b.second << std::endl;
            std::cout << prize.first << " " << prize.second << std::endl;
            double ax = (double)a.first;
            double ay = (double)a.second;

            double bx = (double)b.first;
            double by = (double)b.second;

            double prizex = (double)prize.first;
            double prizey = (double)prize.second;

            // double n = (prizey - (prizex * ay) / ax) / (by - (ay * bx) / ax);
            // double m = (prizey - (prizex * by) / bx) / (ay - (by * ax) / bx);
            double n = (prizey * ax - prizex * ay) / (by * ax - ay * bx);
            double m = (prizey * bx - prizex * by) / (ay * bx - by * ax);

            if (std::abs(std::round(m) - m) < delta && std::abs(std::round(n) - n) < delta) {
                if (m >= 0 && n >= 0) {
                    unsigned long long im = (unsigned long long)std::round(m);
                    unsigned long long in = (unsigned long long)std::round(n);

                    std::cout << "yes " << im << " " << in << std::endl;
                    std::cout << 3 * im + in << std::endl;
                    total_tokens += 3 * im + in;
                }
            }
        }

        t = advance(t);
    }

    std::cout << total_tokens << std::endl;
}

