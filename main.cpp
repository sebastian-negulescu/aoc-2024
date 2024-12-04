#include "questions.h"

#include <string>
#include <fstream>

int main(int argc, char * argv[]) {
    std::string filename = argv[1];
    std::ifstream input_file;
    input_file.open(filename);

    q3(input_file);

    input_file.close();
}

