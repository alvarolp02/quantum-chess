#pragma once

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
        return letters[col] + std::to_string(row);
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

enum GameState {
    Playing,
    WhiteWins,
    BlackWins,
    Draw
};

enum GamePlayer {
    Human,
    Bot_AlphaBeta,
    Bot_MCTS
};

bool is_bot(GamePlayer player) {
    return player == Bot_AlphaBeta || player == Bot_MCTS;
}

std::string string(GameState state) {
    switch (state) {
        case Playing: return "playing";
        case WhiteWins: return "white_wins";
        case BlackWins: return "black_wins";
        case Draw: return "draw";
        default: return "unknown";
    }
}