#include "quantum_chess/board.hpp"

Board::Board() {
    // Initialize the board to be gap
    board_matrix = Eigen::Matrix<int, 8, 8>::Zero();
    board_matrix << b_rook,b_knight,b_bishop,b_queen,b_king,b_bishop,b_knight,b_rook,
             b_pawn,b_pawn,b_pawn,b_pawn,b_pawn,b_pawn,b_pawn,b_pawn,
            gap,gap,gap,gap,gap,gap,gap,gap,
            gap,gap,gap,gap,gap,gap,gap,gap,
            gap,gap,gap,gap,gap,gap,gap,gap,
            gap,gap,gap,gap,gap,gap,gap,gap,
            w_pawn,w_pawn,w_pawn,w_pawn,w_pawn,w_pawn,w_pawn,w_pawn,
            w_rook,w_knight,w_bishop,w_queen,w_king,w_bishop,w_knight,w_rook;
}

void Board::movePiece(Tile source, Tile target) {
    if(board_matrix(source.row, source.col)!=gap){
        board_matrix(target.row, target.col) = board_matrix(source.row, source.col);
        board_matrix(source.row, source.col) = gap;
    }
}

void Board::printBoard() {
    std::cout << board_matrix << std::endl;
}

bool Board::isWhite(Tile t) {
    return board_matrix(t.row, t.col) < 7 && board_matrix(t.row, t.col) != gap;
}

bool Board::isBlack(Tile t) {
    return board_matrix(t.row, t.col) >= 7;
}

std::vector<Tile> Board::getValidMoves(Tile t) {
    int piece = board_matrix(t.row, t.col);

    switch (piece){
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

std::vector<Tile> Board::getValidWhitePawnMoves(Tile t) {
    std::vector<Tile> validMoves;
    //Move
    if (t.row == 6  && board_matrix(t.row - 1, t.col) == gap && board_matrix(t.row - 2, t.col) == gap) {
        validMoves.push_back(Tile(t.row - 2, t.col));
    }
    if (t.row > 0 && board_matrix(t.row - 1, t.col) == gap){
        validMoves.push_back(Tile(t.row - 1, t.col));
    }
    //Capture
    if (t.row > 0 && t.col > 0 && board_matrix(t.row - 1, t.col - 1) >= 7) {
        validMoves.push_back(Tile(t.row - 1, t.col - 1));
    }
    if (t.row > 0 && t.col < 7 && board_matrix(t.row - 1, t.col + 1) >= 7) {
        validMoves.push_back(Tile(t.row - 1, t.col + 1));
    }

    return validMoves;
}

std::vector<Tile> Board::getValidWhiteRookMoves(Tile t) {
    std::vector<Tile> validMoves;

    for (int i = t.row - 1; i >= 0; i--) {
        if (board_matrix(i, t.col) == gap) {
            validMoves.push_back(Tile(i, t.col));
        } else if (board_matrix(i, t.col) >= 7) {
            validMoves.push_back(Tile(i, t.col));
            break;
        } else {
            break;
        }
    }
    for (int i = t.row + 1; i < 8; i++) {
        if (board_matrix(i, t.col) == gap) {
            validMoves.push_back(Tile(i, t.col));
        } else if(board_matrix(i, t.col) >= 7) {
            validMoves.push_back(Tile(i, t.col));
            break;
        } else {
            break;
        }
    }
    for (int i = t.col - 1; i >= 0; i--) {
        if (board_matrix(t.row, i) == gap) {
            validMoves.push_back(Tile(t.row, i));
        } else if(board_matrix(t.row, i) >= 7) {
            validMoves.push_back(Tile(i, t.col));
            break;
        } else {
            break;
        }
    }
    for (int i = t.col + 1; i < 8; i++) {
        if (board_matrix(t.row, i) == gap) {
            validMoves.push_back(Tile(t.row, i));
        } else if(board_matrix(t.row, i) >= 7) {
            validMoves.push_back(Tile(i, t.col));
            break;
        } else {
            break;
        }
    }

    return validMoves;
}

std::vector<Tile> Board::getValidWhiteKnightMoves(Tile t) {
    std::vector<Tile> validMoves;

    if(t.row-2 >= 0 && t.col-1 >= 0 && board_matrix(t.row-2, t.col-1) == gap) {
        validMoves.push_back(Tile(t.row-2, t.col-1));
    } else if (t.row-2 >= 0 && t.col-1 >= 0 && board_matrix(t.row-2, t.col-1) >= 7) {
        validMoves.push_back(Tile(t.row-2, t.col-1));
    }
    if(t.row-2 >= 0 && t.col+1 < 8 && board_matrix(t.row-2, t.col+1) == gap) {
        validMoves.push_back(Tile(t.row-2, t.col+1));
    } else if (t.row-2 >= 0 && t.col+1 < 8 && board_matrix(t.row-2, t.col+1) >= 7) {
        validMoves.push_back(Tile(t.row-2, t.col+1));
    }
    if(t.row-1 >= 0 && t.col-2 >= 0 && board_matrix(t.row-1, t.col-2) == gap) {
        validMoves.push_back(Tile(t.row-1, t.col-2));
    } else if (t.row-1 >= 0 && t.col-2 >= 0 && board_matrix(t.row-1, t.col-2) >= 7) {
        validMoves.push_back(Tile(t.row-1, t.col-2));
    }
    if(t.row-1 >= 0 && t.col+2 < 8 && board_matrix(t.row-1, t.col+2) == gap) {
        validMoves.push_back(Tile(t.row-1, t.col+2));
    } else if (t.row-1 >= 0 && t.col+2 < 8 && board_matrix(t.row-1, t.col+2) >= 7) {
        validMoves.push_back(Tile(t.row-1, t.col+2));
    }
    if(t.row+1 < 8 && t.col-2 >= 0 && board_matrix(t.row+1, t.col-2) == gap) {
        validMoves.push_back(Tile(t.row+1, t.col-2));
    } else if (t.row+1 < 8 && t.col-2 >= 0 && board_matrix(t.row+1, t.col-2) >= 7) {
        validMoves.push_back(Tile(t.row+1, t.col-2));
    }
    if(t.row+1 < 8 && t.col+2 < 8 && board_matrix(t.row+1, t.col+2) == gap) {
        validMoves.push_back(Tile(t.row+1, t.col+2));
    } else if (t.row+1 < 8 && t.col+2 < 8 && board_matrix(t.row+1, t.col+2) >= 7) {
        validMoves.push_back(Tile(t.row+1, t.col+2));
    }
    if(t.row+2 < 8 && t.col-1 >= 0 && board_matrix(t.row+2, t.col-1) == gap) {
        validMoves.push_back(Tile(t.row+2, t.col-1));
    } else if (t.row+2 < 8 && t.col-1 >= 0 && board_matrix(t.row+2, t.col-1) >= 7) {
        validMoves.push_back(Tile(t.row+2, t.col-1));
    }
    if(t.row+2 < 8 && t.col+1 < 8 && board_matrix(t.row+2, t.col+1) == gap) {
        validMoves.push_back(Tile(t.row+2, t.col+1));
    } else if (t.row+2 < 8 && t.col+1 < 8 && board_matrix(t.row+2, t.col+1) >= 7) {
        validMoves.push_back(Tile(t.row+2, t.col+1));
    }

    return validMoves;
}

std::vector<Tile> Board::getValidWhiteBishopMoves(Tile t) {
    std::vector<Tile> validMoves;

    for (int i = 1; t.row - i >= 0 && t.col - i >= 0; i++) {
        if (board_matrix(t.row - i, t.col - i) == gap) {
            validMoves.push_back(Tile(t.row - i, t.col - i));
        } else if (board_matrix(t.row - i, t.col - i) >= 7) {
            validMoves.push_back(Tile(t.row - i, t.col - i));
            break;
        } else {
            break;
        }
    }
    for (int i = 1; t.row - i >= 0 && t.col + i < 8; i++) {
        if (board_matrix(t.row - i, t.col + i) == gap) {
            validMoves.push_back(Tile(t.row - i, t.col + i));
        } else if (board_matrix(t.row - i, t.col + i) >= 7) {
            validMoves.push_back(Tile(t.row - i, t.col + i));
            break;
        } else {
            break;
        }
    }
    for (int i = 1; t.row + i < 8 && t.col - i >= 0; i++) {
        if (board_matrix(t.row + i, t.col - i) == gap) {
            validMoves.push_back(Tile(t.row + i, t.col - i));
        } else if (board_matrix(t.row + i, t.col - i) >= 7) {
            validMoves.push_back(Tile(t.row + i, t.col - i));
            break;
        } else {
            break;
        }
    }
    for (int i = 1; t.row + i < 8 && t.col + i < 8; i++) {
        if (board_matrix(t.row + i, t.col + i) == gap) {
            validMoves.push_back(Tile(t.row + i, t.col + i));
        } else if (board_matrix(t.row + i, t.col + i) >= 7) {
            validMoves.push_back(Tile(t.row + i, t.col + i));
            break;
        } else {
            break;
        }
    }
    
    return validMoves;
}

std::vector<Tile> Board::getValidWhiteQueenMoves(Tile t) {
    std::vector<Tile> validMoves;
    std::vector<Tile> rookMoves = getValidWhiteRookMoves(t);
    std::vector<Tile> bishopMoves = getValidWhiteBishopMoves(t);
    validMoves.insert(validMoves.end(), rookMoves.begin(), rookMoves.end());
    validMoves.insert(validMoves.end(), bishopMoves.begin(), bishopMoves.end());
    return validMoves;
}

std::vector<Tile> Board::getValidWhiteKingMoves(Tile t) {
    std::vector<Tile> validMoves;

    if(t.row-1 >= 0 && t.col-1 >= 0 && board_matrix(t.row-1, t.col-1) == gap) {
        validMoves.push_back(Tile(t.row-1, t.col-1));
    } else if (t.row-1 >= 0 && t.col-1 >= 0 && board_matrix(t.row-1, t.col-1) >= 7) {
        validMoves.push_back(Tile(t.row-1, t.col-1));
    }
    if(t.row-1 >= 0 && board_matrix(t.row-1, t.col) == gap) {
        validMoves.push_back(Tile(t.row-1, t.col));
    } else if (t.row-1 >= 0 && board_matrix(t.row-1, t.col) >= 7) {
        validMoves.push_back(Tile(t.row-1, t.col));
    }
    if(t.row-1 >= 0 && t.col+1 < 8 && board_matrix(t.row-1, t.col+1) == gap) {
        validMoves.push_back(Tile(t.row-1, t.col+1));
    } else if (t.row-1 >= 0 && t.col+1 < 8 && board_matrix(t.row-1, t.col+1) >= 7) {
        validMoves.push_back(Tile(t.row-1, t.col+1));
    }
    if(t.col-1 >= 0 && board_matrix(t.row, t.col-1) == gap) {
        validMoves.push_back(Tile(t.row, t.col-1));
    } else if (t.col-1 >= 0 && board_matrix(t.row, t.col-1) >= 7) {
        validMoves.push_back(Tile(t.row, t.col-1));
    }
    if(t.col+1 < 8 && board_matrix(t.row, t.col+1) == gap) {
        validMoves.push_back(Tile(t.row, t.col+1));
    } else if (t.col+1 < 8 && board_matrix(t.row, t.col+1) >= 7) {
        validMoves.push_back(Tile(t.row, t.col+1));
    }
    if(t.row+1 < 8 && t.col-1 >= 0 && board_matrix(t.row+1, t.col-1) == gap) {
        validMoves.push_back(Tile(t.row+1, t.col-1));
    } else if (t.row+1 < 8 && t.col-1 >= 0 && board_matrix(t.row+1, t.col-1) >= 7) {
        validMoves.push_back(Tile(t.row+1, t.col-1));
    }
    if(t.row+1 < 8 && board_matrix(t.row+1, t.col) == gap) {
        validMoves.push_back(Tile(t.row+1, t.col));
    } else if (t.row+1 < 8 && board_matrix(t.row+1, t.col) >= 7) {
        validMoves.push_back(Tile(t.row+1, t.col));
    }
    if(t.row+1 < 8 && t.col+1 < 8 && board_matrix(t.row+1, t.col+1) == gap) {
        validMoves.push_back(Tile(t.row+1, t.col+1));
    } else if (t.row+1 < 8 && t.col+1 < 8 && board_matrix(t.row+1, t.col+1) >= 7) {
        validMoves.push_back(Tile(t.row+1, t.col+1));
    }

    return validMoves;
}


std::vector<Tile> Board::getValidBlackPawnMoves(Tile t) {
    std::vector<Tile> validMoves;

    //Move
    if (t.row == 1  && board_matrix(t.row + 1, t.col) == gap && board_matrix(t.row + 2, t.col) == gap) {
        validMoves.push_back(Tile(t.row + 2, t.col));
    }
    if (t.row < 7 && board_matrix(t.row + 1, t.col) == gap){
        validMoves.push_back(Tile(t.row + 1, t.col));
    }
    //Capture
    if (t.row < 7 && t.col > 0 && board_matrix(t.row + 1, t.col - 1) < 7 && board_matrix(t.row + 1, t.col - 1) != gap) {
        validMoves.push_back(Tile(t.row + 1, t.col - 1));
    }
    if (t.row < 7 && t.col < 7 && board_matrix(t.row + 1, t.col + 1) < 7 && board_matrix(t.row + 1, t.col + 1) != gap) {
        validMoves.push_back(Tile(t.row + 1, t.col + 1));
    }
    
    return validMoves;
}

std::vector<Tile> Board::getValidBlackRookMoves(Tile t) {
    std::vector<Tile> validMoves;

    for (int i = t.row - 1; i >= 0; i--) {
        if (board_matrix(i, t.col) == gap) {
            validMoves.push_back(Tile(i, t.col));
        } else if (board_matrix(i, t.col) < 7) {
            validMoves.push_back(Tile(i, t.col));
            break;
        } else {
            break;
        }
    }
    for (int i = t.row + 1; i < 8; i++) {
        if (board_matrix(i, t.col) == gap) {
            validMoves.push_back(Tile(i, t.col));
        } else if(board_matrix(i, t.col) < 7) {
            validMoves.push_back(Tile(i, t.col));
            break;
        } else {
            break;
        }
    }
    for (int i = t.col - 1; i >= 0; i--) {
        if (board_matrix(t.row, i) == gap) {
            validMoves.push_back(Tile(t.row, i));
        } else if(board_matrix(t.row, i) < 7) {
            validMoves.push_back(Tile(t.row, i));
            break;
        } else {
            break;
        }
    }
    for (int i = t.col + 1; i < 8; i++) {
        if (board_matrix(t.row, i) == gap) {
            validMoves.push_back(Tile(t.row, i));
        } else if(board_matrix(t.row, i) < 7) {
            validMoves.push_back(Tile(t.row, i));
            break;
        } else {
            break;
        }
    }

    return validMoves;
}

std::vector<Tile> Board::getValidBlackKnightMoves(Tile t) {
    std::vector<Tile> validMoves;

    if(t.row-2 >= 0 && t.col-1 >= 0 && board_matrix(t.row-2, t.col-1) == gap) {
        validMoves.push_back(Tile(t.row-2, t.col-1));
    } else if (t.row-2 >= 0 && t.col-1 >= 0 && board_matrix(t.row-2, t.col-1) < 7) {
        validMoves.push_back(Tile(t.row-2, t.col-1));
    }
    if(t.row-2 >= 0 && t.col+1 < 8 && board_matrix(t.row-2, t.col+1) == gap) {
        validMoves.push_back(Tile(t.row-2, t.col+1));
    } else if (t.row-2 >= 0 && t.col+1 < 8 && board_matrix(t.row-2, t.col+1) < 7) {
        validMoves.push_back(Tile(t.row-2, t.col+1));
    }
    if(t.row-1 >= 0 && t.col-2 >= 0 && board_matrix(t.row-1, t.col-2) == gap) {
        validMoves.push_back(Tile(t.row-1, t.col-2));
    } else if (t.row-1 >= 0 && t.col-2 >= 0 && board_matrix(t.row-1, t.col-2) < 7) {
        validMoves.push_back(Tile(t.row-1, t.col-2));
    }
    if(t.row-1 >= 0 && t.col+2 < 8 && board_matrix(t.row-1, t.col+2) == gap) {
        validMoves.push_back(Tile(t.row-1, t.col+2));
    } else if (t.row-1 >= 0 && t.col+2 < 8 && board_matrix(t.row-1, t.col+2) < 7) {
        validMoves.push_back(Tile(t.row-1, t.col+2));
    }
    if(t.row+1 < 8 && t.col-2 >= 0 && board_matrix(t.row+1, t.col-2) == gap) {
        validMoves.push_back(Tile(t.row+1, t.col-2));
    } else if (t.row+1 < 8 && t.col-2 >= 0 && board_matrix(t.row+1, t.col-2) < 7) {
        validMoves.push_back(Tile(t.row+1, t.col-2));
    }
    if(t.row+1 < 8 && t.col+2 < 8 && board_matrix(t.row+1, t.col+2) == gap) {
        validMoves.push_back(Tile(t.row+1, t.col+2));
    } else if (t.row+1 < 8 && t.col+2 < 8 && board_matrix(t.row+1, t.col+2) < 7) {
        validMoves.push_back(Tile(t.row+1, t.col+2));
    }
    if(t.row+2 < 8 && t.col-1 >= 0 && board_matrix(t.row+2, t.col-1) == gap) {
        validMoves.push_back(Tile(t.row+2, t.col-1));
    } else if (t.row+2 < 8 && t.col-1 >= 0 && board_matrix(t.row+2, t.col-1) < 7) {
        validMoves.push_back(Tile(t.row+2, t.col-1));
    }
    if(t.row+2 < 8 && t.col+1 < 8 && board_matrix(t.row+2, t.col+1) == gap) {
        validMoves.push_back(Tile(t.row+2, t.col+1));
    } else if (t.row+2 < 8 && t.col+1 < 8 && board_matrix(t.row+2, t.col+1) < 7) {
        validMoves.push_back(Tile(t.row+2, t.col+1));
    }

    return validMoves;
}

std::vector<Tile> Board::getValidBlackBishopMoves(Tile t) {
    std::vector<Tile> validMoves;

    for (int i = 1; t.row - i >= 0 && t.col - i >= 0; i++) {
        if (board_matrix(t.row - i, t.col - i) == gap) {
            validMoves.push_back(Tile(t.row - i, t.col - i));
        } else if (board_matrix(t.row - i, t.col - i) < 7) {
            validMoves.push_back(Tile(t.row - i, t.col - i));
            break;
        } else {
            break;
        }
    }
    for (int i = 1; t.row - i >= 0 && t.col + i < 8; i++) {
        if (board_matrix(t.row - i, t.col + i) == gap) {
            validMoves.push_back(Tile(t.row - i, t.col + i));
        } else if (board_matrix(t.row - i, t.col + i) < 7) {
            validMoves.push_back(Tile(t.row - i, t.col + i));
            break;
        } else {
            break;
        }
    }
    for (int i = 1; t.row + i < 8 && t.col - i >= 0; i++) {
        if (board_matrix(t.row + i, t.col - i) == gap) {
            validMoves.push_back(Tile(t.row + i, t.col - i));
        } else if (board_matrix(t.row + i, t.col - i) < 7) {
            validMoves.push_back(Tile(t.row + i, t.col - i));
            break;
        } else {
            break;
        }
    }
    for (int i = 1; t.row + i < 8 && t.col + i < 8; i++) {
        if (board_matrix(t.row + i, t.col + i) == gap) {
            validMoves.push_back(Tile(t.row + i, t.col + i));
        } else if (board_matrix(t.row + i, t.col + i) < 7) {
            validMoves.push_back(Tile(t.row + i, t.col + i));
            break;
        } else {
            break;
        }
    }
    
    return validMoves;
}

std::vector<Tile> Board::getValidBlackQueenMoves(Tile t) {
    std::vector<Tile> validMoves;
    std::vector<Tile> rookMoves = getValidBlackRookMoves(t);
    std::vector<Tile> bishopMoves = getValidBlackBishopMoves(t);
    validMoves.insert(validMoves.end(), rookMoves.begin(), rookMoves.end());
    validMoves.insert(validMoves.end(), bishopMoves.begin(), bishopMoves.end());
    return validMoves;
}

std::vector<Tile> Board::getValidBlackKingMoves(Tile t) {
    std::vector<Tile> validMoves;

    if(t.row-1 >= 0 && t.col-1 >= 0 && board_matrix(t.row-1, t.col-1) == gap) {
        validMoves.push_back(Tile(t.row-1, t.col-1));
    } else if (t.row-1 >= 0 && t.col-1 >= 0 && board_matrix(t.row-1, t.col-1) < 7) {
        validMoves.push_back(Tile(t.row-1, t.col-1));
    }
    if(t.row-1 >= 0 && board_matrix(t.row-1, t.col) == gap) {
        validMoves.push_back(Tile(t.row-1, t.col));
    } else if (t.row-1 >= 0 && board_matrix(t.row-1, t.col) < 7) {
        validMoves.push_back(Tile(t.row-1, t.col));
    }
    if(t.row-1 >= 0 && t.col+1 < 8 && board_matrix(t.row-1, t.col+1) == gap) {
        validMoves.push_back(Tile(t.row-1, t.col+1));
    } else if (t.row-1 >= 0 && t.col+1 < 8 && board_matrix(t.row-1, t.col+1) < 7) {
        validMoves.push_back(Tile(t.row-1, t.col+1));
    }
    if(t.col-1 >= 0 && board_matrix(t.row, t.col-1) == gap) {
        validMoves.push_back(Tile(t.row, t.col-1));
    } else if (t.col-1 >= 0 && board_matrix(t.row, t.col-1) < 7) {
        validMoves.push_back(Tile(t.row, t.col-1));
    }
    if(t.col+1 < 8 && board_matrix(t.row, t.col+1) == gap) {
        validMoves.push_back(Tile(t.row, t.col+1));
    } else if (t.col+1 < 8 && board_matrix(t.row, t.col+1) < 7) {
        validMoves.push_back(Tile(t.row, t.col+1));
    }
    if(t.row+1 < 8 && t.col-1 >= 0 && board_matrix(t.row+1, t.col-1) == gap) {
        validMoves.push_back(Tile(t.row+1, t.col-1));
    } else if (t.row+1 < 8 && t.col-1 >= 0 && board_matrix(t.row+1, t.col-1) < 7) {
        validMoves.push_back(Tile(t.row+1, t.col-1));
    }
    if(t.row+1 < 8 && board_matrix(t.row+1, t.col) == gap) {
        validMoves.push_back(Tile(t.row+1, t.col));
    } else if (t.row+1 < 8 && board_matrix(t.row+1, t.col) < 7) {
        validMoves.push_back(Tile(t.row+1, t.col));
    }
    if(t.row+1 < 8 && t.col+1 < 8 && board_matrix(t.row+1, t.col+1) == gap) {
        validMoves.push_back(Tile(t.row+1, t.col+1));
    } else if (t.row+1 < 8 && t.col+1 < 8 && board_matrix(t.row+1, t.col+1) < 7) {
        validMoves.push_back(Tile(t.row+1, t.col+1));
    }

    return validMoves;
}