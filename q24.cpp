#include "questions.h"

#include <cstdlib>
#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <bitset>
#include <format>
#include <unordered_map>

typedef struct expression_t {
    std::string a;
    std::string b;
    std::string op;
} expression;

bool solve(
    const std::unordered_map<std::string, expression> &expressions, 
    std::unordered_map<std::string, bool> &values, 
    std::string reg
) {
    if (!values.contains(reg)) {
        bool a = solve(expressions, values, expressions.at(reg).a);
        bool b = solve(expressions, values, expressions.at(reg).b);
        bool result;    
        std::string op = expressions.at(reg).op;
        if (op == "AND") {
            result = a & b;
        } else if (op == "OR") {
            result = a | b;
        } else if (op == "XOR") {
            result = a ^ b;
        } else {
            assert(false);
        }
        values[reg] = result;
    }

    return values[reg];
}

void load_values(
    std::unordered_map<std::string, bool> &values, 
    std::string name, 
    unsigned long long num, 
    size_t bits
) {
    for (size_t bit = 0; bit < bits; ++bit) {
        bool val = num & 0b1;
        values[name + std::format("{:02}", bit)] = val;
        num = num >> 1;
    }
}

void q24(std::ifstream &input_file) {
    std::string line;

    while (std::getline(input_file, line)) {
        if (line == "") {
            break;
        }

        std::string reg_name = line.substr(0, 3);
        bool reg_value = std::stoi(line.substr(5, 1)) == 1;

        // values[reg_name] = reg_value;
    }

    std::unordered_map<std::string, expression> expressions;

    std::vector<std::string> z_regs;
    while (std::getline(input_file, line)) {
        std::istringstream equation(line);
        std::string a;
        std::string op;
        std::string b;
        std::string equals;
        std::string reg;

        equation >> a;
        equation >> op;
        equation >> b;
        equation >> equals;
        equation >> reg;

        if (reg[0] == 'z') {
            z_regs.push_back(reg);
        }

        expression e{a, b, op};
        expressions[reg] = e;
    }

    for (size_t i = 0; i < 100; ++i) {
        std::unordered_map<std::string, bool> values;

        unsigned long long x = 0;
        unsigned long long y = 0;
        size_t bits = 45;
        for (size_t bit = 0; bit < bits; ++bit) {
            x = (x << 1) | std::rand() % 2;
            y = (y << 1) | std::rand() % 2;
        }

        unsigned int z = x + y;

        load_values(values, "x", x, bits);
        load_values(values, "y", y, bits);

        unsigned long long total = 0;
        for (auto &z_reg : z_regs) {
            bool value = solve(expressions, values, z_reg);
            unsigned int position = std::stoi(z_reg.substr(1, z_reg.size() - 1));
            total += (unsigned long long) value << position;
        }
        std::cout << total << std::endl;
        std::cout << std::bitset<46>(total ^ z) << std::endl;
    }
}

