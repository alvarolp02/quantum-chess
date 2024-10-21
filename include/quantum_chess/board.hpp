#include <iostream>
#include <Eigen/Dense>

#define empty 0
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

        // void initialize();
        // bool movePiece(int startX, int startY, int endX, int endY);
        std::vector<std::pair<int,int>> getValidMoves(int row, int col);

    private:
        Eigen::Matrix<int, 8, 8> board;
        std::vector<std::pair<int,int>> getValidWhitePawnMoves(int row, int col);
        std::vector<std::pair<int,int>> getValidBlackPawnMoves(int row, int col);
        std::vector<std::pair<int,int>> getValidRookMoves(int row, int col);
        std::vector<std::pair<int,int>> getValidKnightMoves(int row, int col);
        std::vector<std::pair<int,int>> getValidBishopMoves(int row, int col);
        std::vector<std::pair<int,int>> getValidQueenMoves(int row, int col);
        std::vector<std::pair<int,int>> getValidKingMoves(int row, int col);


        // void setupPieces();
        // bool isValidMove(int startX, int startY, int endX, int endY) const;
};