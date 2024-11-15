#include "quantum_chess/qc_tree.hpp"
#include "quantum_chess/board.hpp"


QCNode::QCNode(Board B, int index){
    this->board = B;
    this->index = index;
    this->left = nullptr;
    this->right = nullptr;
}

// Update the board for all leaf nodes
void QCNode::propagate(Tile source, Tile target){

    if(this->left == nullptr && this->right == nullptr){
        this->board.movePiece(source, target);
    }else{
        this->left->propagate(source, target);
        this->right->propagate(source, target);
    }

}

// Split all leaf nodes in two
void QCNode::split(Tile source, Tile target1, Tile target2){
    if(this->left == nullptr && this->right == nullptr){
        Board new_board1 = this->board;
        new_board1.movePiece(source, target1);
        Board new_board2 = this->board;
        new_board2.movePiece(source, target2);
        this->left = new QCNode(new_board1, index+1);
        this->right = new QCNode(new_board2, index+1);
    }else{
        this->left->split(source, target1, target2);
        this->right->split(source, target1, target2);
    }
}
