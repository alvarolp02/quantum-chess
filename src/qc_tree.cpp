#include "quantum_chess/qc_tree.hpp"

std::random_device rd;
std::mt19937 gen(rd());
std::bernoulli_distribution dist(0.5);

QCTree::QCTree(){
    Board q_board = Board();
    this->root = new QCNode(Board(),0);
}

void QCTree::propagate(Tile source, Tile target){
    this->root->propagate(source, target);

    // Update splits positions
    for (int i = 0; i < splits.size(); ++i) {
        if(splits[i].piece == source){
            splits[i].piece = target;
            break;
        }
    }
}

void QCTree::split(Tile source, Tile target1, Tile target2){

    Split s1;
    s1.depth = depth;
    s1.piece = target1;
    splits.push_back(s1);

    Split s2;
    s2.depth = depth;
    s2.piece = target2;
    splits.push_back(s2);

    this->root->split(source, target1, target2);
    depth++;
}

void QCTree::collapse(Tile piece){
    std::cout << "Collapsing..." << std::endl;
    for (int i = 0; i < splits.size(); ++i) {
        // Search the split related to the actual piece
        if(splits[i].piece == piece){
            // Collapse all nodes at depth of split
            for (auto node : get_nodes_at_depth(splits[i].depth)){
                // Delete one of the children randomly
                bool random_boolean = dist(gen);
                if (random_boolean){
                    delete_node(node->right_); 
                    node->index_ = node->left_->index_;
                    node->board_ = node->left_->board_;
                    node->right_ = node->left_->right_;
                    node->left_ = node->left_->left_;
                }else{
                    delete_node(node->left_); 
                    node->index_ = node->right_->index_;
                    node->board_ = node->right_->board_;
                    node->left_ = node->right_->left_;
                    node->right_ = node->right_->right_;
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

// Delete node and all its children
void QCTree::delete_node(QCNode* node){
    if(node == nullptr){
        return;
    }
    delete_node(node->left_);
    delete_node(node->right_);
    
    delete node;
}

std::vector<QCNode*> QCTree::get_nodes_at_depth(int depth){
    std::vector<QCNode*> nodes = get_nodes_at_depth_aux(this->root, depth, {});
    return nodes;
}

std::vector<QCNode*> QCTree::get_nodes_at_depth_aux(QCNode* node, int depth, std::vector<QCNode*> acum){
    if(node==nullptr){
        return acum;
    }

    if(node->index_ == depth){
        acum.push_back(node);
    }

    if(node->left_ != nullptr && node->right_ != nullptr){
        auto acum_aux = get_nodes_at_depth_aux(node->left_, depth, acum);
        acum = get_nodes_at_depth_aux(node->right_, depth, acum_aux);
    }

    return acum;
}

void QCTree::print_tree(){
    print_tree_aux(this->root, "");
    
    std::cout  << std::endl << std::endl;
}

void QCTree::print_tree_aux(QCNode* node, std::string prefix){
    if(node==nullptr){
        return;
    }

    std::cout << std::endl;
    std::cout << prefix;
    std::cout << "└─ ";
    prefix += "    ";
    std::cout << node->index_;

    print_tree_aux(node->left_, prefix);
    print_tree_aux(node->right_, prefix);
    
}

void QCTree::get_ponderated_board(){
    std::vector<Eigen::Matrix<int, 8, 8>> boards = {};
    for(auto node : get_nodes_at_depth(depth)){
        boards.push_back(node->board_.board_matrix_);
    }
    int n_boards = boards.size();

    pond_matrix = Eigen::Matrix<double, 8, 8>::Zero();
    q_board.board_matrix_ = Eigen::Matrix<int, 8, 8>::Zero();

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            double sum = 0.0;
            for (auto board : boards) {
                if (board(i, j) != 0) {
                    sum+=1;
                    if(q_board.board_matrix_(i, j) == 0){
                        q_board.board_matrix_(i, j) = board(i, j);
                    } else if (q_board.board_matrix_(i, j) != board(i, j)){
                        std::cout << "Error: Different pieces in the same tile" << std::endl;
                    }
                }
            }
            pond_matrix(i, j) = sum/n_boards;
        }
    }

}


