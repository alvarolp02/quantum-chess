#ifndef QCTREE_H
#define QCTREE_H

#include <iostream>
#include "quantum_chess/qc_node.hpp"
#include <random> 

class QCTree {
    public:

        Eigen::MatrixXd pond_matrix;
        Board q_board;
        int depth;
        std::vector<Split*> splits;
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
                for (int j = 0; j < this->splits[i]->pieces.size(); ++j) {
                    if(this->splits[i]->pieces[j] == source){
                        this->splits[i]->pieces[j] = target;
                        break;
                    }
                }
            }
        }
        
        void split(Tile source, Tile target1, Tile target2){
            Tile t1 = target1;
            Tile t2 = target2;
            
            bool nested_split = false;
            for (Split* split : splits){
                if(std::find(split->pieces.begin(), split->pieces.end(), source) != split->pieces.end()){
                    split->depths.push_back(this->depth);
                    split->pieces.erase(std::remove(split->pieces.begin(), split->pieces.end(), source), split->pieces.end());
                    split->pieces.push_back(target1);
                    split->pieces.push_back(target2);

                    nested_split = true;
                    break;
                }
            }
            
            if (!nested_split){
                Split* s = new Split();
                s->depths = {this->depth};
                s->pieces = {t1, t2};
                this->splits.push_back(s);
            }
            this->root->split(source, target1, target2);

            this->depth++;
            
            std::cout << ":::::::::::::::" << std::endl;
            for (Split* split : splits){
                std::cout << "Pieces: ";
                for (Tile piece : split->pieces){
                    std::cout << piece.to_string() << " ";
                }
                std::cout << "Depths: ";
                for (int depth : split->depths){
                    std::cout << depth << " ";
                }
                std::cout << std::endl;
            }
            std::cout << ":::::::::::::::" << std::endl;
        }
        
        void collapse(Tile piece){
            std::cout << "Collapsing..." << std::endl;
            bool random_boolean;
            
            for (int i = 0; i < splits.size(); ++i) {
                // Search the split related to the actual piece
                if(std::find(splits[i]->pieces.begin(), splits[i]->pieces.end(), piece) != splits[i]->pieces.end()){
                    
                    for (int j = splits[i]->depths.size()-1; j >= 0; j--) {
                        // Randomly choose one of the children to collapse
                        random_boolean = std::rand()%2;

                        // Collapse all nodes at depth of split
                        for (auto node : get_nodes_at_depth(this->splits[i]->depths[j])){
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
                    }
                    
                    splits.erase(splits.begin() + i);
                    //Erase all splits collapsed
                    // std::vector<Split> new_splits;
                    // for (Split s : splits){
                    //     if(s.depth != splits[i].depth){
                    //         new_splits.push_back(s);
                    //     }
                    // }
                    // splits = new_splits;
                    break;
                }
            }
        
        }
        

        void print_tree(){
            print_tree_aux(this->root, "");
            
            std::cout  << std::endl << std::endl;
        }
        
        
        
        void get_ponderated_board(){
            std::vector<Eigen::MatrixXi> boards = {};
            for(auto node : get_nodes_at_depth(this->depth)){
                boards.push_back(node->board.board_matrix);
            }
            int n_boards = boards.size();
        
            pond_matrix = Eigen::MatrixXd::Zero(8, 8);
            q_board.board_matrix = Eigen::MatrixXi::Zero(8, 8);
        
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