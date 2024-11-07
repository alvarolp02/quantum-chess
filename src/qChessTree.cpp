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

        void split(Tile source, Tile target1, Tile target2){
            this->root->split(source, target1, target2);
        }

        void print_tree(){
            print_tree_aux(this->root, "");
            
            std::cout  << std::endl << std::endl << std::endl;
        }

        void print_tree_aux(qChessNode* node, std::string prefix, bool split = false){
            if(node==nullptr){
                return;
            }

            if(split){
                std::cout << std::endl;
                std::cout << prefix;
                std::cout << "└─ ";
                prefix += "    ";
            } else {
                std::cout << "── "; 
                prefix += "    "; 
            }
            std::cout << node->index;

            print_tree_aux(node->next, prefix, false);
            print_tree_aux(node->split1, prefix, true);
            print_tree_aux(node->split2, prefix, true);
            
        }

        void print_tree_aux2(qChessNode* node, std::string prefix, bool last = true){
            if(node==nullptr){
                return;
            }

            std::cout << prefix;
            if(last){
                std::cout << "└── ";
                prefix += "    ";
            } else {
                std::cout << "├── ";  
                prefix += "│   "; 
            }
            std::cout << node->index << std::endl;

            print_tree_aux(node->next, prefix, true);
            print_tree_aux(node->split1, prefix, false);
            print_tree_aux(node->split2, prefix, true);
        }

        qChessNode* root;
};