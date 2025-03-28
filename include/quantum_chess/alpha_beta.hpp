#include "quantum_chess/qc_tree.hpp"
#include "quantum_chess/board.hpp"
#include "quantum_chess/structs.hpp"
#include "quantum_chess/utils.hpp"

class AlphaBeta {
    public:
        int MAX_DEPTH = 5;

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

                for (auto move : tree.get_movements(turn).first) {
                    if (move.size() == 2) {
                        QCTree new_tree = tree;
                        new_tree.propagate(move[0], move[1]);

                        double eval = search_aux(new_tree, depth - 1, alpha, beta, "black");
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
                }
                return maxEval;

            } else { // Minimizing player
                double minEval = 1000;

                for (auto move : tree.get_movements(turn).first) {
                    if (move.size() == 2) {
                        QCTree new_tree = tree;
                        new_tree.propagate(move[0], move[1]);

                        double eval = search_aux(new_tree, depth - 1, alpha, beta, "white");

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
                }
                return minEval;
            }
        }

};