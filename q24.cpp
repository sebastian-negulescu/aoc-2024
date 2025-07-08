#include "questions.h"

#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
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

void q24(std::ifstream &input_file) {
    std::string line;

    std::unordered_map<std::string, bool> values;
    std::unordered_map<std::string, expression> expressions;

    while (std::getline(input_file, line)) {
        if (line == "") {
            break;
        }

        std::string reg_name = line.substr(0, 3);
        bool reg_value = std::stoi(line.substr(5, 1)) == 1;

        values[reg_name] = reg_value;
    }

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

    unsigned long long total = 0;
    for (auto &z_reg : z_regs) {
        bool value = solve(expressions, values, z_reg);
        unsigned int position = std::stoi(z_reg.substr(1, z_reg.size() - 1));
        total += (unsigned long long) value << position;
    }
    std::cout << total << std::endl;
}

