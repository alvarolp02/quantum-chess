#ifndef QCTREE_H
#define QCTREE_H

#include <iostream>
#include "quantum_chess/qc_node.hpp"
#include <random> 

class QCTree {
    public:

        Eigen::Matrix<double, 8, 8> pond_matrix;
        Board q_board;
        int depth;
        std::vector<Split> splits;
        QCNode* root;
        
        QCTree(){
            this->q_board = Board();
            this->depth = 0;
            this->root = new QCNode(Board(),0);

            std::srand(std::time(nullptr)); // use current time as seed for random generator
        }
        
        void propagate(Tile source, Tile target){
            this->root->propagate(source, target);
        
            // Update splits positions
            for (int i = 0; i < this->splits.size(); ++i) {
                if(this->splits[i].piece == source){
                    this->splits[i].piece = target;
                    break;
                }
            }
        }
        
        void split(Tile source, Tile target1, Tile target2){
        
            Split s1;
            s1.depth = this->depth;
            s1.piece = target1;
            this->splits.push_back(s1);
        
            Split s2;
            s2.depth = this->depth;
            s2.piece = target2;
            splits.push_back(s2);
        
            this->root->split(source, target1, target2);
            this->depth++;
        }
        
        void collapse(Tile piece){
            std::cout << "Collapsing..." << std::endl;

            // Randomly choose one of the children to collapse
            bool random_boolean = std::rand()%2;
            
            for (int i = 0; i < splits.size(); ++i) {
                // Search the split related to the actual piece
                if(splits[i].piece == piece){
                    // Collapse all nodes at depth of split
                    for (auto node : get_nodes_at_depth(this->splits[i].depth)){
                        // Delete one of the children randomly
                        if (random_boolean){
                            delete_node(node->right); 
                            node->index = node->left->index;
                            node->board = node->left->board;
                            node->right = node->left->right;
                            node->left = node->left->left;
                        }else{
                            delete_node(node->left); 
                            node->index = node->right->index;
                            node->board = node->right->board;
                            node->left = node->right->left;
                            node->right = node->right->right;
                        }
                    }
                    
                    //Erase all splits collapsed
                    std::vector<Split> new_splits;
                    for (Split s : splits){
                        if(s.depth != splits[i].depth){
                            new_splits.push_back(s);
                        }
                    }
                    splits = new_splits;
                    break;
                }
            }
        
        }
        

        void print_tree(){
            print_tree_aux(this->root, "");
            
            std::cout  << std::endl << std::endl;
        }
        
        
        
        void get_ponderated_board(){
            std::vector<Eigen::Matrix<int, 8, 8>> boards = {};
            for(auto node : get_nodes_at_depth(this->depth)){
                boards.push_back(node->board.board_matrix);
                std::cout << node->board.board_matrix << std::endl;
                std::cout <<  "------------" << std::endl;
            }
            int n_boards = boards.size();
        
            pond_matrix = Eigen::Matrix<double, 8, 8>::Zero();
            q_board.board_matrix = Eigen::Matrix<int, 8, 8>::Zero();
        
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    double sum = 0.0;
                    for (auto board : boards) {
                        if (board(i, j) != gap) {
                            sum+=1;
                            if(q_board.board_matrix(i, j) == gap){
                                q_board.board_matrix(i, j) = board(i, j);
                            } else if (q_board.board_matrix(i, j) != board(i, j)){
                                std::cout << "Error: Different pieces in the same tile" << std::endl;
                            }
                        }
                    }
                    pond_matrix(i, j) = sum/n_boards;
                }
            }
        }
    

    private: 

        void print_tree_aux(QCNode* node, std::string prefix){
            if(node==nullptr){
                return;
            }
        
            std::cout << std::endl;
            std::cout << prefix;
            std::cout << "└─ ";
            prefix += "    ";
            std::cout << node->index;
        
            print_tree_aux(node->left, prefix);
            print_tree_aux(node->right, prefix);
            
        }

        // Delete node and all its children
        void delete_node(QCNode* node){
            if(node == nullptr){
                return;
            }
            delete_node(node->left);
            delete_node(node->right);
            
            delete node;
        }

        std::vector<QCNode*> get_nodes_at_depth(int depth){
            std::vector<QCNode*> nodes = get_nodes_at_depth_aux(this->root, depth, {});
            return nodes;
        }
        
        std::vector<QCNode*> get_nodes_at_depth_aux(QCNode* node, int depth, std::vector<QCNode*> acum){
            if(node==nullptr){
                return acum;
            }
        
            if(node->index == depth){
                acum.push_back(node);
            }
        
            if(node->left != nullptr && node->right != nullptr){
                auto acum_aux = get_nodes_at_depth_aux(node->left, depth, acum);
                acum = get_nodes_at_depth_aux(node->right, depth, acum_aux);
            }
        
            return acum;
        }


};

#endif // QCTREE