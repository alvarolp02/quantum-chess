#ifndef QCTREE_H
#define QCTREE_H

#include <iostream>
#include "quantum_chess/qc_node.hpp"
#include <random> 

class QCTree {
    public:
        QCTree();

        void propagate(Tile source, Tile target);

        void split(Tile source, Tile target1, Tile target2);

        void collapse(Tile piece);

        void print_tree();

        void get_ponderated_board();

        Eigen::Matrix<double, 8, 8> pond_matrix;
        Board q_board;
        int depth = 0;
        std::vector<Split> splits;

    private: 
        void print_tree_aux(QCNode* node, std::string prefix, bool split);

        void delete_node(QCNode* node);

        std::vector<QCNode*> get_nodes_at_depth(int depth);
        std::vector<QCNode*> get_nodes_at_depth_aux(QCNode* node, int depth, std::vector<QCNode*> acum);

        std::vector<Eigen::Matrix<int, 8, 8>> get_all_boards();

        std::vector<Eigen::Matrix<int, 8, 8>> get_all_boards_aux(QCNode* node, 
                                                            std::vector<Eigen::Matrix<int, 8, 8>> acum);

        QCNode* root;

};

#endif // QCTREE