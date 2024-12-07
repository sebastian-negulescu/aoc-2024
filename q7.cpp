#include "questions.h"

#include <list>
#include <string>
#include <sstream>
#include <iostream>

unsigned long long evaluate(
    const unsigned long long final_val, 
    const unsigned long long partial_val,
    std::list<unsigned long long> &tokens
) {
    if (tokens.empty()) {
        return final_val == partial_val;
    }

    // pop the first token off of tokens
    const unsigned long long token = tokens.front();
    tokens.pop_front();

    const unsigned long long sum = evaluate(final_val, partial_val + token, tokens);
    const unsigned long long mult = evaluate(final_val, partial_val * token, tokens);
    const unsigned long long conc = evaluate(final_val, std::stoull(std::to_string(partial_val) + std::to_string(token)), tokens);

    tokens.push_front(token);

    return sum + mult + conc;
}

void q7(std::ifstream &input_file) {
    std::string line;
    unsigned long long total_val = 0;
    while (getline(input_file, line)) {
        std::cout << line << std::endl;
        std::istringstream equation(line);

        unsigned long long final_val;
        equation >> final_val;

        char c;
        equation >> c;

        unsigned long long initial_val;
        equation >> initial_val;

        unsigned long long token;
        std::list<unsigned long long> tokens;
        while (equation >> token) {
            tokens.push_back(token);
        }

        const unsigned long long count = evaluate(final_val, initial_val, tokens);
        if (count > 0) {
            total_val += final_val;
            std::cout << final_val << std::endl;
            std::cout << count << std::endl << std::endl;
        }
    }

    std::cout << total_val << std::endl;
}

