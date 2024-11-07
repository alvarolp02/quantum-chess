#include "quantum_chess/qc_tree.hpp"

QCTree::QCTree(){
    Board q_board = Board();
    this->root = new QCNode(Board(),0);
}

void QCTree::propagate(Tile source, Tile target){
    this->root->propagate(source, target);
}

void QCTree::split(Tile source, Tile target1, Tile target2){
    this->root->split(source, target1, target2);
}

void QCTree::print_tree(){
    print_tree_aux(this->root, "", false);
    
    std::cout  << std::endl << std::endl << std::endl;
}

void QCTree::print_tree_aux(QCNode* node, std::string prefix, bool split = false){
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

std::vector<Eigen::Matrix<int, 8, 8>> QCTree::get_all_boards(){
    std::vector<Eigen::Matrix<int, 8, 8>> boards = get_all_boards_aux(root, {});
    return boards;
}

std::vector<Eigen::Matrix<int, 8, 8>> QCTree::get_all_boards_aux(QCNode* node, 
                                                    std::vector<Eigen::Matrix<int, 8, 8>> acum){
    if(node->next != nullptr){
        acum = get_all_boards_aux(node->next, acum);
    }else if(node->split1 != nullptr && node->split2 != nullptr){
        auto acum_aux = get_all_boards_aux(node->split1, acum);
        acum = get_all_boards_aux(node->split2, acum_aux);
    } else {
        acum.push_back(node->board.board_matrix);
    }

    return acum;
}

void QCTree::get_ponderated_board(){
    auto boards = get_all_boards();
    int n_boards = boards.size();

    pond_matrix = Eigen::Matrix<double, 8, 8>::Zero();
    q_board.board_matrix = Eigen::Matrix<int, 8, 8>::Zero();

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            double sum = 0.0;
            for (auto board : boards) {
                if (board(i, j) != 0) {
                    sum+=1;
                    if(q_board.board_matrix(i, j) == 0){
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


