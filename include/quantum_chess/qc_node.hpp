#pragma once

#include <iostream>
#include <memory> 
#include "quantum_chess/board.hpp"

class QCNode {
    public:

        int index;
        std::shared_ptr<Board> board;
        std::shared_ptr<QCNode> left;
        std::shared_ptr<QCNode> right;

        // Default constructor
        QCNode() : index(0), left(nullptr), right(nullptr) {}

        // Constructor
        QCNode(std::shared_ptr<Board> B, int index)
            : board(B), index(index), left(nullptr), right(nullptr) {}

        // Deep copy constructor
        QCNode(const QCNode& other) : index(other.index), left(nullptr), right(nullptr) {
            if (other.board != nullptr)
                board = std::make_shared<Board>(*other.board);
            if (other.left != nullptr)
                left = std::make_shared<QCNode>(*other.left);
            if (other.right != nullptr)
                right = std::make_shared<QCNode>(*other.right);
        }

        // Deep copy assignment operator
        QCNode& operator=(const QCNode& other) {
            if (this == &other)
                return *this; // Avoid self-assignment

            index = other.index;

            if (other.board != nullptr)
                board = std::make_shared<Board>(*other.board);
            else 
                board = nullptr;

            // Deep copy left subtree
            if (other.left != nullptr)
                left = std::make_shared<QCNode>(*other.left);
            else
                left = nullptr;

            // Deep copy right subtree
            if (other.right != nullptr) 
                right = std::make_shared<QCNode>(*other.right);
            else
                right = nullptr;

            return *this;
        }

        // Destructor
        ~QCNode() {
            // No need to manually delete anything, std::shared_ptr will handle it
        }

        // Split all leaf nodes in two
        void split(Tile source, Tile target1, Tile target2) {
            if (this->left == nullptr && this->right == nullptr) {
                std::shared_ptr<Board> new_board1 = std::make_shared<Board>(this->board->board_matrix);
                new_board1->movePiece(source, target1);
                std::shared_ptr<Board> new_board2 = std::make_shared<Board>(this->board->board_matrix);
                new_board2->movePiece(source, target2);
                this->left = std::make_shared<QCNode>(new_board1, this->index + 1);
                this->right = std::make_shared<QCNode>(new_board2, this->index + 1);
            } else {
                this->left->split(source, target1, target2);
                this->right->split(source, target1, target2);
            }
        }

    private:
};  
