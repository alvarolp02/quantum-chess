#include <iostream>
#include "board.cpp"

class qChessNode {
    public:
        qChessNode(Board B, int index){
            this->board = B;
            this->index = index;
            this->next = nullptr;
            this->split1 = nullptr;
            this->split2 = nullptr;
        }

        void propagate(Tile source, Tile target){

            if(this->next == nullptr){
                if(this->split1 == nullptr && this->split2 == nullptr){
                    Board new_board = this->board;
                    new_board.movePiece(source, target);
                    this->next = new qChessNode(new_board, index+1);
                }else{
                    this->split1->propagate(source, target);
                    this->split2->propagate(source, target);
                }
            }else{
                this->next->propagate(source, target);
            }

        }

        void split(Tile source, Tile target1, Tile target2){
            if(this->next == nullptr){
                if(this->split1 == nullptr && this->split2 == nullptr){
                    Board new_board1 = this->board;
                    new_board1.movePiece(source, target1);
                    Board new_board2 = this->board;
                    new_board2.movePiece(source, target2);
                    this->split1 = new qChessNode(new_board1, index+1);
                    this->split2 = new qChessNode(new_board2, index+1);
                }else{
                    this->split1->split(source, target1, target2);
                    this->split2->split(source, target1, target2);
                }
            }else{
                this->next->split(source, target1, target2);
            }
        }

        int index;
        Board board;
        qChessNode* next;
        qChessNode* split1;
        qChessNode* split2;
};