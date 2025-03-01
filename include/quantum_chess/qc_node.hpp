#ifndef QCNODE_H
#define QCNODE_H

#include <iostream>
#include "quantum_chess/board.hpp"

class QCNode {
    public:

        int index_;
        Board board_;
        QCNode* left_;
        QCNode* right_;

        QCNode(Board B, int index){
            this->board_ = B;
            this->index_ = index;
            this->left_ = nullptr;
            this->right_ = nullptr;
        }
        
        // Update the board for all leaf nodes
        void propagate(Tile source, Tile target){
        
            if(this->left_ == nullptr && this->right_ == nullptr){
                this->board_.movePiece(source, target);
            }else{
                this->left_->propagate(source, target);
                this->right_->propagate(source, target);
            }
        
        }
        
        // Split all leaf nodes in two
        void split(Tile source, Tile target1, Tile target2){
            if(this->left_ == nullptr && this->right_ == nullptr){
                Board new_board1 = this->board_;
                new_board1.movePiece(source, target1);
                Board new_board2 = this->board_;
                new_board2.movePiece(source, target2);
                this->left_ = new QCNode(new_board1, this->index_+1);
                this->right_ = new QCNode(new_board2, this->index_+1);
            }else{
                this->left_->split(source, target1, target2);
                this->right_->split(source, target1, target2);
            }
        }
        
};

#endif // QCNODE_H