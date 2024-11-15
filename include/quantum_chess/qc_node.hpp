#ifndef QCNODE_H
#define QCNODE_H

#include <iostream>
#include "board.hpp"

class QCNode {
    public:
        QCNode(Board B, int index);

        void propagate(Tile source, Tile target);

        void split(Tile source, Tile target1, Tile target2);

        int index;
        Board board;
        QCNode* left;
        QCNode* right;
};

#endif // QCNODE_H