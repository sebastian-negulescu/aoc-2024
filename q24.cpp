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
    std::string reg;
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

inline std::string gen_key(const std::string &reg, const std::string &op) {
    return reg + op;
}

inline std::string gen_reg(std::string base, unsigned int bit) {
    return base + std::format("{:02}", bit);
}

const std::string XOR = "XOR";
const std::string OR = "OR";
const std::string AND = "AND";
const std::string BLANK = "";


void validate_adder(
    std::unordered_map<std::string, bool> &values, 
    const std::unordered_map<std::string, expression> &expressions
) {
    size_t bit = 0;

    std::string x_base = "x";
    std::string y_base = "y";
    std::string z_base = "z";

    std::string x = gen_reg(x_base, bit);
    std::string y = gen_reg(y_base, bit);
    std::string z = gen_reg(z_base, bit);

    // half-adder validation

    expression e = expressions.at(gen_key(x, XOR));
    assert(e.reg == z);
    e = expressions.at(gen_key(x, AND));

    std::string next_reg = e.reg;

    for (bit = 1; bit < 45; ++bit) {
        x = gen_reg(x_base, bit);
        y = gen_reg(y_base, bit);
        z = gen_reg(z_base, bit);

        std::string first = expressions.at(gen_key(x, XOR)).reg;

        expression z_back = expressions.at(gen_key(z, BLANK));
        expression two = z_back;

        if (expressions.contains(gen_key(first, XOR))) {
            two = expressions.at(gen_key(first, XOR));
        }

        if (expressions.contains(gen_key(next_reg, XOR))) {
            two = expressions.at(gen_key(next_reg, XOR));
        }

        if (two.a != first && two.b != first) {
            std::cout << "swap: " << first << std::endl;
            if (two.a == next_reg) {
                first = two.b;
            } else {
                first = two.a;
            }
        }

        if (two.a != next_reg && two.b != next_reg) {
            std::cout << "swap: " << next_reg << std::endl;
            if (two.a == first) {
                next_reg = two.b;
            } else {
                next_reg = two.a;
            }
        }

        if (two.reg != z) {
            std::cout << "swap: " << two.reg << std::endl;
        }

        assert(expressions.contains(gen_key(first, AND)));
        assert(expressions.contains(gen_key(next_reg, AND)));

        std::string fourth = expressions.at(gen_key(first, AND)).reg;
        std::string third = expressions.at(gen_key(x, AND)).reg;

        expression five;

        if (expressions.contains(gen_key(fourth, OR))) {
            five = expressions.at(gen_key(fourth, OR));
        }

        if (expressions.contains(gen_key(third, OR))) {
            five = expressions.at(gen_key(third, OR));
        }

        if (five.a != fourth && five.b != fourth) {
            std::cout << "swap: " << fourth << std::endl;
            if (five.a == third) {
                fourth = five.b;
            } else {
                fourth = five.a;
            }
        }

        if (five.a != third && five.b != third) {
            std::cout << "swap: " << third << std::endl;
            if (five.a == fourth) {
                third = five.b;
            } else {
                third = five.a;
            }
        }

        next_reg = expressions.at(gen_key(fourth, OR)).reg;
    }
}

void q24(std::ifstream &input_file) {
    std::string line;

    std::unordered_map<std::string, bool> values; 

    while (std::getline(input_file, line)) {
        if (line == "") {
            break;
        }

        std::string reg_name = line.substr(0, 3);
        bool reg_value = std::stoi(line.substr(5, 1)) == 1;

        values[reg_name] = reg_value;
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

        expression e{a, b, op, reg};
        std::string a_key = gen_key(a, op);
        std::string b_key = gen_key(b, op);
        std::string reg_key = gen_key(reg, BLANK);

        assert(!expressions.contains(a_key));
        assert(!expressions.contains(b_key));
        assert(!expressions.contains(reg_key));

        expressions[a_key] = e;
        expressions[b_key] = e;
        expressions[reg_key] = e;
    }

    validate_adder(values, expressions);
}

