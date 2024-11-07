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

        void print_tree_aux(QCNode* node, std::string prefix, bool split);

        QCNode* root;
};

#endif // QCTREE