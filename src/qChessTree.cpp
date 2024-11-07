#include <iostream>
#include "qChessNode.cpp"

class qChessTree {
    public:
        qChessTree(Board B){
            this->root = new qChessNode(B,0);
        }

        void propagate(Tile source, Tile target){
            this->root->propagate(source, target);
        }

        void print_tree(){
            qChessNode* current = this->root;
            while(current != nullptr){
                std::cout << current->index << std::endl;
                current = current->next;
            }
        }

        qChessNode* root;
};