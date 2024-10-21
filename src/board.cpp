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

void Board::movePiece(int row0, int col0, int row1, int col1) {
    board(row1, col1) = board(row0, col0);
    board(row0, col0) = empty;
}

void Board::printBoard() {
    std::cout << board << std::endl;
}

bool Board::isWhite(int row, int col) {
    return board(row, col) < 7 && board(row, col) != empty;
}

bool Board::isBlack(int row, int col) {
    return board(row, col) >= 7;
}

std::vector<std::pair<int,int>> Board::getValidMoves(int row, int col) {
    int piece = board(row, col);

    switch (piece){
        case w_pawn:
            return getValidWhitePawnMoves(row, col);
        case w_rook:
            return getValidWhiteRookMoves(row, col);
        case w_knight:
            return getValidWhiteKnightMoves(row, col);
        case w_bishop:
            return getValidWhiteBishopMoves(row, col);
        case w_queen:   
            return getValidWhiteQueenMoves(row, col);
        case w_king:
            return getValidWhiteKingMoves(row, col); 
        case b_pawn:
            return getValidBlackPawnMoves(row, col);
        case b_rook:    
            return getValidBlackRookMoves(row, col);
        case b_knight:
            return getValidBlackKnightMoves(row, col);
        case b_bishop:
            return getValidBlackBishopMoves(row, col);
        case b_queen:
            return getValidBlackQueenMoves(row, col);
        case b_king:
            return getValidBlackKingMoves(row, col);
        default:
        std::cout << "Invalid piece" << std::endl;
            return {};
    }
}

std::vector<std::pair<int,int>> Board::getValidWhitePawnMoves(int row, int col) {
    std::vector<std::pair<int,int>> validMoves;
    //Move
    if (row == 6  && board(row - 1, col) == empty && board(row - 2, col) == empty) {
        validMoves.push_back(std::make_pair(row - 2, col));
    }
    if (row > 0 && board(row - 1, col) == empty){
        validMoves.push_back(std::make_pair(row - 1, col));
    }
    //Capture
    if (row > 0 && col > 0 && board(row - 1, col - 1) >= 7) {
        validMoves.push_back(std::make_pair(row - 1, col - 1));
    }
    if (row > 0 && col < 7 && board(row - 1, col + 1) >= 7) {
        validMoves.push_back(std::make_pair(row - 1, col + 1));
    }

    return validMoves;
}

std::vector<std::pair<int,int>> Board::getValidWhiteRookMoves(int row, int col) {
    std::vector<std::pair<int,int>> validMoves;

    for (int i = row - 1; i >= 0; i--) {
        if (board(i, col) == empty) {
            validMoves.push_back(std::make_pair(i, col));
        } else if (board(i, col) >= 7) {
            validMoves.push_back(std::make_pair(i, col));
            break;
        } else {
            break;
        }
    }
    for (int i = row + 1; i < 8; i++) {
        if (board(i, col) == empty) {
            validMoves.push_back(std::make_pair(i, col));
        } else if(board(i, col) >= 7) {
            validMoves.push_back(std::make_pair(i, col));
            break;
        } else {
            break;
        }
    }
    for (int i = col - 1; i >= 0; i--) {
        if (board(row, i) == empty) {
            validMoves.push_back(std::make_pair(row, i));
        } else if(board(row, i) >= 7) {
            validMoves.push_back(std::make_pair(i, col));
            break;
        } else {
            break;
        }
    }
    for (int i = col + 1; i < 8; i++) {
        if (board(row, i) == empty) {
            validMoves.push_back(std::make_pair(row, i));
        } else if(board(row, i) >= 7) {
            validMoves.push_back(std::make_pair(i, col));
            break;
        } else {
            break;
        }
    }

    return validMoves;
}

std::vector<std::pair<int,int>> Board::getValidWhiteKnightMoves(int row, int col) {
    std::vector<std::pair<int,int>> validMoves;

    if(row-2 >= 0 && col-1 >= 0 && board(row-2, col-1) == empty) {
        validMoves.push_back(std::make_pair(row-2, col-1));
    } else if (row-2 >= 0 && col-1 >= 0 && board(row-2, col-1) >= 7) {
        validMoves.push_back(std::make_pair(row-2, col-1));
    }
    if(row-2 >= 0 && col+1 < 8 && board(row-2, col+1) == empty) {
        validMoves.push_back(std::make_pair(row-2, col+1));
    } else if (row-2 >= 0 && col+1 < 8 && board(row-2, col+1) >= 7) {
        validMoves.push_back(std::make_pair(row-2, col+1));
    }
    if(row-1 >= 0 && col-2 >= 0 && board(row-1, col-2) == empty) {
        validMoves.push_back(std::make_pair(row-1, col-2));
    } else if (row-1 >= 0 && col-2 >= 0 && board(row-1, col-2) >= 7) {
        validMoves.push_back(std::make_pair(row-1, col-2));
    }
    if(row-1 >= 0 && col+2 < 8 && board(row-1, col+2) == empty) {
        validMoves.push_back(std::make_pair(row-1, col+2));
    } else if (row-1 >= 0 && col+2 < 8 && board(row-1, col+2) >= 7) {
        validMoves.push_back(std::make_pair(row-1, col+2));
    }
    if(row+1 < 8 && col-2 >= 0 && board(row+1, col-2) == empty) {
        validMoves.push_back(std::make_pair(row+1, col-2));
    } else if (row+1 < 8 && col-2 >= 0 && board(row+1, col-2) >= 7) {
        validMoves.push_back(std::make_pair(row+1, col-2));
    }
    if(row+1 < 8 && col+2 < 8 && board(row+1, col+2) == empty) {
        validMoves.push_back(std::make_pair(row+1, col+2));
    } else if (row+1 < 8 && col+2 < 8 && board(row+1, col+2) >= 7) {
        validMoves.push_back(std::make_pair(row+1, col+2));
    }
    if(row+2 < 8 && col-1 >= 0 && board(row+2, col-1) == empty) {
        validMoves.push_back(std::make_pair(row+2, col-1));
    } else if (row+2 < 8 && col-1 >= 0 && board(row+2, col-1) >= 7) {
        validMoves.push_back(std::make_pair(row+2, col-1));
    }
    if(row+2 < 8 && col+1 < 8 && board(row+2, col+1) == empty) {
        validMoves.push_back(std::make_pair(row+2, col+1));
    } else if (row+2 < 8 && col+1 < 8 && board(row+2, col+1) >= 7) {
        validMoves.push_back(std::make_pair(row+2, col+1));
    }

    return validMoves;
}

std::vector<std::pair<int,int>> Board::getValidWhiteBishopMoves(int row, int col) {
    std::vector<std::pair<int,int>> validMoves;

    for (int i = 1; row - i >= 0 && col - i >= 0; i++) {
        if (board(row - i, col - i) == empty) {
            validMoves.push_back(std::make_pair(row - i, col - i));
        } else if (board(row - i, col - i) >= 7) {
            validMoves.push_back(std::make_pair(row - i, col - i));
            break;
        } else {
            break;
        }
    }
    for (int i = 1; row - i >= 0 && col + i < 8; i++) {
        if (board(row - i, col + i) == empty) {
            validMoves.push_back(std::make_pair(row - i, col + i));
        } else if (board(row - i, col + i) >= 7) {
            validMoves.push_back(std::make_pair(row - i, col + i));
            break;
        } else {
            break;
        }
    }
    for (int i = 1; row + i < 8 && col - i >= 0; i++) {
        if (board(row + i, col - i) == empty) {
            validMoves.push_back(std::make_pair(row + i, col - i));
        } else if (board(row + i, col - i) >= 7) {
            validMoves.push_back(std::make_pair(row + i, col - i));
            break;
        } else {
            break;
        }
    }
    for (int i = 1; row + i < 8 && col + i < 8; i++) {
        if (board(row + i, col + i) == empty) {
            validMoves.push_back(std::make_pair(row + i, col + i));
        } else if (board(row + i, col + i) >= 7) {
            validMoves.push_back(std::make_pair(row + i, col + i));
            break;
        } else {
            break;
        }
    }
    
    return validMoves;
}

std::vector<std::pair<int,int>> Board::getValidWhiteQueenMoves(int row, int col) {
    std::vector<std::pair<int,int>> validMoves;
    std::vector<std::pair<int,int>> rookMoves = getValidWhiteRookMoves(row, col);
    std::vector<std::pair<int,int>> bishopMoves = getValidWhiteBishopMoves(row, col);
    validMoves.insert(validMoves.end(), rookMoves.begin(), rookMoves.end());
    validMoves.insert(validMoves.end(), bishopMoves.begin(), bishopMoves.end());
    return validMoves;
}

std::vector<std::pair<int,int>> Board::getValidWhiteKingMoves(int row, int col) {
    std::vector<std::pair<int,int>> validMoves;

    if(row-1 >= 0 && col-1 >= 0 && board(row-1, col-1) == empty) {
        validMoves.push_back(std::make_pair(row-1, col-1));
    } else if (row-1 >= 0 && col-1 >= 0 && board(row-1, col-1) >= 7) {
        validMoves.push_back(std::make_pair(row-1, col-1));
    }
    if(row-1 >= 0 && board(row-1, col) == empty) {
        validMoves.push_back(std::make_pair(row-1, col));
    } else if (row-1 >= 0 && board(row-1, col) >= 7) {
        validMoves.push_back(std::make_pair(row-1, col));
    }
    if(row-1 >= 0 && col+1 < 8 && board(row-1, col+1) == empty) {
        validMoves.push_back(std::make_pair(row-1, col+1));
    } else if (row-1 >= 0 && col+1 < 8 && board(row-1, col+1) >= 7) {
        validMoves.push_back(std::make_pair(row-1, col+1));
    }
    if(col-1 >= 0 && board(row, col-1) == empty) {
        validMoves.push_back(std::make_pair(row, col-1));
    } else if (col-1 >= 0 && board(row, col-1) >= 7) {
        validMoves.push_back(std::make_pair(row, col-1));
    }
    if(col+1 < 8 && board(row, col+1) == empty) {
        validMoves.push_back(std::make_pair(row, col+1));
    } else if (col+1 < 8 && board(row, col+1) >= 7) {
        validMoves.push_back(std::make_pair(row, col+1));
    }
    if(row+1 < 8 && col-1 >= 0 && board(row+1, col-1) == empty) {
        validMoves.push_back(std::make_pair(row+1, col-1));
    } else if (row+1 < 8 && col-1 >= 0 && board(row+1, col-1) >= 7) {
        validMoves.push_back(std::make_pair(row+1, col-1));
    }
    if(row+1 < 8 && board(row+1, col) == empty) {
        validMoves.push_back(std::make_pair(row+1, col));
    } else if (row+1 < 8 && board(row+1, col) >= 7) {
        validMoves.push_back(std::make_pair(row+1, col));
    }
    if(row+1 < 8 && col+1 < 8 && board(row+1, col+1) == empty) {
        validMoves.push_back(std::make_pair(row+1, col+1));
    } else if (row+1 < 8 && col+1 < 8 && board(row+1, col+1) >= 7) {
        validMoves.push_back(std::make_pair(row+1, col+1));
    }

    return validMoves;
}


std::vector<std::pair<int,int>> Board::getValidBlackPawnMoves(int row, int col) {
    std::vector<std::pair<int,int>> validMoves;

    //Move
    if (row == 1  && board(row + 1, col) == empty && board(row + 2, col) == empty) {
        validMoves.push_back(std::make_pair(row + 2, col));
    }
    if (row < 7 && board(row + 1, col) == empty){
        validMoves.push_back(std::make_pair(row + 1, col));
    }
    //Capture
    if (row < 7 && col > 0 && board(row + 1, col - 1) < 7 && board(row + 1, col - 1) != empty) {
        validMoves.push_back(std::make_pair(row + 1, col - 1));
    }
    if (row < 7 && col < 7 && board(row + 1, col + 1) < 7 && board(row + 1, col + 1) != empty) {
        validMoves.push_back(std::make_pair(row + 1, col + 1));
    }
    
    return validMoves;
}

std::vector<std::pair<int,int>> Board::getValidBlackRookMoves(int row, int col) {
    std::vector<std::pair<int,int>> validMoves;

    for (int i = row - 1; i >= 0; i--) {
        if (board(i, col) == empty) {
            validMoves.push_back(std::make_pair(i, col));
        } else if (board(i, col) < 7) {
            validMoves.push_back(std::make_pair(i, col));
            break;
        } else {
            break;
        }
    }
    for (int i = row + 1; i < 8; i++) {
        if (board(i, col) == empty) {
            validMoves.push_back(std::make_pair(i, col));
        } else if(board(i, col) < 7) {
            validMoves.push_back(std::make_pair(i, col));
            break;
        } else {
            break;
        }
    }
    for (int i = col - 1; i >= 0; i--) {
        if (board(row, i) == empty) {
            validMoves.push_back(std::make_pair(row, i));
        } else if(board(i, col) < 7) {
            validMoves.push_back(std::make_pair(i, col));
            break;
        } else {
            break;
        }
    }
    for (int i = col + 1; i < 8; i++) {
        if (board(row, i) == empty) {
            validMoves.push_back(std::make_pair(row, i));
        } else if(board(i, col) < 7) {
            validMoves.push_back(std::make_pair(i, col));
            break;
        } else {
            break;
        }
    }

    return validMoves;
}

std::vector<std::pair<int,int>> Board::getValidBlackKnightMoves(int row, int col) {
    std::vector<std::pair<int,int>> validMoves;

    if(row-2 >= 0 && col-1 >= 0 && board(row-2, col-1) == empty) {
        validMoves.push_back(std::make_pair(row-2, col-1));
    } else if (row-2 >= 0 && col-1 >= 0 && board(row-2, col-1) < 7) {
        validMoves.push_back(std::make_pair(row-2, col-1));
    }
    if(row-2 >= 0 && col+1 < 8 && board(row-2, col+1) == empty) {
        validMoves.push_back(std::make_pair(row-2, col+1));
    } else if (row-2 >= 0 && col+1 < 8 && board(row-2, col+1) < 7) {
        validMoves.push_back(std::make_pair(row-2, col+1));
    }
    if(row-1 >= 0 && col-2 >= 0 && board(row-1, col-2) == empty) {
        validMoves.push_back(std::make_pair(row-1, col-2));
    } else if (row-1 >= 0 && col-2 >= 0 && board(row-1, col-2) < 7) {
        validMoves.push_back(std::make_pair(row-1, col-2));
    }
    if(row-1 >= 0 && col+2 < 8 && board(row-1, col+2) == empty) {
        validMoves.push_back(std::make_pair(row-1, col+2));
    } else if (row-1 >= 0 && col+2 < 8 && board(row-1, col+2) < 7) {
        validMoves.push_back(std::make_pair(row-1, col+2));
    }
    if(row+1 < 8 && col-2 >= 0 && board(row+1, col-2) == empty) {
        validMoves.push_back(std::make_pair(row+1, col-2));
    } else if (row+1 < 8 && col-2 >= 0 && board(row+1, col-2) < 7) {
        validMoves.push_back(std::make_pair(row+1, col-2));
    }
    if(row+1 < 8 && col+2 < 8 && board(row+1, col+2) == empty) {
        validMoves.push_back(std::make_pair(row+1, col+2));
    } else if (row+1 < 8 && col+2 < 8 && board(row+1, col+2) < 7) {
        validMoves.push_back(std::make_pair(row+1, col+2));
    }
    if(row+2 < 8 && col-1 >= 0 && board(row+2, col-1) == empty) {
        validMoves.push_back(std::make_pair(row+2, col-1));
    } else if (row+2 < 8 && col-1 >= 0 && board(row+2, col-1) < 7) {
        validMoves.push_back(std::make_pair(row+2, col-1));
    }
    if(row+2 < 8 && col+1 < 8 && board(row+2, col+1) == empty) {
        validMoves.push_back(std::make_pair(row+2, col+1));
    } else if (row+2 < 8 && col+1 < 8 && board(row+2, col+1) < 7) {
        validMoves.push_back(std::make_pair(row+2, col+1));
    }

    return validMoves;
}

std::vector<std::pair<int,int>> Board::getValidBlackBishopMoves(int row, int col) {
    std::vector<std::pair<int,int>> validMoves;

    for (int i = 1; row - i >= 0 && col - i >= 0; i++) {
        if (board(row - i, col - i) == empty) {
            validMoves.push_back(std::make_pair(row - i, col - i));
        } else if (board(row - i, col - i) < 7) {
            validMoves.push_back(std::make_pair(row - i, col - i));
            break;
        } else {
            break;
        }
    }
    for (int i = 1; row - i >= 0 && col + i < 8; i++) {
        if (board(row - i, col + i) == empty) {
            validMoves.push_back(std::make_pair(row - i, col + i));
        } else if (board(row - i, col + i) < 7) {
            validMoves.push_back(std::make_pair(row - i, col + i));
            break;
        } else {
            break;
        }
    }
    for (int i = 1; row + i < 8 && col - i >= 0; i++) {
        if (board(row + i, col - i) == empty) {
            validMoves.push_back(std::make_pair(row + i, col - i));
        } else if (board(row + i, col - i) < 7) {
            validMoves.push_back(std::make_pair(row + i, col - i));
            break;
        } else {
            break;
        }
    }
    for (int i = 1; row + i < 8 && col + i < 8; i++) {
        if (board(row + i, col + i) == empty) {
            validMoves.push_back(std::make_pair(row + i, col + i));
        } else if (board(row + i, col + i) < 7) {
            validMoves.push_back(std::make_pair(row + i, col + i));
            break;
        } else {
            break;
        }
    }
    
    return validMoves;
}

std::vector<std::pair<int,int>> Board::getValidBlackQueenMoves(int row, int col) {
    std::vector<std::pair<int,int>> validMoves;
    std::vector<std::pair<int,int>> rookMoves = getValidBlackRookMoves(row, col);
    std::vector<std::pair<int,int>> bishopMoves = getValidBlackBishopMoves(row, col);
    validMoves.insert(validMoves.end(), rookMoves.begin(), rookMoves.end());
    validMoves.insert(validMoves.end(), bishopMoves.begin(), bishopMoves.end());
    return validMoves;
}

std::vector<std::pair<int,int>> Board::getValidBlackKingMoves(int row, int col) {
    std::vector<std::pair<int,int>> validMoves;

    if(row-1 >= 0 && col-1 >= 0 && board(row-1, col-1) == empty) {
        validMoves.push_back(std::make_pair(row-1, col-1));
    } else if (row-1 >= 0 && col-1 >= 0 && board(row-1, col-1) < 7) {
        validMoves.push_back(std::make_pair(row-1, col-1));
    }
    if(row-1 >= 0 && board(row-1, col) == empty) {
        validMoves.push_back(std::make_pair(row-1, col));
    } else if (row-1 >= 0 && board(row-1, col) < 7) {
        validMoves.push_back(std::make_pair(row-1, col));
    }
    if(row-1 >= 0 && col+1 < 8 && board(row-1, col+1) == empty) {
        validMoves.push_back(std::make_pair(row-1, col+1));
    } else if (row-1 >= 0 && col+1 < 8 && board(row-1, col+1) < 7) {
        validMoves.push_back(std::make_pair(row-1, col+1));
    }
    if(col-1 >= 0 && board(row, col-1) == empty) {
        validMoves.push_back(std::make_pair(row, col-1));
    } else if (col-1 >= 0 && board(row, col-1) < 7) {
        validMoves.push_back(std::make_pair(row, col-1));
    }
    if(col+1 < 8 && board(row, col+1) == empty) {
        validMoves.push_back(std::make_pair(row, col+1));
    } else if (col+1 < 8 && board(row, col+1) < 7) {
        validMoves.push_back(std::make_pair(row, col+1));
    }
    if(row+1 < 8 && col-1 >= 0 && board(row+1, col-1) == empty) {
        validMoves.push_back(std::make_pair(row+1, col-1));
    } else if (row+1 < 8 && col-1 >= 0 && board(row+1, col-1) < 7) {
        validMoves.push_back(std::make_pair(row+1, col-1));
    }
    if(row+1 < 8 && board(row+1, col) == empty) {
        validMoves.push_back(std::make_pair(row+1, col));
    } else if (row+1 < 8 && board(row+1, col) < 7) {
        validMoves.push_back(std::make_pair(row+1, col));
    }
    if(row+1 < 8 && col+1 < 8 && board(row+1, col+1) == empty) {
        validMoves.push_back(std::make_pair(row+1, col+1));
    } else if (row+1 < 8 && col+1 < 8 && board(row+1, col+1) < 7) {
        validMoves.push_back(std::make_pair(row+1, col+1));
    }

    return validMoves;
}