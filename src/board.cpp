#include "quantum_chess/board.hpp"

Board::Board() {
    // Initialize the board to be empty
    board = Eigen::Matrix<int, 8, 8>::Zero();
    board << b_rook,b_knight,b_bishop,b_queen,b_king,b_bishop,b_knight,b_rook,
             b_pawn,b_pawn,b_pawn,b_pawn,b_pawn,b_pawn,b_pawn,b_pawn,
            empty,empty,empty,empty,empty,empty,empty,empty,
            empty,empty,empty,empty,empty,empty,empty,empty,
            empty,empty,empty,empty,empty,empty,empty,empty,
            empty,empty,empty,empty,empty,empty,empty,empty,
            w_pawn,w_pawn,w_pawn,w_pawn,w_pawn,w_pawn,w_pawn,w_pawn,
            w_rook,w_knight,w_bishop,w_queen,w_king,w_bishop,w_knight,w_rook;
    std::cout << board << std::endl;
}

std::vector<std::pair<int,int>> Board::getValidMoves(int row, int col) {
    int piece = board(row, col);

    switch (piece){
        case w_pawn:
            return getValidWhitePawnMoves(row, col);
        case w_rook:
            return getValidRookMoves(row, col);
        case w_knight:
            return getValidKnightMoves(row, col);
        case w_bishop:
            return getValidBishopMoves(row, col);
        case w_queen:   
            return getValidQueenMoves(row, col);
        case w_king:
            return getValidKingMoves(row, col); 
        case b_pawn:
            return getValidBlackPawnMoves(row, col);
        case b_rook:    
            return getValidRookMoves(row, col);
        case b_knight:
            return getValidKnightMoves(row, col);
        case b_bishop:
            return getValidBishopMoves(row, col);
        case b_queen:
            return getValidQueenMoves(row, col);
        case b_king:
            return getValidKingMoves(row, col);
        default:
        std::cout << "Invalid piece" << std::endl;
            return {};
    }
}

std::vector<std::pair<int,int>> Board::getValidWhitePawnMoves(int row, int col) {
    std::vector<std::pair<int,int>> validMoves;

    if (row == 6  && board(row - 1, col) == empty && board(row - 2, col) == empty) {
        validMoves.push_back(std::make_pair(row - 2, col));
    }
    if (board(row - 1, col) == empty){
        validMoves.push_back(std::make_pair(row - 1, col));
    }

    return validMoves;
}

std::vector<std::pair<int,int>> Board::getValidBlackPawnMoves(int row, int col) {
    std::vector<std::pair<int,int>> validMoves;

    if (row == 1  && board(row + 1, col) == empty && board(row + 2, col) == empty) {
        validMoves.push_back(std::make_pair(row + 2, col));
    }
    if (board(row + 1, col) == empty){
        validMoves.push_back(std::make_pair(row + 1, col));
    }
    
    return validMoves;
}

std::vector<std::pair<int,int>> Board::getValidRookMoves(int row, int col) {
    return {};
}

std::vector<std::pair<int,int>> Board::getValidKnightMoves(int row, int col) {
    return {};
}

std::vector<std::pair<int,int>> Board::getValidBishopMoves(int row, int col) {
    return {};
}

std::vector<std::pair<int,int>> Board::getValidQueenMoves(int row, int col) {
    return {};
}

std::vector<std::pair<int,int>> Board::getValidKingMoves(int row, int col) {
    return {};
}
