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

    std::string to_string(){
        return letters[col] + std::to_string(8-row);
    }
};

struct Split {
    int depth;
    Tile piece;

    bool operator==(const Split& other) const {
        return (depth == other.depth && piece == other.piece);
    }
};


#endif // TILE_H