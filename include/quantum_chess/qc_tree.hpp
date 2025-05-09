#pragma once

#include <iostream>
#include "quantum_chess/qc_node.hpp"
#include "quantum_chess/utils.hpp"
#include <random> 
#include <ctime>

class QCTree {
    public:                             

        Eigen::MatrixXd pond_matrix;
        Board q_board;
        int depth;
        GameState state;
        std::vector<std::shared_ptr<Split>> splits;
        std::shared_ptr<QCNode> root;
        int N_ROWS;
        int N_COLS;
        bool DEBUG = false;
        bool ALLOW_ENTANGLEMENT = false;
        double score;

        QCTree(){
            N_ROWS = 8;
            N_COLS = 8;
            score = 0.0;
            this->q_board = Board(Eigen::MatrixXi::Zero(N_ROWS, N_COLS));
            this->depth = 0;
            this->state = Playing;
            this->root = std::make_shared<QCNode>(std::make_shared<Board>(), 0);

            std::srand(std::time(nullptr)); // use current time as seed for random generator
            this->get_ponderated_board();
            this->compute_tree_score();
        }

        // Deep copy constructor
        QCTree(const QCTree& other): pond_matrix(other.pond_matrix), q_board(other.q_board), depth(other.depth),
                N_ROWS(other.N_ROWS), N_COLS(other.N_COLS), score(other.score), state(other.state), root(nullptr) {
            // Deep copy splits 
            splits.reserve(other.splits.size());
            for (auto s : other.splits) {
                if (s != nullptr)
                    splits.push_back(std::make_shared<Split>(*s));
                else
                    splits.push_back(nullptr);
            }
            // Deep copy root
            if (other.root != nullptr)
                root = std::make_shared<QCNode>(*other.root);
        }

        // Deep copy assignment operator
        QCTree& operator=(const QCTree& other) {
            if (this == &other)
                return *this; // Avoid self-assignment
            
            pond_matrix = other.pond_matrix;
            q_board = other.q_board;
            depth = other.depth;
            N_ROWS = other.N_ROWS;
            N_COLS = other.N_COLS;
            score = other.score;
            state = other.state;

            // Deep copy splits
            splits.reserve(other.splits.size());
            for (auto s : other.splits) {
                if (s != nullptr)
                    splits.push_back(std::make_shared<Split>(*s));
                else
                    splits.push_back(nullptr);
            }

            // Deep copy root
            if (other.root != nullptr)
                root = std::make_shared<QCNode>(*other.root);

            return *this;
        }

        // Destructor
        ~QCTree() {
        }
        
        QCTree(Eigen::MatrixXi matrix){
            N_ROWS = matrix.rows();
            N_COLS = matrix.cols();
            this->q_board = Board(Eigen::MatrixXi::Zero(N_ROWS, N_COLS));
            this->depth = 0;
            this->state = Playing;
            this->root = std::make_shared<QCNode>(std::make_shared<Board>(matrix), 0);

            std::srand(std::time(nullptr)); // use current time as seed for random generator
            this->get_ponderated_board();
            this->compute_tree_score();
        }
        
        void propagate(Tile source, Tile target){
            // Move the piece in all leaf nodes and store wether the move was possible or not
            std::vector<bool> moves_result = {};
            int sum = 0;
            for (std::shared_ptr<Board> board : get_leaf_boards()){
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
                std::shared_ptr<QCNode> node = this->root;
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
                        if (DEBUG) std::cout << "Entanglement at depth: " << depths[depths.size()-i-1] << std::endl;
                    }

                    i++;
                    p=p2;
                    moves_result = new_moves_result;
                }
            }

            // Update splits pieces with entanglement
            for (int d : entanglement_depths){
                for (auto s : splits){
                    // Check the split depth and if the source piece is not already in the split
                    if(contains(s->depths, d) && !contains(s->pieces, source)){
                        s->pieces.push_back(source);
                        s->pieces.push_back(target);
                    }
                }
            }

            
        
            // Update splits positions
            for (int i = 0; i < this->splits.size(); ++i) {
                std::shared_ptr<Split> s = this->splits[i];
                for (int j = 0; j < s->pieces.size(); ++j) {
                    if(s->pieces[j] == source){
                        // If the target is not in the split, update the source position
                        if (!contains(s->pieces, target)){
                            s->pieces[j] = target;
                        // If the target is already in the split, remove the source position
                        } else {
                            s->pieces.erase(s->pieces.begin() + j);
                        }
                        break;
                    }
                }
                // This only occurs when a piece is merged with itself and its probability is 1
                // We collapse the split to avoid redundant branches
                if (s->pieces.size() == 1){
                    this->collapse(s->pieces[0]);
                }
            }

            this->get_ponderated_board();
            this->compute_tree_score();
            this->check_state();
        }
        
        void split(Tile source, Tile target1, Tile target2){
            Tile t1 = target1;
            Tile t2 = target2;
            
            bool nested_split = false;
            for (std::shared_ptr<Split> split : splits){
                if(contains(split->pieces, source)){
                    split->depths.push_back(this->depth);
                    split->pieces.erase(std::remove(split->pieces.begin(), split->pieces.end(), source), split->pieces.end());
                    split->pieces.push_back(target1);
                    split->pieces.push_back(target2);

                    nested_split = true;
                    break;
                }
            }
            
            if (!nested_split){
                std::shared_ptr<Split> s = std::make_shared<Split>();
                s->depths = {this->depth};
                s->pieces = {t1, t2};
                this->splits.push_back(s);
            }
            this->root->split(source, target1, target2);

            this->depth++;
            this->get_ponderated_board();
            this->compute_tree_score();
            this->check_state();
        }
        
        void collapse(Tile piece){
            if (DEBUG) std::cout << "Collapsing..." << std::endl;
            bool random_boolean;
            
            for (int i = 0; i < splits.size(); ++i) {
                // Search the split related to the actual piece
                if(contains(splits[i]->pieces, piece)) {
                    
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
                                node->board = std::make_shared<Board>(node->left->board->board_matrix);
                                node->right = node->left->right;
                                node->left = node->left->left;
                            }else{
                                delete_node(node->left); 
                                node->index = node->right->index;
                                node->board = std::make_shared<Board>(node->right->board->board_matrix);
                                node->left = node->right->left;
                                node->right = node->right->right;
                            }
                        }
                    }
                    
                    splits.erase(splits.begin() + i);
                    break;
                }
            }

            this->get_ponderated_board();
            this->compute_tree_score();
            this->check_state();
        }



        std::vector<std::shared_ptr<Board>> get_leaf_boards(){
            std::vector<std::shared_ptr<Board>> leaf_boards = {};
            for (auto node : get_nodes_at_depth(this->depth)){
                leaf_boards.push_back((node->board));
            }
            return leaf_boards;
        }
        

        std::pair<std::vector<std::vector<Tile>>,std::vector<std::vector<Tile>>> 
			                                get_movements(std::string turn){
            std::vector<std::vector<Tile>> movements;
            std::vector<std::vector<Tile>> collapse_movements;

            for (int i = 0; i < N_ROWS; i++){
                for (int j = 0; j < N_COLS; j++){
                    Tile source = Tile(i, j);

                    if (turn == "white" && this->q_board.isWhite(source) ||
                            turn == "black" && this->q_board.isBlack(source)){
                        std::vector<Tile> validMoves = this->q_board.getValidMoves(source);
                        
                        if (ALLOW_ENTANGLEMENT){
                            for (std::shared_ptr<Board> board : this->get_leaf_boards()){
                                for (Tile move : board->getValidMoves(source)){
                                    if (this->q_board.board_matrix(move.row, move.col) == gap || 
                                        this->q_board.board_matrix(move.row, move.col) == board->board_matrix(source.row, source.col)){
                                        validMoves.push_back(move);
                                    }
                                }
                            }
                        }
                        

                        for (Tile target1 : validMoves){
                            // Check if the move would be a quantum capture
                            if (this->pond_matrix(source.row, source.col) != 1.00
                                && ((turn=="white" && this->q_board.isBlack(target1))
                                || (turn=="black" && this->q_board.isWhite(target1)))){
                                    
                                collapse_movements.push_back({source, target1});
                                continue;
                            } else {
                                // Get simple moves
                                movements.push_back({source, target1});
                            }

                            // Get split moves only to gap targets
                            if (this->q_board.board_matrix(target1.row, target1.col) != gap){
                                continue;
                            }
                            for (Tile target2 : validMoves){
                                if (!(target1==target2) && this->q_board.board_matrix(target2.row, target2.col) == gap){
                                    movements.push_back({source, target1, target2});
                                }
                            }
                        }
                    } 
                }
            }

            return {movements, collapse_movements};
        }


        void check_state(){
            bool white_king = false;
            bool black_king = false;
        
            for (int i = 0; i < N_ROWS && !(white_king && black_king); i++){
                for (int j = 0; j < N_COLS && !(white_king && black_king); j++){
                    if (this->q_board.board_matrix(i, j) == w_king){
                        white_king = true;
                    } else if (this->q_board.board_matrix(i, j) == b_king){
                        black_king = true;
                    }
                }
            }
        
            if (!white_king && black_king){
                this->state = BlackWins;
            } else if (!black_king && white_king){
                this->state = WhiteWins;
            } else if (!white_king && !black_king){
                this->state = Draw;
            } else {
                this->state = Playing;
            }
        }


        void print_tree(){
            print_tree_aux(this->root, "");
            
            std::cout  << std::endl << std::endl;
        }
        
    

    private: 

        void print_tree_aux(std::shared_ptr<QCNode> node, std::string prefix){
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
        void delete_node(std::shared_ptr<QCNode> node){
            if(node == nullptr){
                return;
            }
            delete_node(node->left);
            delete_node(node->right);
    
            node = nullptr;
        }

        std::vector<std::shared_ptr<QCNode>> get_nodes_at_depth(int depth){
            std::vector<std::shared_ptr<QCNode>> nodes = get_nodes_at_depth_aux(this->root, depth, {});
            return nodes;
        }
        
        std::vector<std::shared_ptr<QCNode>> get_nodes_at_depth_aux(std::shared_ptr<QCNode> node, int depth, std::vector<std::shared_ptr<QCNode>> acum){
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


        void get_ponderated_board(){
            std::vector<Eigen::MatrixXi> boards = {};
            for(auto node : get_nodes_at_depth(this->depth)){
                boards.push_back(node->board->board_matrix);
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
                                if (DEBUG) std::cout << "Error: Different pieces in the same tile" << std::endl;
                            }
                        }
                    }
                    pond_matrix(i, j) = sum/n_boards;
                }
            }
        }


        void compute_tree_score(){
            double sum = 0.0;
            for (int i = 0; i < this->q_board.N_ROWS; ++i) {
                for (int j = 0; j < this->q_board.N_COLS; ++j) {
                    sum += this->q_board.get_score(i,j) * this->pond_matrix(i, j);
                }
            }
            this->score = sum/100;
        } 


};