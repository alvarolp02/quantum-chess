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
    std::vector<int> depths = {};
    std::vector<Tile> pieces = {};

    Split(std::vector<int> depths, std::vector<Tile> pieces) : depths(depths), pieces(pieces) {}

    Split() {
        depths = {};
        pieces = {};
    }

    bool operator==(const Split& other) const {
        return (depths == other.depths && pieces == other.pieces);
    }
};


#endif // TILE_H