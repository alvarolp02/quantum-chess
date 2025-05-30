#pragma once

#include <iostream>
#include <Eigen/Dense>
#include "structs.hpp"
#include "utils.hpp"
#include "PSQT.hpp"
#include <map>

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

        Eigen::MatrixXi board_matrix;
        int N_ROWS;
        int N_COLS;
        bool PESTO = false;

        std::map<int, double> piece_values = {
            {w_pawn, 1.0},
            {w_rook, 5.0},
            {w_knight, 3.0},
            {w_bishop, 3.0},
            {w_queen, 9.0},
            {w_king, 100.0},
            {b_pawn, -1.0},
            {b_rook, -5.0},
            {b_knight, -3.0},
            {b_bishop, -3.0},
            {b_queen, -9.0},
            {b_king, -100.0}
        };

        std::map<int, double> pesto_mid_piece_values = {
            {w_pawn, 82.0},
            {w_rook, 477.0},
            {w_knight, 337.0},
            {w_bishop, 365.0},
            {w_queen, 1025.0},
            {w_king, 10000.0},
            {b_pawn, -82.0},
            {b_rook, -477.0},
            {b_knight, -337.0},
            {b_bishop, -365.0},
            {b_queen, -1025.0},
            {b_king, -10000.0}
        };

        std::map<int, double> pesto_end_piece_values = {
            {w_pawn, 94.0},
            {w_rook, 512.0},
            {w_knight, 281.0},
            {w_bishop, 297.0},
            {w_queen, 936.0},
            {w_king, 10000.0},
            {b_pawn, -94.0},
            {b_rook, -512.0},
            {b_knight, -281.0},
            {b_bishop, -297.0},
            {b_queen, -936.0},
            {b_king, -10000.0}
        };


        std::map<int, Eigen::MatrixXi> mid_PSQT;
        
        std::map<int, Eigen::MatrixXi> end_PSQT;

        Board() {
            // Initialize the board to be gap
            N_ROWS = 8;
            N_COLS = 8;
            board_matrix = Eigen::MatrixXi::Zero(8, 8);
            board_matrix << b_rook,b_knight,b_bishop,b_queen,b_king,b_bishop,b_knight,b_rook,
                            b_pawn, b_pawn, b_pawn, b_pawn, b_pawn, b_pawn, b_pawn, b_pawn,
                            gap,    gap,    gap,    gap,    gap,    gap,    gap,    gap,
                            gap,    gap,    gap,    gap,    gap,    gap,    gap,    gap,
                            gap,    gap,    gap,    gap,    gap,    gap,    gap,    gap,
                            gap,    gap,    gap,    gap,    gap,    gap,    gap,    gap,
                            w_pawn, w_pawn, w_pawn, w_pawn, w_pawn, w_pawn, w_pawn, w_pawn,
                            w_rook,w_knight,w_bishop,w_queen,w_king,w_bishop,w_knight,w_rook;
            PESTO = true;
            mid_PSQT = PSQT::get_mid_PSQT(N_ROWS, N_COLS);
        }

        Board(Eigen::MatrixXi matrix) {
            N_ROWS = matrix.rows();
            N_COLS = matrix.cols();
            board_matrix = matrix;
        }

        ~Board() {}
        
        // Deep copy constructor
        Board(const Board& other) 
            : N_ROWS(other.N_ROWS), N_COLS(other.N_COLS), board_matrix(other.board_matrix) {
            // Deep copy map because it is a pointer internally
            piece_values = other.piece_values;
        }

        // Deep copy assignment operator
        Board& operator=(const Board& other) {
            if (this != &other) {  
                N_ROWS = other.N_ROWS;
                N_COLS = other.N_COLS;
                board_matrix = other.board_matrix;  
                piece_values = other.piece_values;  
            }
            return *this;
        }
        
        bool movePiece(Tile source, Tile target) {
            auto allowed_moves = this->getValidMoves(source);
            if (contains(allowed_moves, target)) {
                board_matrix(target.row, target.col) = board_matrix(source.row, source.col);
                board_matrix(source.row, source.col) = gap;
                return true;
            } else {
                return false;
            }
        }

        double get_score(int row, int col){
            double score;

            int piece = board_matrix(row, col);
            if (piece == gap) return 0.0;

            if (PESTO) {
                double mid_score = pesto_mid_piece_values[piece]
                        + mid_PSQT[piece](row,col);
                // // double end_score = pesto_end_piece_values[piece] + 
                // //                     end_PSQT[piece](row,col);
                score = mid_score;
            } else {
                score = piece_values[board_matrix(row, col)];
            }
            return score;
        }
        
        void printBoard() {
            std::cout << board_matrix << std::endl;
        }
        
        bool isWhite(Tile t) {
            return board_matrix(t.row, t.col) < 7 && board_matrix(t.row, t.col) != gap;
        }
        
        bool isBlack(Tile t) {
            return board_matrix(t.row, t.col) >= 7;
        }

        bool isWhite(int row, int col) {
            return board_matrix(row, col) < 7 && board_matrix(row, col) != gap;
        }

        bool isBlack(int row, int col) {
            return board_matrix(row, col) >= 7;
        }

        int other(int piece) {
            if (piece == gap) return gap;
            if (piece <= w_king) return piece + 6;
            if (piece >= b_pawn) return piece - 6;
            return piece;
        }
        
        std::vector<Tile> getValidMoves(int row, int col) {
            return getValidMoves(Tile(row, col));
        }

        std::vector<Tile> getValidMoves(Tile t) {
            int piece = board_matrix(t.row, t.col);
        
            switch (piece){
                case gap:
                    return {};
                case w_pawn:
                    return getValidWhitePawnMoves(t);
                case w_rook:
                    return getValidWhiteRookMoves(t);
                case w_knight:
                    return getValidWhiteKnightMoves(t);
                case w_bishop:
                    return getValidWhiteBishopMoves(t);
                case w_queen:   
                    return getValidWhiteQueenMoves(t);
                case w_king:
                    return getValidWhiteKingMoves(t); 
                case b_pawn:
                    return getValidBlackPawnMoves(t);
                case b_rook:    
                    return getValidBlackRookMoves(t);
                case b_knight:
                    return getValidBlackKnightMoves(t);
                case b_bishop:
                    return getValidBlackBishopMoves(t);
                case b_queen:
                    return getValidBlackQueenMoves(t);
                case b_king:
                    return getValidBlackKingMoves(t);
                default:
                std::cout << "Invalid piece" << std::endl;
                    return {};
            }
        }

    private:

        bool isWhite(int piece) {
            return piece <= w_king && piece != gap;
        }

        bool isBlack(int piece) {
            return piece >= b_pawn && piece != gap;
        }

        bool isWhiteOrGap(int piece) {
            return piece <= w_king || piece == gap;
        }

        bool isBlackOrGap(int piece) {
            return piece >= b_pawn || piece == gap;
        }


        std::vector<Tile> getValidWhitePawnMoves(Tile t) {
            std::vector<Tile> validMoves;
            //Move
            if (t.row == (N_ROWS-2)  && board_matrix(t.row - 1, t.col) == gap && board_matrix(t.row - 2, t.col) == gap) {
                validMoves.push_back(Tile(t.row - 2, t.col));
            }
            if (t.row > 0 && board_matrix(t.row - 1, t.col) == gap){
                validMoves.push_back(Tile(t.row - 1, t.col));
            }
            //Capture
            if (t.row > 0 && t.col > 0 && isBlack(board_matrix(t.row - 1, t.col - 1))) {
                validMoves.push_back(Tile(t.row - 1, t.col - 1));
            }
            if (t.row > 0 && t.col < (N_COLS-1) && isBlack(board_matrix(t.row - 1, t.col + 1))) {
                validMoves.push_back(Tile(t.row - 1, t.col + 1));
            }
        
            return validMoves;
        }
        
        std::vector<Tile> getValidWhiteRookMoves(Tile t) {
            std::vector<Tile> validMoves;
        
            for (int i = t.row - 1; i >= 0; i--) {
                if (board_matrix(i, t.col) == gap) {
                    validMoves.push_back(Tile(i, t.col));
                } else if (isBlack(board_matrix(i, t.col))) {
                    validMoves.push_back(Tile(i, t.col));
                    break;
                } else {
                    break;
                }
            }
            for (int i = t.row + 1; i < N_ROWS; i++) {
                if (board_matrix(i, t.col) == gap) {
                    validMoves.push_back(Tile(i, t.col));
                } else if(isBlack(board_matrix(i, t.col))) {
                    validMoves.push_back(Tile(i, t.col));
                    break;
                } else {
                    break;
                }
            }
            for (int i = t.col - 1; i >= 0; i--) {
                if (board_matrix(t.row, i) == gap) {
                    validMoves.push_back(Tile(t.row, i));
                } else if(isBlack(board_matrix(t.row, i))) {
                    validMoves.push_back(Tile(t.row, i));
                    break;
                } else {
                    break;
                }
            }
            for (int i = t.col + 1; i < N_COLS; i++) {
                if (board_matrix(t.row, i) == gap) {
                    validMoves.push_back(Tile(t.row, i));
                } else if(isBlack(board_matrix(t.row, i))) {
                    validMoves.push_back(Tile(t.row, i));
                    break;
                } else {
                    break;
                }
            }
        
            return validMoves;
        }
        
        std::vector<Tile> getValidWhiteKnightMoves(Tile t) {
            std::vector<Tile> validMoves;
        
            if(t.row-2 >= 0 && t.col-1 >= 0 && isBlackOrGap(board_matrix(t.row-2, t.col-1))) {
                validMoves.push_back(Tile(t.row-2, t.col-1));
            } 
            if(t.row-2 >= 0 && t.col+1 < N_COLS && isBlackOrGap(board_matrix(t.row-2, t.col+1))) {
                validMoves.push_back(Tile(t.row-2, t.col+1));
            }
            if(t.row-1 >= 0 && t.col-2 >= 0 && isBlackOrGap(board_matrix(t.row-1, t.col-2))) {
                validMoves.push_back(Tile(t.row-1, t.col-2));
            }
            if(t.row-1 >= 0 && t.col+2 < N_COLS && isBlackOrGap(board_matrix(t.row-1, t.col+2))) {
                validMoves.push_back(Tile(t.row-1, t.col+2));
            }
            if(t.row+1 < N_ROWS && t.col-2 >= 0 && isBlackOrGap(board_matrix(t.row+1, t.col-2))) {
                validMoves.push_back(Tile(t.row+1, t.col-2));
            }
            if(t.row+1 < N_ROWS && t.col+2 < N_COLS && isBlackOrGap(board_matrix(t.row+1, t.col+2))) {
                validMoves.push_back(Tile(t.row+1, t.col+2));
            }
            if(t.row+2 < N_ROWS && t.col-1 >= 0 && isBlackOrGap(board_matrix(t.row+2, t.col-1))) {
                validMoves.push_back(Tile(t.row+2, t.col-1));
            }
            if(t.row+2 < N_ROWS && t.col+1 < N_COLS && isBlackOrGap(board_matrix(t.row+2, t.col+1))) {
                validMoves.push_back(Tile(t.row+2, t.col+1));
            }
        
            return validMoves;
        }
        
        std::vector<Tile> getValidWhiteBishopMoves(Tile t) {
            std::vector<Tile> validMoves;
        
            for (int i = 1; t.row - i >= 0 && t.col - i >= 0; i++) {
                if (board_matrix(t.row - i, t.col - i) == gap) {
                    validMoves.push_back(Tile(t.row - i, t.col - i));
                } else if (isBlack(board_matrix(t.row - i, t.col - i))) {
                    validMoves.push_back(Tile(t.row - i, t.col - i));
                    break;
                } else {
                    break;
                }
            }
            for (int i = 1; t.row - i >= 0 && t.col + i < N_COLS; i++) {
                if (board_matrix(t.row - i, t.col + i) == gap) {
                    validMoves.push_back(Tile(t.row - i, t.col + i));
                } else if (isBlack(board_matrix(t.row - i, t.col + i))) {
                    validMoves.push_back(Tile(t.row - i, t.col + i));
                    break;
                } else {
                    break;
                }
            }
            for (int i = 1; t.row + i < N_ROWS && t.col - i >= 0; i++) {
                if (board_matrix(t.row + i, t.col - i) == gap) {
                    validMoves.push_back(Tile(t.row + i, t.col - i));
                } else if (isBlack(board_matrix(t.row + i, t.col - i))) {
                    validMoves.push_back(Tile(t.row + i, t.col - i));
                    break;
                } else {
                    break;
                }
            }
            for (int i = 1; t.row + i < N_ROWS && t.col + i < N_COLS; i++) {
                if (board_matrix(t.row + i, t.col + i) == gap) {
                    validMoves.push_back(Tile(t.row + i, t.col + i));
                } else if (isBlack(board_matrix(t.row + i, t.col + i))) {
                    validMoves.push_back(Tile(t.row + i, t.col + i));
                    break;
                } else {
                    break;
                }
            }
            
            return validMoves;
        }
        
        std::vector<Tile> getValidWhiteQueenMoves(Tile t) {
            std::vector<Tile> validMoves;
            std::vector<Tile> rookMoves = getValidWhiteRookMoves(t);
            std::vector<Tile> bishopMoves = getValidWhiteBishopMoves(t);
            validMoves.insert(validMoves.end(), rookMoves.begin(), rookMoves.end());
            validMoves.insert(validMoves.end(), bishopMoves.begin(), bishopMoves.end());
            return validMoves;
        }
        
        std::vector<Tile> getValidWhiteKingMoves(Tile t) {
            std::vector<Tile> validMoves;
        
            if(t.row-1 >= 0 && t.col-1 >= 0 && isBlackOrGap(board_matrix(t.row-1, t.col-1))) {
                validMoves.push_back(Tile(t.row-1, t.col-1));
            }
            if(t.row-1 >= 0 && isBlackOrGap(board_matrix(t.row-1, t.col))) {
                validMoves.push_back(Tile(t.row-1, t.col));
            }
            if(t.row-1 >= 0 && t.col+1 < N_COLS && isBlackOrGap(board_matrix(t.row-1, t.col+1))) {
                validMoves.push_back(Tile(t.row-1, t.col+1));
            }
            if(t.col-1 >= 0 && isBlackOrGap(board_matrix(t.row, t.col-1))) {
                validMoves.push_back(Tile(t.row, t.col-1));
            }
            if(t.col+1 < N_COLS && isBlackOrGap(board_matrix(t.row, t.col+1))) {
                validMoves.push_back(Tile(t.row, t.col+1));
            }
            if(t.row+1 < N_ROWS && t.col-1 >= 0 && isBlackOrGap(board_matrix(t.row+1, t.col-1))) {
                validMoves.push_back(Tile(t.row+1, t.col-1));
            }
            if(t.row+1 < N_ROWS && isBlackOrGap(board_matrix(t.row+1, t.col))) {
                validMoves.push_back(Tile(t.row+1, t.col));
            }
            if(t.row+1 < N_ROWS && t.col+1 < N_COLS && isBlackOrGap(board_matrix(t.row+1, t.col+1))) {
                validMoves.push_back(Tile(t.row+1, t.col+1));
            }
        
            return validMoves;
        }
        
        
        std::vector<Tile> getValidBlackPawnMoves(Tile t) {
            std::vector<Tile> validMoves;
        
            //Move
            if (t.row == 1  && board_matrix(t.row + 1, t.col) == gap && board_matrix(t.row + 2, t.col) == gap) {
                validMoves.push_back(Tile(t.row + 2, t.col));
            }
            if (t.row < (N_ROWS-1) && board_matrix(t.row + 1, t.col) == gap){
                validMoves.push_back(Tile(t.row + 1, t.col));
            }
            //Capture
            if (t.row < (N_ROWS-1) && t.col > 0 && isWhite(board_matrix(t.row + 1, t.col - 1))) {
                validMoves.push_back(Tile(t.row + 1, t.col - 1));
            }
            if (t.row < (N_ROWS-1) && t.col < (N_COLS-1) && isWhite(board_matrix(t.row + 1, t.col + 1))) {
                validMoves.push_back(Tile(t.row + 1, t.col + 1));
            }
            
            return validMoves;
        }


        std::vector<Tile> getValidBlackRookMoves(Tile t) {
            std::vector<Tile> validMoves;
        
            for (int i = t.row - 1; i >= 0; i--) {
                if (board_matrix(i, t.col) == gap) {
                    validMoves.push_back(Tile(i, t.col));
                } else if (isWhite(board_matrix(i, t.col))) {
                    validMoves.push_back(Tile(i, t.col));
                    break;
                } else {
                    break;
                }
            }
            for (int i = t.row + 1; i < N_ROWS; i++) {
                if (board_matrix(i, t.col) == gap) {
                    validMoves.push_back(Tile(i, t.col));
                } else if(isWhite(board_matrix(i, t.col))) {
                    validMoves.push_back(Tile(i, t.col));
                    break;
                } else {
                    break;
                }
            }
            for (int i = t.col - 1; i >= 0; i--) {
                if (board_matrix(t.row, i) == gap) {
                    validMoves.push_back(Tile(t.row, i));
                } else if(isWhite(board_matrix(t.row, i))) {
                    validMoves.push_back(Tile(t.row, i));
                    break;
                } else {
                    break;
                }
            }
            for (int i = t.col + 1; i < N_COLS; i++) {
                if (board_matrix(t.row, i) == gap) {
                    validMoves.push_back(Tile(t.row, i));
                } else if(isWhite(board_matrix(t.row, i))) {
                    validMoves.push_back(Tile(t.row, i));
                    break;
                } else {
                    break;
                }
            }
        
            return validMoves;
        }
        
        std::vector<Tile> getValidBlackKnightMoves(Tile t) {
            std::vector<Tile> validMoves;
        
            if(t.row-2 >= 0 && t.col-1 >= 0 && isWhiteOrGap(board_matrix(t.row-2, t.col-1))) {
                validMoves.push_back(Tile(t.row-2, t.col-1));
            } 
            if(t.row-2 >= 0 && t.col+1 < N_COLS && isWhiteOrGap(board_matrix(t.row-2, t.col+1))) {
                validMoves.push_back(Tile(t.row-2, t.col+1));
            }
            if(t.row-1 >= 0 && t.col-2 >= 0 && isWhiteOrGap(board_matrix(t.row-1, t.col-2))) {
                validMoves.push_back(Tile(t.row-1, t.col-2));
            }
            if(t.row-1 >= 0 && t.col+2 < N_COLS && isWhiteOrGap(board_matrix(t.row-1, t.col+2))) {
                validMoves.push_back(Tile(t.row-1, t.col+2));
            }
            if(t.row+1 < N_ROWS && t.col-2 >= 0 && isWhiteOrGap(board_matrix(t.row+1, t.col-2))) {
                validMoves.push_back(Tile(t.row+1, t.col-2));
            }
            if(t.row+1 < N_ROWS && t.col+2 < N_COLS && isWhiteOrGap(board_matrix(t.row+1, t.col+2))) {
                validMoves.push_back(Tile(t.row+1, t.col+2));
            }
            if(t.row+2 < N_ROWS && t.col-1 >= 0 && isWhiteOrGap(board_matrix(t.row+2, t.col-1))) {
                validMoves.push_back(Tile(t.row+2, t.col-1));
            }
            if(t.row+2 < N_ROWS && t.col+1 < N_COLS && isWhiteOrGap(board_matrix(t.row+2, t.col+1))) {
                validMoves.push_back(Tile(t.row+2, t.col+1));
            }
        
            return validMoves;
        }
        
        std::vector<Tile> getValidBlackBishopMoves(Tile t) {
            std::vector<Tile> validMoves;
        
            for (int i = 1; t.row - i >= 0 && t.col - i >= 0; i++) {
                if (board_matrix(t.row - i, t.col - i) == gap) {
                    validMoves.push_back(Tile(t.row - i, t.col - i));
                } else if (isWhite(board_matrix(t.row - i, t.col - i))) {
                    validMoves.push_back(Tile(t.row - i, t.col - i));
                    break;
                } else {
                    break;
                }
            }
            for (int i = 1; t.row - i >= 0 && t.col + i < N_COLS; i++) {
                if (board_matrix(t.row - i, t.col + i) == gap) {
                    validMoves.push_back(Tile(t.row - i, t.col + i));
                } else if (isWhite(board_matrix(t.row - i, t.col + i))) {
                    validMoves.push_back(Tile(t.row - i, t.col + i));
                    break;
                } else {
                    break;
                }
            }
            for (int i = 1; t.row + i < N_ROWS && t.col - i >= 0; i++) {
                if (board_matrix(t.row + i, t.col - i) == gap) {
                    validMoves.push_back(Tile(t.row + i, t.col - i));
                } else if (isWhite(board_matrix(t.row + i, t.col - i))) {
                    validMoves.push_back(Tile(t.row + i, t.col - i));
                    break;
                } else {
                    break;
                }
            }
            for (int i = 1; t.row + i < N_ROWS && t.col + i < N_COLS; i++) {
                if (board_matrix(t.row + i, t.col + i) == gap) {
                    validMoves.push_back(Tile(t.row + i, t.col + i));
                } else if (isWhite(board_matrix(t.row + i, t.col + i))) {
                    validMoves.push_back(Tile(t.row + i, t.col + i));
                    break;
                } else {
                    break;
                }
            }
            
            return validMoves;
        }
        
        std::vector<Tile> getValidBlackQueenMoves(Tile t) {
            std::vector<Tile> validMoves;
            std::vector<Tile> rookMoves = getValidBlackRookMoves(t);
            std::vector<Tile> bishopMoves = getValidBlackBishopMoves(t);
            validMoves.insert(validMoves.end(), rookMoves.begin(), rookMoves.end());
            validMoves.insert(validMoves.end(), bishopMoves.begin(), bishopMoves.end());
            return validMoves;
        }
        
        std::vector<Tile> getValidBlackKingMoves(Tile t) {
            std::vector<Tile> validMoves;
        
            if(t.row-1 >= 0 && t.col-1 >= 0 && isWhiteOrGap(board_matrix(t.row-1, t.col-1))) {
                validMoves.push_back(Tile(t.row-1, t.col-1));
            }
            if(t.row-1 >= 0 && isWhiteOrGap(board_matrix(t.row-1, t.col))) {
                validMoves.push_back(Tile(t.row-1, t.col));
            }
            if(t.row-1 >= 0 && t.col+1 < N_COLS && isWhiteOrGap(board_matrix(t.row-1, t.col+1))) {
                validMoves.push_back(Tile(t.row-1, t.col+1));
            }
            if(t.col-1 >= 0 && isWhiteOrGap(board_matrix(t.row, t.col-1))) {
                validMoves.push_back(Tile(t.row, t.col-1));
            }
            if(t.col+1 < N_COLS && isWhiteOrGap(board_matrix(t.row, t.col+1))) {
                validMoves.push_back(Tile(t.row, t.col+1));
            }
            if(t.row+1 < N_ROWS && t.col-1 >= 0 && isWhiteOrGap(board_matrix(t.row+1, t.col-1))) {
                validMoves.push_back(Tile(t.row+1, t.col-1));
            }
            if(t.row+1 < N_ROWS && isWhiteOrGap(board_matrix(t.row+1, t.col))) {
                validMoves.push_back(Tile(t.row+1, t.col));
            }
            if(t.row+1 < N_ROWS && t.col+1 < N_COLS && isWhiteOrGap(board_matrix(t.row+1, t.col+1))) {
                validMoves.push_back(Tile(t.row+1, t.col+1));
            }
        
            return validMoves;
        }

};