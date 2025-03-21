#pragma once

#include <iostream>
#include "quantum_chess/board.hpp"

class QCNode {
    public:

        int index;
        Board board;
        QCNode* left;
        QCNode* right;

        QCNode(Board B, int index){
            this->board = B;
            this->index = index;
            this->left = nullptr;
            this->right = nullptr;
        }
        
        // Split all leaf nodes in two
        void split(Tile source, Tile target1, Tile target2){
            if(this->left == nullptr && this->right == nullptr){
                Board new_board1 = this->board;
                new_board1.movePiece(source, target1);
                Board new_board2 = this->board;
                new_board2.movePiece(source, target2);
                this->left = new QCNode(new_board1, this->index+1);
                this->right = new QCNode(new_board2, this->index+1);
            }else{
                this->left->split(source, target1, target2);
                this->right->split(source, target1, target2);
            }
        }
        
};