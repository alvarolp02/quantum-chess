#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <Eigen/Dense>
#include "structs.hpp"

#define gap 0
#define w_pawn 1
#define w_rook 2
#define w_knight 3
#define w_bishop 4
#define w_queen 5
#define w_king 6
#define b_pawn 7
#define b_rook 8
#define b_knight 9
#define b_bishop 10
#define b_queen 11
#define b_king 12


class Board {
    public:
        Board();

        void movePiece(Tile source, Tile target);
        std::vector<Tile> getValidMoves(Tile t);
        void printBoard();
        bool isWhite(Tile t);
        bool isBlack(Tile t);


        Eigen::Matrix<int, 8, 8> board_matrix;

    private:
        std::vector<Tile> getValidWhitePawnMoves(Tile t);
        std::vector<Tile> getValidWhiteRookMoves(Tile t);
        std::vector<Tile> getValidWhiteKnightMoves(Tile t);
        std::vector<Tile> getValidWhiteBishopMoves(Tile t);
        std::vector<Tile> getValidWhiteQueenMoves(Tile t);
        std::vector<Tile> getValidWhiteKingMoves(Tile t);
        std::vector<Tile> getValidBlackPawnMoves(Tile t);
        std::vector<Tile> getValidBlackRookMoves(Tile t);
        std::vector<Tile> getValidBlackKnightMoves(Tile t);
        std::vector<Tile> getValidBlackBishopMoves(Tile t);
        std::vector<Tile> getValidBlackQueenMoves(Tile t);
        std::vector<Tile> getValidBlackKingMoves(Tile t);


        // void setupPieces();
        // bool isValidMove(int startX, int startY, int endX, int endY) const;
};

#endif // BOARD_H