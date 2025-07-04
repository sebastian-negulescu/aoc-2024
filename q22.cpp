#include "questions.h"

#include <numeric>
#include <assert.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

unsigned int mix(unsigned int m, unsigned int n) {
    return m ^ n;
}

unsigned int prune(unsigned int n) {
    return n % 16777216;
}

unsigned int next_secret(unsigned int secret) {
    unsigned int s_1 = prune(mix(secret << 6, secret));
    unsigned int s_2 = prune(mix(s_1 >> 5, s_1));
    unsigned int s_3 = prune(mix(s_2 << 11, s_2));
    return s_3;
}

bool add_diff(std::vector<int> &sequence, int diff) {
    assert(sequence.size() > 0);
    bool primed = true;
    for (size_t i = 1; i < sequence.size(); ++i) {
        sequence[i - 1] = sequence[i];
        if (sequence[i] == -10) {
            primed = false;
        }
    }
    sequence[sequence.size() - 1] = diff;
    return primed;
}

std::string sequence_stringified(std::vector<int> &sequence) {
    std::string seq = "";
    for (size_t i = 0; i < sequence.size(); ++i) {
        if (sequence[i] > 0) {
            seq += " ";
        } else {
            seq += "-";
        }
        seq += std::to_string(abs(sequence[i]));
        if (i != sequence.size() - 1) {
            seq += ",";
        }
    }

    return seq;
}

void q22(std::ifstream &input_file) {
    std::string line;
    std::vector<std::vector<unsigned int>> prices;
    std::vector<std::vector<int>> diffs;
    while (std::getline(input_file, line)) {
        std::vector<unsigned int> monkey_prices;
        std::vector<int> monkey_diffs;

        unsigned int secret = std::stoi(line);
        unsigned int price = secret % 10;
        monkey_prices.push_back(price);

        for (size_t i = 0; i < 2000; ++i) {
            secret = next_secret(secret);
            price = secret % 10;
            int diff = price - monkey_prices[monkey_prices.size() - 1];
            monkey_prices.push_back(price);
            monkey_diffs.push_back(diff);
        }
        prices.push_back(monkey_prices);
        diffs.push_back(monkey_diffs);
    }

    std::unordered_map<std::string, std::vector<unsigned int>> unioned_sequences;
    std::vector<std::unordered_map<std::string, unsigned int>> unique_sequences; // don't need this map but it's useful :)
    for (size_t i = 0; i < diffs.size(); ++i) {
        const std::vector<int> &monkey_diff = diffs[i];
        const std::vector<unsigned int> &monkey_price = prices[i];
        std::unordered_map<std::string, unsigned int> monkey_unique_sequences;
        std::vector<int> sequence = {-10, -10, -10, -10};
        for (size_t j = 0; j < monkey_diff.size(); ++j) {
            int diff = monkey_diff[j];
            // std::cout << diff << " " << monkey_price[j + 1] << std::endl;
            if (add_diff(sequence, diff)) {
                std::string sequence_key = sequence_stringified(sequence);
                if (!monkey_unique_sequences.contains(sequence_key)) {
                    // std::cout << sequence_key << std::endl;
                    monkey_unique_sequences[sequence_key] = monkey_price[j + 1];
                    if (!unioned_sequences.contains(sequence_key)) {
                        unioned_sequences[sequence_key] = std::vector<unsigned int>(prices.size());
                    }
                    unioned_sequences[sequence_key][i] = monkey_price[j + 1];
                }
            }
        }
        unique_sequences.push_back(monkey_unique_sequences);
    }

    unsigned int largest_price = 0;
    for (const auto &item : unioned_sequences) {
        unsigned int total_price = std::reduce(item.second.begin(), item.second.end());
        if (total_price > largest_price) {
            largest_price = total_price;
        }
    }

    std::cout << largest_price << std::endl;
}
