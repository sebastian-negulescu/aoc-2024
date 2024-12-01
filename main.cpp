#include "questions.h"

#include <iostream>
#include <fstream>

int main() {
    std::cout << "hello, world!" << std::endl;

    std::ifstream input_file;
    input_file.open("input.txt");

    q1(input_file);

    input_file.close();
}

