#ifndef QCTREE_H
#define QCTREE_H

#include <iostream>
#include "quantum_chess/qc_node.hpp"

class QCTree {
    public:
        QCTree(Board B);

        void propagate(Tile source, Tile target);

        void split(Tile source, Tile target1, Tile target2);

        void print_tree();

        void get_ponderated_board();

        Eigen::Matrix<double, 8, 8> pond_board;
        Eigen::Matrix<int, 8, 8> q_board;

    private: 
        void print_tree_aux(QCNode* node, std::string prefix, bool split);

        std::vector<Eigen::Matrix<int, 8, 8>> get_all_boards();

        std::vector<Eigen::Matrix<int, 8, 8>> get_all_boards_aux(QCNode* node, 
                                                            std::vector<Eigen::Matrix<int, 8, 8>> acum);

        QCNode* root;
};

#endif // QCTREE