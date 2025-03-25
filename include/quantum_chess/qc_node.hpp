#pragma once

#include <iostream>
#include "quantum_chess/board.hpp"

class QCNode {
    public:

        int index;
        Board* board;
        QCNode* left;
        QCNode* right;

        // Default constructor
        QCNode(): index(0), left(nullptr), right(nullptr) {}

        // Constructor
        QCNode(Board* B, int index){
            this->board = B;
            this->index = index;
            this->left = nullptr;
            this->right = nullptr;
        }

        // Deep copy constructor
        QCNode(const QCNode& other): index(other.index), board(other.board),
                                        left(nullptr), right(nullptr) {
            if (other.board != nullptr)
                board = new Board(*other.board);
            if (other.left != nullptr)
                left = new QCNode(*other.left);
            if (other.right != nullptr)
                right = new QCNode(*other.right);
        }

        // Deep copy assignment operator
        QCNode& operator=(const QCNode& other) {
            if (this == &other)
                return *this; // Avoid self-assignment
            clear(); // Free memory
            index = other.index;
            if (other.board != nullptr)
                board = new Board(*other.board);
            else 
                board = nullptr;
            // Deep copy left subtree
            if (other.left != nullptr)
                left = new QCNode(*other.left);
            else
                left = nullptr;
            // Deep copy right subtree
            if (other.right != nullptr)
                right = new QCNode(*other.right);
            else
                right = nullptr;

            return *this;
        }

        // Destructor
        ~QCNode() {
            clear();
        }

        
        // Split all leaf nodes in two
        void split(Tile source, Tile target1, Tile target2){
            if(this->left == nullptr && this->right == nullptr){
                Board* new_board1 = new Board(this->board->board_matrix);
                new_board1->movePiece(source, target1);
                Board* new_board2 = new Board(this->board->board_matrix);
                new_board2->movePiece(source, target2);
                this->left = new QCNode(new_board1, this->index+1);
                this->right = new QCNode(new_board2, this->index+1);
            }else{
                this->left->split(source, target1, target2);
                this->right->split(source, target1, target2);
            }
        }

    private:
        // Free memory
        void clear() {
            // if (left != nullptr) {
            //     std::cout << "Deleting left: " << left << std::endl;
            //     delete left;
            //     left = nullptr;
            // }
            // if (right != nullptr) {
            //     std::cout << "Deleting right: " << right << std::endl;
            //     delete right;
            //     right = nullptr;
            // }
            // if (board != nullptr) {
            //     std::cout << "Deleting board: " << board << std::endl;
            //     // abort();
            //     delete board;
            //     board = nullptr;
            // }
        }
        
};