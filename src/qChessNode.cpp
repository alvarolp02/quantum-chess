#include <iostream>
#include "board.cpp"

class qChessNode {
    public:
        qChessNode(Board B){
            this->B = B;
            this->index = 0;
            this->next = nullptr;
            this->split1 = nullptr;
            this->split2 = nullptr;
        }

        void propagate(std::pair<int,int> source, std::pair<int,int> target){
            Board new_B = this->B.copy();
            new_B.movePiece(source.first, source.second, target.first, target.second);
            this->next = new qChessNode(new_B);
            this->next->index = this->index + 1;
        }

        void split(){
            this->split1 = new qChessNode(this->B);
            this->split1->index = this->index + 1;
            this->split2 = new qChessNode(this->B);
            this->split2->index = this->index + 1;
        }

        int index;
        qChessNode* next;
        qChessNode* split1;
        qChessNode* split2;
};