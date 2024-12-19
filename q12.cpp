#include "questions.h"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <list>

struct plot {
    char plant_type;
    unsigned int perimiter;
    unsigned int area;
    unsigned long long sides;
};

enum states {
    EMPTY,
    BLOCKED,
    FLUSH
};

void flood_fill(
    plot &p, 
    const std::vector< std::vector<char> > &garden,
    std::vector< std::vector<bool> > &garden_ghost,
    size_t row,
    size_t col
) {
    garden_ghost[row][col] = true;

    p.area++;
    std::vector<states> directions(4, EMPTY);

    /*
     * What do we do about the sides?
     */

    unsigned int num_ners = 0;
    if (row > 0 && garden[row - 1][col] == p.plant_type && garden_ghost[row - 1][col]) {
        num_ners++;
        p.perimiter -= 2;

        p.sides--;
        directions[0] = BLOCKED;

        if (!(col > 0 && garden[row - 1][col - 1] == p.plant_type && garden_ghost[row - 1][col - 1])) {
            if (directions[3] != BLOCKED) {
                directions[3] = FLUSH;
            }
        }
        if (!(col < garden[row - 1].size() - 1 && garden[row - 1][col + 1] == p.plant_type && garden_ghost[row - 1][col + 1])) {
            if (directions[1] != BLOCKED) {
                directions[1] = FLUSH;
            }
        }
    }
    if (row < garden.size() - 1 && garden[row + 1][col] == p.plant_type && garden_ghost[row + 1][col]) {
        num_ners++;
        p.perimiter -= 2;

        p.sides--;
        directions[2] = BLOCKED;

        if (!(col > 0 && garden[row + 1][col - 1] == p.plant_type && garden_ghost[row + 1][col - 1])) {
            if (directions[3] != BLOCKED) {
                directions[3] = FLUSH;
            }
        }
        if (!(col < garden[row + 1].size() - 1 && garden[row + 1][col + 1] == p.plant_type && garden_ghost[row + 1][col + 1])) {
            if (directions[1] != BLOCKED) {
                directions[1] = FLUSH;
            }
        }
    }
    if (col > 0 && garden[row][col - 1] == p.plant_type && garden_ghost[row][col - 1]) {
        num_ners++;
        p.perimiter -= 2;

        p.sides--;
        directions[3] = BLOCKED;

        if (!(row > 0 && garden[row - 1][col - 1] == p.plant_type && garden_ghost[row - 1][col - 1])) {
            if (directions[0] != BLOCKED) {
                directions[0] = FLUSH;
            }
        }
        if (!(row < garden.size() - 1 && garden[row + 1][col - 1] == p.plant_type && garden_ghost[row + 1][col - 1])) {
            if (directions[2] != BLOCKED) {
                directions[2] = FLUSH;
            }
        }
    }
    if (col < garden[row].size() - 1 && garden[row][col + 1] == p.plant_type && garden_ghost[row][col + 1]) {
        num_ners++;
        p.perimiter -= 2;

        p.sides--;
        directions[1] = BLOCKED;

        if (!(row > 0 && garden[row - 1][col + 1] == p.plant_type && garden_ghost[row - 1][col + 1])) {
            if (directions[0] != BLOCKED) {
                directions[0] = FLUSH;
            }
        }
        if (!(row < garden.size() - 1 && garden[row + 1][col + 1] == p.plant_type && garden_ghost[row + 1][col + 1])) {
            if (directions[2] != BLOCKED) {
                directions[2] = FLUSH;
            }
        }
    }

    std::cout << p.plant_type << std::endl;
    for (size_t i = 0; i < directions.size(); ++i) {
        std::cout << directions[i] << " ";
        unsigned int added_sides = 0;
        if (directions[i] == EMPTY) {
            p.sides++;
            std::cout << "prev " << directions[(i + directions.size() - 1) % 4] << " next " << directions[(i + 1) % 4] << std::endl;
            if (directions[(i + directions.size() - 1) % 4] == BLOCKED) {
                // std::cout << "prev" << std::endl;
                added_sides++;
            }
            if (directions[(i + 1) % 4] == BLOCKED) {
                // std::cout << "next" << std::endl;
                added_sides++;
            }
        /*
        } else {
        */
        }
        p.sides += (added_sides + 1) / 2;
    }
    std::cout << std::endl;

    for (const auto &line : garden_ghost) {
        for (const auto &filled : line) {
            if (filled) {
                std::cout << "X";
            } else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }
    std::cout << p.sides << std::endl;

    // up
    if (row > 0 && garden[row - 1][col] == p.plant_type && !garden_ghost[row - 1][col]) {
        flood_fill(p, garden, garden_ghost, row - 1, col);
    }

    // left
    if (col < garden[row].size() - 1 && garden[row][col + 1] == p.plant_type && !garden_ghost[row][col + 1]) {
        flood_fill(p, garden, garden_ghost, row, col + 1);
    }

    // down
    if (row < garden.size() - 1 && garden[row + 1][col] == p.plant_type && !garden_ghost[row + 1][col]) {
        flood_fill(p, garden, garden_ghost, row + 1, col);
    }
    
    // right
    if (col > 0 && garden[row][col - 1] == p.plant_type && !garden_ghost[row][col - 1]) {
        flood_fill(p, garden, garden_ghost, row, col - 1);
    }
    
}

void q12(std::ifstream &input_file) {
    std::vector< std::vector<char> > garden;
    std::vector< std::vector<bool> > garden_ghost;

    std::string line;
    size_t row = 0;
    while (getline(input_file, line)) {
        std::istringstream plant_reader(line);        
        std::vector<char> garden_line;

        char plant_type;
        size_t col = 0;
        while (plant_reader >> plant_type) {
            // plant_type is defined here
            garden_line.push_back(plant_type);

        }
        garden.push_back(garden_line);
        std::vector<bool> ghost_line(garden_line.size(), false);
        garden_ghost.push_back(ghost_line);
    }

    // implement a flood fill
    std::list<plot> plots;
    for (size_t i = 0; i < garden.size(); ++i) {
        for (size_t j = 0; j < garden[i].size(); ++j) {
            if (!garden_ghost[i][j]) {
                // flood fill here
                plot p;
                p.plant_type = garden[i][j];
                p.perimiter = 0;
                p.area = 0;
                p.sides = 0;

                flood_fill(p, garden, garden_ghost, i, j);
                plots.push_back(p);
            }
        }
    }

    unsigned long long total_price = 0;
    std::cout << plots.size() << std::endl;
    for (const auto &p : plots) {
        total_price += p.area * p.sides;
        std::cout << p.plant_type << " " << p.area << " " << p.sides << std::endl;
    }

    std::cout << total_price << std::endl;
}

