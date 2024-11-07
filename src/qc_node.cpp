#include "quantum_chess/qc_tree.hpp"
#include "quantum_chess/board.hpp"


QCNode::QCNode(Board B, int index){
    this->board = B;
    this->index = index;
    this->next = nullptr;
    this->split1 = nullptr;
    this->split2 = nullptr;
}

void QCNode::propagate(Tile source, Tile target){

    if(this->next == nullptr){
        if(this->split1 == nullptr && this->split2 == nullptr){
            Board new_board = this->board;
            new_board.movePiece(source, target);
            this->next = new QCNode(new_board, index+1);
        }else{
            this->split1->propagate(source, target);
            this->split2->propagate(source, target);
        }
    }else{
        this->next->propagate(source, target);
    }

}

void QCNode::split(Tile source, Tile target1, Tile target2){
    if(this->next == nullptr){
        if(this->split1 == nullptr && this->split2 == nullptr){
            Board new_board1 = this->board;
            new_board1.movePiece(source, target1);
            Board new_board2 = this->board;
            new_board2.movePiece(source, target2);
            this->split1 = new QCNode(new_board1, index+1);
            this->split2 = new QCNode(new_board2, index+1);
        }else{
            this->split1->split(source, target1, target2);
            this->split2->split(source, target1, target2);
        }
    }else{
        this->next->split(source, target1, target2);
    }
}
