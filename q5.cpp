#include "questions.h"

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <vector>

void q5(std::ifstream &input_file) {
    std::string line;

    std::unordered_map<unsigned int, std::unordered_set<unsigned int>> page_orderings;

    while (getline(input_file, line)) {
        if (line == "") {
            break;
        }

        std::istringstream parser(line);

        unsigned int a, b;
        char c;
        parser >> a >> c >> b;

        if (!page_orderings.contains(a)) {
            page_orderings[a] = std::unordered_set<unsigned int>();
        }
        page_orderings[a].insert(b);
    }

    unsigned int sum = 0;
    while (getline(input_file, line)) {
        std::istringstream parser(line);

        std::vector<unsigned int> pages;

        unsigned int a;
        while (parser >> a) {
            pages.push_back(a); 
            
            char c;
            if (!(parser >> c)) {
                break;
            }
        }

        bool modified = false;
        size_t invalid = 0;
        size_t i = 0;
        while (i < pages.size()) {
            invalid = 0;
            size_t j = i + 1;
            while (j < pages.size()) {
                if (page_orderings[pages[j]].contains(pages[i])) {
                    std::cout << j << std::endl;
                    invalid = j;
                    break;
                }
                j++;
            }

            if (invalid > 0) {
                // move invalid up to i, retry
                unsigned int temp = pages[invalid];
                pages.erase(pages.begin() + invalid);
                pages.insert(pages.begin() + i, temp);
                modified = true;
            } else {
                i++;
            }
        }

        if (modified) {
            sum += pages[pages.size() / 2];
        }

        for (auto el : pages) {
            std::cout << el << " ";
        }
        std::cout << std::endl;
    }

    std::cout << sum << std::endl;
}

