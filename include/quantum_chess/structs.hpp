#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <vector>

std::vector<std::string> letters = {
    {"a", "b", "c", "d", "e", "f", "g", "h"}
};

struct Tile {
    int row;
    int col;

    Tile(int rowCoord = 0, int colCoord = 0) : row(rowCoord), col(colCoord) {}

    bool operator==(const Tile& other) const {
        return (row == other.row && col == other.col);
    }

    void display() const {

        std::cout << letters[col] << 8-row << std::endl;
    }
};


#endif // TILE_H