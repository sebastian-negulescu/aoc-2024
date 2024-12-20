#include "questions.h"

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

struct robot {
    std::pair<ssize_t, ssize_t> room;
    std::pair<int, int> position;
    std::pair<int, int> velocity;

    void pass_time(const unsigned int t) {
        std::pair<int, int> delta = std::make_pair(velocity.first * t, velocity.second * t);

        position.first += delta.first;
        position.second += delta.second;

        position.first %= room.first;
        if (position.first < 0) {
            position.first += room.first;
        }
        position.second %= room.second;
        if (position.second < 0) {
            position.second += room.second;
        }
    }
};

void q14(std::ifstream &input_file) {
    const ssize_t width = 101;
    const ssize_t lower_width = width / 2;
    const ssize_t height = 103;
    const ssize_t lower_height = height / 2;
    std::pair room = std::make_pair(width, height);

    std::vector<std::string> room_board;
    for (size_t i = 0; i < height; ++i) {
        std::string room_line(width, '.'); 
        room_board.push_back(room_line);
    }

    std::vector<robot> robots;
    std::vector<unsigned int> quadrants(4, 0);

    std::string line;
    while (getline(input_file, line)) {
        std::istringstream reader(line);

        char dummy;
        unsigned int px, py;
        int vx, vy;

        reader >> dummy >> dummy;
        reader >> px >> dummy >> py;
        reader >> dummy >> dummy;
        reader >> vx >> dummy >> vy;

        // std::cout << px << " " << py << std::endl;
        // std::cout << vx << " " << vy << std::endl;

        robot r;
        r.room = room;
        r.position = std::make_pair(px, py);        
        r.velocity = std::make_pair(vx, vy);

        // r.pass_time(100);

        /*
        if (r.position.first < lower_width) {
            if (r.position.second < lower_height) {
                quadrants[0]++;
            } else if (r.position.second > lower_height) {
                quadrants[1]++;
            }
        } else if (r.position.first > lower_width) {
            if (r.position.second < lower_height) {
                quadrants[2]++;
            } else if (r.position.second > lower_height) {
                quadrants[3]++;
            }
        }
        */

        robots.push_back(r);
    }

    /*
    unsigned long long safety_factor = 1;
    for (const auto &quadrant : quadrants) {
        std::cout << quadrant << std::endl;
        safety_factor *= quadrant;
    }

    std::cout << safety_factor << std::endl;
    */
    for (auto &r : robots) {
        r.pass_time(12);
        // room_board[r.position.second][r.position.first] = 'X';
    }


    unsigned int epoch = 0;
    while (true) {
        std::getchar();
        for (auto &board_line : room_board) {
            for (size_t i = 0; i < board_line.size(); ++i) {
                board_line[i] = '.';
            }
        }

        for (auto &r : robots) {
            r.pass_time(103);
            room_board[r.position.second][r.position.first] = 'X';
        }

        epoch++;
        std::cout << epoch << std::endl;
        for (const auto &board_line : room_board) {
            std::cout << board_line << std::endl;
        }
        std::cout << std::endl;
    }
}

