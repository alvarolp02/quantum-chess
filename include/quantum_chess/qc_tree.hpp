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
        int N_ROWS;
        int N_COLS;

        QCTree(){
            N_ROWS = 8;
            N_COLS = 8;
            this->q_board = Board(Eigen::MatrixXi::Zero(N_ROWS, N_COLS));
            this->depth = 0;
            this->root = new QCNode(Board(),0);

            std::srand(std::time(nullptr)); // use current time as seed for random generator
        }
        
        QCTree(Eigen::MatrixXi matrix){
            N_ROWS = matrix.rows();
            N_COLS = matrix.cols();
            this->q_board = Board(Eigen::MatrixXi::Zero(N_ROWS, N_COLS));
            this->depth = 0;
            this->root = new QCNode(Board(matrix),0);

            std::srand(std::time(nullptr)); // use current time as seed for random generator
        }
        
        void propagate(Tile source, Tile target){
            // Move the piece in all leaf nodes and store wether the move was possible or not
            std::vector<bool> moves_result = {};
            int sum = 0;
            for (Board* board : get_leaf_boards()){
                moves_result.push_back(board->movePiece(source, target));
                sum += int(moves_result.back());
            }


            // Search depths where entanglement occurs by comparing the number of moves that are possible
            std::vector<int> entanglement_depths = {};

            if (sum < moves_result.size()){
                double p = double(sum)/moves_result.size();
                int i = 1;

                // Get actual depths list
                std::vector <int> depths = {this->root->index};
                QCNode* node = this->root;
                while (node->left != nullptr && node->right != nullptr){
                    depths.push_back(node->left->index);
                    node = node->left;
                }

                while (moves_result.size() > 1){
                    sum = 0;
                    std::vector<bool> new_moves_result = {};
                    for (int i = 0; i < moves_result.size(); i+=2){
                        new_moves_result.push_back(moves_result[i] || moves_result[i+1]);
                        sum += int(new_moves_result.back());
                    }
                    
                    // If the proportion of moves that are possible is higher than the previous one, 
                    // then there is entanglement with the split at this depth
                    double p2 = double(sum)/new_moves_result.size();
                    if (p2 > p){
                        entanglement_depths.push_back(depths[depths.size()-i-1]);
                        std::cout << "Entanglement at depth: " << depths[depths.size()-i-1] << std::endl;
                    }

                    i++;
                    p=p2;
                    moves_result = new_moves_result;
                }
            }

            // Update splits pieces with entanglement
            for (int d : entanglement_depths){
                for (auto s : splits){
                    if (std::find(s->depths.begin(), s->depths.end(), d) != s->depths.end()){
                        s->pieces.push_back(source);
                        s->pieces.push_back(target);
                    }
                }
            }

            
        
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
                        int D = this->splits[i]->depths[j];
                        for (auto node : get_nodes_at_depth(D)){
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
                    break;
                }
            }
        
        }


        std::vector<Board*> get_leaf_boards(){
            std::vector<Board*> leaf_boards = {};
            for (auto node : get_nodes_at_depth(this->depth)){
                leaf_boards.push_back(&(node->board));
            }
            return leaf_boards;
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
        
            pond_matrix = Eigen::MatrixXd::Zero(N_ROWS, N_COLS);
            q_board.board_matrix = Eigen::MatrixXi::Zero(N_ROWS, N_COLS);
        
            for (int i = 0; i < N_ROWS; ++i) {
                for (int j = 0; j < N_COLS; ++j) {
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