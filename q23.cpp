#include "questions.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

std::string stringify_set(std::vector<std::string> set) {
    std::sort(set.begin(), set.end());

    std::string stringified_set = "";
    for (auto &element : set) {
        stringified_set += element;
    }

    return stringified_set;
}

void threes(
    const std::unordered_map<std::string, std::unordered_set<std::string>> &graph, 
    const std::string &starting_node,
    int depth, 
    std::string node, 
    std::vector<std::string> &set,
    std::unordered_set<std::string> &sets
) {
    if (depth == 3) {
        if (node == starting_node) {
            std::string key = stringify_set(set);
            if (!sets.contains(key)) {
                sets.insert(key);
            }
        }
        return;
    }

    for (auto &edge : graph.at(node)) {
        set.push_back(edge);
        threes(graph, starting_node, depth + 1, edge, set, sets);
        set.pop_back();
    }
}

//TODO: dynamic programming
void ner_combinations(const std::vector<std::string> &ners, int depth, std::vector<std::string> &combo, std::vector<std::vector<std::string>> &combos) {
    if (depth == ners.size()) {
        combos.emplace_back(combo);
        return;
    }

    ner_combinations(ners, depth + 1, combo, combos);
    combo.push_back(ners[depth]);
    ner_combinations(ners, depth + 1, combo, combos);
    combo.pop_back();
}

bool strongly_connected(const std::unordered_map<std::string, std::unordered_set<std::string>> &graph, const std::vector<std::string> &ners) {
    for (auto &a : ners) {
        for (auto &b : ners) {
            if (a != b && !graph.at(a).contains(b)) {
                return false;
            }
        }
    }

    return true;
}

void q23(std::ifstream &input_file) {
    std::string line;
    std::unordered_map<std::string, std::unordered_set<std::string>> graph;
    while (std::getline(input_file, line)) {
        std::string a = line.substr(0, 2);
        std::string b = line.substr(3, std::string::npos);

        if (!graph.contains(a)) {
            graph[a] = {b};
        } else {
            graph[a].insert(b);
        }
        if (!graph.contains(b)) {
            graph[b] = {a};
        } else {
            graph[b].insert(a);
        }
    }

    /*
    std::unordered_set<std::string> sets;
    for (auto &[node, edges] : graph) {
        std::vector<std::string> set = {};
        threes(graph, node, 0, node, set, sets);
    }

    unsigned int count = 0;
    for (auto &s : sets) {
        if (s[0] == 't' || s[2] == 't' || s[4] == 't') {
            count ++;
        }
    }

    std::cout << count << std::endl;
    */

    std::unordered_set<std::string> sets;
    std::string largest_combo;
    for (auto &[node, edges] : graph) {
        std::vector<std::string> ners;
        for (auto &ner : edges) {
            ners.push_back(ner);
        }
        std::vector<std::vector<std::string>> combos;
        std::vector<std::string> combo = {};
        ner_combinations(ners, 0, combo, combos);
        
        for (auto &c : combos) {
            if (c.size() + 1 > largest_combo.size() / 2) {
                if (strongly_connected(graph, c)) {
                    c.push_back(node);
                    largest_combo = stringify_set(c); 
                }
            }
        }
    }

    for (size_t i = 0; i < largest_combo.size(); ++i) {
        std::cout << largest_combo[i];
        if (i % 2 == 1 && i != largest_combo.size() - 1) {
            std::cout << ',';
        }
    }

    std::cout << std::endl;
}

