#include "questions.h"

#include <generator>
#include <string>
#include <optional>
#include <iostream>
#include <cstddef>

std::optional<unsigned int> char_to_digit(char c) {  
    std::optional<unsigned int> ret;
    if (c >= '0' && c <= '9') {
        return c - '0';
    }

    return {};
}

void q3(std::ifstream &input_file) {
    const std::string mul_sequence("mul(*,*)");
    std::size_t mul_sequence_ind = 0;

    const std::string do_sequence("do()");
    std::size_t do_sequence_ind = 0;

    const std::string dont_sequence("don't()");
    std::size_t dont_sequence_ind = 0;

    unsigned int a, b;
    unsigned int num;
    size_t digits;
    unsigned int sum = 0;

    bool enabled = true;

    char c;
    bool has_char = true;
    while (true) {
        input_file >> c;
        has_char = input_file.peek() != EOF;

        while (true) {
            bool fresh = false;

            while (true) {
                if (!has_char) {
                    std::cout << std::endl << sum << std::endl;
                    return;
                }

                if (do_sequence[do_sequence_ind] != c) {
                    do_sequence_ind = 0;
                    break; 
                } else {
                    do_sequence_ind++;
                }

                if (do_sequence_ind <= 2) {
                    dont_sequence_ind++;
                } else {
                    dont_sequence_ind = 0;
                }
                
                if (do_sequence_ind >= do_sequence.size()) {
                    enabled = true;
                    do_sequence_ind = 0;
                }

                input_file >> c;
                has_char = input_file.peek() != EOF;
                fresh = true;
            }

            while (true) {
                if (!has_char) {
                    std::cout << std::endl << sum << std::endl;
                    return;
                }

                if (dont_sequence[dont_sequence_ind] != c) {
                    dont_sequence_ind = 0;
                    break;
                } else {
                    dont_sequence_ind++;
                }

                if (dont_sequence_ind >= dont_sequence.size()) {
                    enabled = false;
                    dont_sequence_ind = 0;
                }

                input_file >> c;
                has_char = input_file.peek() != EOF;
                fresh = true;
            }
            
            num = 0;
            digits = 0;
            while (enabled) {
                if (!has_char) {
                    std::cout << std::endl << sum << std::endl;
                    return;
                }

                std::cout << c;
                if (mul_sequence[mul_sequence_ind] != '*' && mul_sequence[mul_sequence_ind] != c) {
                    mul_sequence_ind = 0;
                    break;
                }

                if (mul_sequence[mul_sequence_ind] == '*') {
                    // parsing number
                    auto digit = char_to_digit(c);
                    if (digit.has_value()) {
                        num *= 10;
                        num += *digit;
                        digits++;
                    } else if (mul_sequence_ind == 4 && c == ',') {
                        if (digits == 0 || digits > 3) {
                            mul_sequence_ind = 0;
                            break;
                        }
                        a = num;
                        num = 0;
                        digits = 0;
                        mul_sequence_ind += 2;
                    } else if (mul_sequence_ind == 6 && c == ')') {
                        if (digits == 0 || digits > 3) {
                            mul_sequence_ind = 0;
                            break;
                        }
                        b = num;
                        std::cout << a << " " << b << std::endl;
                        sum += a * b; 
                        mul_sequence_ind = 0;
                        break;
                    } else {
                        mul_sequence_ind = 0;
                        break;
                    }
                } else {
                    mul_sequence_ind++;
                }

                input_file >> c;
                has_char = input_file.peek() != EOF;
                fresh = true;
            }

            if (!fresh) {
                break;
            }
        }
    }
}

