#include "questions.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>

enum object {
    EMPTY,
    ROBOT,
    WALL,
    BOX_LEFT,
    BOX_RIGHT
};

enum move {
    UP,
    RIGHT,
    DOWN,
    LEFT 
};

void commit_move(
    std::vector<std::vector<object>> &warehouse,
    std::pair<size_t, size_t> &position,
    move m
) {
    std::cout << m << " " << position.first << " " << position.second << std::endl;

    std::pair<size_t, size_t> target_position = position;
    switch (m) {
        case UP:
            target_position.first--;
            break;
        case LEFT:
            target_position.second--;
            break;
        case DOWN:
            target_position.first++;
            break;
        case RIGHT:
            target_position.second++;
            break;
    }

    switch (warehouse[target_position.first][target_position.second]) {
        case EMPTY:
            warehouse[target_position.first][target_position.second] = 
                warehouse[position.first][position.second];
            warehouse[position.first][position.second] = EMPTY;
            break;
        case ROBOT:
            commit_move(warehouse, target_position, m);
            warehouse[target_position.first][target_position.second] = 
                warehouse[position.first][position.second];
            warehouse[position.first][position.second] = EMPTY;
            break;
        case BOX_LEFT: {
            if (m == UP || m == DOWN) {
                std::pair<size_t, size_t> alt_target_position = target_position;
                alt_target_position.second++;
                commit_move(warehouse, alt_target_position, m);
            }

            if (m == RIGHT) {
                target_position.second++;
            }
            commit_move(warehouse, target_position, m);
            if (m == RIGHT) {
                target_position.second--;
                warehouse[target_position.first][target_position.second + 1] = 
                    warehouse[position.first][position.second + 1];
                warehouse[position.first][position.second + 1] = EMPTY;
            }

            warehouse[target_position.first][target_position.second] = 
                warehouse[position.first][position.second];
            warehouse[position.first][position.second] = EMPTY;

            if (m == LEFT) {
                warehouse[target_position.first][target_position.second + 1] =
                    warehouse[position.first][position.second + 1];
                warehouse[position.first][position.second + 1] = EMPTY;
            } else if (m != RIGHT) {
                warehouse[target_position.first][target_position.second + 1] = EMPTY;
                //    warehouse[position.first][position.second - 1];
                // warehouse[position.first][position.second - 1] = EMPTY;
            }
            break;
        }
        case BOX_RIGHT: {
            if (m == UP || m == DOWN) {
                std::pair<size_t, size_t> alt_target_position = target_position;
                alt_target_position.second--;
                commit_move(warehouse, alt_target_position, m);
            }

            if (m == LEFT) {
                target_position.second--;
            }
            commit_move(warehouse, target_position, m);
            if (m == LEFT) {
                target_position.second++;
                warehouse[target_position.first][target_position.second - 1] = 
                    warehouse[position.first][position.second - 1];
                warehouse[position.first][position.second - 1] = EMPTY;
            }

            warehouse[target_position.first][target_position.second] = 
                warehouse[position.first][position.second];
            warehouse[position.first][position.second] = EMPTY;

            if (m == RIGHT) {
                warehouse[target_position.first][target_position.second - 1] =
                    warehouse[position.first][position.second - 1];
                warehouse[position.first][position.second - 1] = EMPTY;
            } else if (m != LEFT) {
                warehouse[target_position.first][target_position.second - 1] = EMPTY;
                //    warehouse[position.first][position.second - 1];
                // warehouse[position.first][position.second - 1] = EMPTY;
            }
            break;
        }
        default: break;
    }


}

bool check_move(
    std::vector<std::vector<object>> &warehouse,
    std::pair<size_t, size_t> &position,
    move m
) {
    std::pair<size_t, size_t> target_position = position;
    switch (m) {
        case UP:
            target_position.first--;
            break;
        case LEFT:
            target_position.second--;
            break;
        case DOWN:
            target_position.first++;
            break;
        case RIGHT:
            target_position.second++;
            break;
    }

    switch (warehouse[target_position.first][target_position.second]) {
        case EMPTY: break;
        case WALL: return false;
        case BOX_LEFT:
            if (m == UP || m == DOWN) {
                std::pair<size_t, size_t> alt_target_position = target_position;
                alt_target_position.second++;
                return check_move(warehouse, target_position, m) && 
                    check_move(warehouse, alt_target_position, m);
            } else if (m == LEFT) {
                // target_position.second--;
                return check_move(warehouse, target_position, m);
            } else if (m == RIGHT) {
                target_position.second++;
                return check_move(warehouse, target_position, m);
            }
            break;
        case BOX_RIGHT:
            if (m == UP || m == DOWN) {
                std::pair<size_t, size_t> alt_target_position = target_position;
                alt_target_position.second--;
                return check_move(warehouse, target_position, m) && 
                    check_move(warehouse, alt_target_position, m);
            } else if (m == LEFT) {
                target_position.second--;
                return check_move(warehouse, target_position, m);
            } else if (m == RIGHT) {
                // target_position.second++;
                return check_move(warehouse, target_position, m);
            }
            break;
        default: break;
    }

    return true;
}

bool make_move(
    std::vector<std::vector<object>> &warehouse, 
    std::pair<size_t, size_t> &position, 
    move m
) {
    std::pair<size_t, size_t> target_position = position;
    switch (m) {
        case UP:
            target_position.first--;
            break;
        case LEFT:
            target_position.second--;
            break;
        case DOWN:
            target_position.first++;
            break;
        case RIGHT:
            target_position.second++;
            break;
    }

    if (!check_move(warehouse, position, m)) {
        return false; 
    }

    std::cout << "committing..." << std::endl;

    // make the move
    commit_move(warehouse, position, m);
    position = target_position;

    return true;
}

void q15(std::ifstream &input_file) {
    std::vector<std::vector<object>> warehouse; 
    std::list<move> moves;

    std::string line;
    std::pair<size_t, size_t> robot_position;
    while (getline(input_file, line)) {
        if (line == "") {
            break;
        }

        std::vector<object> warehouse_line;
        char object_c;
        std::istringstream reader(line);
        while (reader >> object_c) {
            switch (object_c) {
                case '.':
                    warehouse_line.push_back(EMPTY);
                    warehouse_line.push_back(EMPTY);
                    break;
                case '@':
                    robot_position.first = warehouse.size();
                    robot_position.second = warehouse_line.size();
                    warehouse_line.push_back(ROBOT);
                    warehouse_line.push_back(EMPTY);
                    break;
                case '#':
                    warehouse_line.push_back(WALL);
                    warehouse_line.push_back(WALL);
                    break;
                case 'O':
                    warehouse_line.push_back(BOX_LEFT);
                    warehouse_line.push_back(BOX_RIGHT);
                    break;
            }
        }

        warehouse.push_back(warehouse_line);
    }

    std::cout << robot_position.first << " " << robot_position.second << std::endl;

    while (getline(input_file, line)) {
        char move_c;
        std::istringstream reader(line);
        while (reader >> move_c) {
            switch (move_c) {
                case '^':
                    moves.push_back(UP);
                    break;
                case '>':
                    moves.push_back(RIGHT);
                    break;
                case 'v':
                    moves.push_back(DOWN);
                    break;
                case '<':
                    moves.push_back(LEFT);
                    break;
            }
        }
    }

    for (const auto &move : moves) {
        std::cout << move << std::endl;
        make_move(warehouse, robot_position, move);

        for (const auto &warehouse_line : warehouse) {
            for (const auto &o : warehouse_line) {
                switch (o) {
                    case EMPTY: std::cout << '.'; break;
                    case ROBOT: std::cout << '@'; break;
                    case WALL: std::cout << '#'; break;
                    case BOX_LEFT: std::cout << '['; break;
                    case BOX_RIGHT: std::cout << ']'; break;
                }
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    unsigned long long gps_sum = 0;
    for (size_t row = 0; row < warehouse.size(); ++row) {
        for (size_t col = 0; col < warehouse[row].size(); ++col) {
            switch(warehouse[row][col]) {
                case BOX_LEFT:
                    gps_sum += 100 * row + col;
                    break;
                default: break;
            }
        }
    }

    std::cout << gps_sum << std::endl;
}

