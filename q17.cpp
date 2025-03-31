#include "questions.h"

#include <climits>

#include <fstream>
#include <iostream>
#include <sstream>

#include <string>
#include <vector>

bool program(std::vector<unsigned char> &output, std::vector<unsigned char> &instructions, unsigned long long registers[3]) {
    for (size_t pc = 0; pc < instructions.size();) {
        unsigned char instr = instructions[pc];
        unsigned char operand = instructions[pc + 1];

        if (output.size() > 0 && output[output.size() - 1] != instructions[output.size() - 1]) {
            return false;
        }
        if (output.size() >= 16) {
            return false;
        }

        switch (instr) {
            case 0: {
                unsigned long long denominator = operand;
                if (operand > 3) {
                    denominator = registers[operand - 4];
                }
                registers[0] = registers[0] >> denominator;
                break;
            }
            case 1: 
                registers[1] ^= operand;
                break;
            case 2: {
                unsigned long long value = operand;
                if (operand > 3) {
                    value = registers[operand - 4];
                }
                registers[1] = value % 8;
                break;
            }
            case 3: {
                if (registers[0] != 0) {
                    pc = (size_t)operand;
                    continue;
                }
                break;
            }
            case 4: 
                registers[1] ^= registers[2];
                break;
            case 5: {
                unsigned long long value = operand;
                if (operand > 3) {
                    value = registers[operand - 4];
                }
                output.push_back(value & 0b111);
                break;
            }
            case 6: {
                unsigned long long denominator = operand;
                if (operand > 3) {
                    denominator = registers[operand - 4];
                }
                registers[1] = registers[0] >> denominator;
                break;
            }
            case 7: {
                unsigned long long denominator = operand;
                if (operand > 3) {
                    denominator = registers[operand - 4];
                }
                registers[2] = registers[0] >> denominator;
                break;
            }
            default:
                return false;
        }

        pc += 2;
    }

    return true;
}

void q17(std::ifstream &input_file) {
    unsigned long long registers[3];
    std::vector<unsigned char> instructions;

    const std::string delimiter = ": ";
    std::string line;
    bool reading_registers = true;
    while (getline(input_file, line)) {
        ssize_t location = line.find(delimiter);

        if (location == std::string::npos) {
            reading_registers = false; 
            continue;
        }

        std::string value = line.substr(location + delimiter.size(), line.size() - location);
        if (!reading_registers) {
            std::stringstream reader(value);
            unsigned char c;
            while (reader >> c) {
                instructions.push_back(c - '0');
                if (reader.peek() == ',') {
                    reader.ignore();
                }
            }
        }

        if (reading_registers) {
            char reg = line[location - 1];
            registers[reg - 'A'] = std::stoull(value);
        }
    }

    for (unsigned long long r : registers) {
        std::cout << r << " ";
    }
    std::cout << std::endl;
    for (unsigned char c : instructions) {
        std::cout << (char)(c + '0') << " ";
    }
    std::cout << std::endl;


    std::vector<unsigned char> output;
    for (unsigned long long A = 0x6252C0000000; A < 0x6252FFFFFFFF; ++A) {
        output.clear();
        registers[0] = A;
        registers[1] = 0;
        registers[2] = 0;
        if (program(output, instructions, registers)) {
            std::cout << A << std::endl;
            break;
        }
        /*
        for (unsigned char c : output) {
            std::cout << (char)(c + '0') << " ";
        }
        */

        // break;
    }
}
