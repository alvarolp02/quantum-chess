#include <iostream>
#include <Eigen/Dense>

class Board {
    public:
        Board();

        // void initialize();
        // bool movePiece(int startX, int startY, int endX, int endY);

    private:
        // Define the board size
        static const int BOARD_SIZE = 8;
        // 2D array to represent the board
        Eigen::Matrix<int, BOARD_SIZE, BOARD_SIZE> board;

        // void setupPieces();
        // bool isValidMove(int startX, int startY, int endX, int endY) const;
};