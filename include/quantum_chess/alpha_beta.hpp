#include "quantum_chess/qc_tree.hpp"
#include "quantum_chess/board.hpp"
#include "quantum_chess/structs.hpp"
#include "quantum_chess/utils.hpp"

class AlphaBeta {
    public:
        int MAX_DEPTH = 5;
        double SPLIT_BONUS = 0.01;
        double COLLAPSE_BONUS = 0.1;

        std::vector<Tile> best_move;


        void search(QCTree tree, std::string turn) {
            search_aux(tree, MAX_DEPTH, -1000, 1000, turn);
        }


        double search_aux(QCTree tree, int depth, double alpha, double beta, std::string turn) {
            if (depth == 0) {
                return tree.score; 
            }

            if (turn == "white") { // Maximizing player
                double maxEval = -1000;
                auto M = tree.get_movements(turn);
                std::vector<std::vector<Tile>> movements = M.first;
                std::vector<std::vector<Tile>> collapse_movements = M.second;

                for (auto move : movements) {
                    QCTree new_tree = tree;
                    new_tree.propagate(move[0], move[1]);

                    double eval = search_aux(new_tree, depth - 1, alpha, beta, "black");
                    if (move.size() == 3) {
                        eval += SPLIT_BONUS;
                    }

                    if (eval > maxEval) {
                        maxEval = eval;
                        if (depth == MAX_DEPTH) { 
                            this->best_move = move;
                        }
                    }

                    alpha = std::max(alpha, eval);
                    if (beta <= alpha) {
                        break; // beta pruning
                    }
                }
                // Collapse moves
                for (auto move : collapse_movements) {
                    QCTree new_tree = tree;
                    new_tree.propagate(move[0], move[1]);
                    new_tree.collapse(move[0]);
                    // If the piece is real, capture the target
                    if(new_tree.q_board.board_matrix(move[0].row, move[0].col) != 0.0){
                        new_tree.propagate(move[0], move[1]);
                    }

                    double eval = search_aux(new_tree, depth - 1, alpha, beta, "black");
                    eval += COLLAPSE_BONUS;

                    if (eval > maxEval) {
                        maxEval = eval;
                        if (depth == MAX_DEPTH) {
                            this->best_move = move;
                        }
                    }

                    alpha = std::max(alpha, eval);
                    if (beta <= alpha) {
                        break; // beta pruning
                    }
                }
                return maxEval;

            } else { // Minimizing player
                double minEval = 1000;
                auto M = tree.get_movements(turn);
                std::vector<std::vector<Tile>> movements = M.first;
                std::vector<std::vector<Tile>> collapse_movements = M.second;

                for (auto move : movements) {
                    QCTree new_tree = tree;
                    new_tree.propagate(move[0], move[1]);

                    double eval = search_aux(new_tree, depth - 1, alpha, beta, "white");
                    if (move.size() == 3) {
                        eval -= SPLIT_BONUS;
                    }
                    
                    if (eval < minEval) {
                        minEval = eval;
                        if (depth == MAX_DEPTH) {
                            this->best_move = move;
                        }
                    }

                    beta = std::min(beta, eval);
                    if (beta <= alpha) {
                        break; // alpha pruning
                    }
                }
                // Collapse moves
                for (auto move : collapse_movements) {
                    QCTree new_tree = tree;
                    new_tree.propagate(move[0], move[1]);
                    new_tree.collapse(move[0]);
                    // If the piece is real, capture the target
                    if(new_tree.q_board.board_matrix(move[0].row, move[0].col) != 0.0){
                        new_tree.propagate(move[0], move[1]);
                    }

                    double eval = search_aux(new_tree, depth - 1, alpha, beta, "white");
                    eval -= COLLAPSE_BONUS;

                    if (eval < minEval) {
                        minEval = eval;
                        if (depth == MAX_DEPTH) {
                            this->best_move = move;
                        }
                    }

                    beta = std::min(beta, eval);
                    if (beta <= alpha) {
                        break; // alpha pruning
                    }
                }
                return minEval;
            }
        }

};