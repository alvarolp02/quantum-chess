#include "quantum_chess/qc_tree.hpp"
#include "quantum_chess/board.hpp"
#include "quantum_chess/structs.hpp"
#include "quantum_chess/utils.hpp"
#include <ctime>

class AlphaBeta {
    public:
        int MAX_DEPTH = 5;
        bool ALLOW_QUANTUM = true;
        double SPLIT_BONUS = 0.01;
        double COLLAPSE_BONUS = 0.1;
        double time = 0.0;
        int it = 0;

        std::vector<Tile> best_move;


        void search(QCTree tree, std::string turn) {
            auto start = omp_get_wtime();
            search_aux(tree, MAX_DEPTH, -1000, 1000, turn);
            this->time = omp_get_wtime() - start;
        }


        double search_aux(QCTree tree, int depth, double alpha, double beta, std::string turn) {
            this->it++;

            if (depth == 0) {
                return tree.score; 
            }

            auto M = tree.get_movements(turn);
            std::vector<std::vector<Tile>> movements = M.first;
            std::vector<std::vector<Tile>> collapse_movements = M.second;
            std::vector<std::vector<Tile>> all_movements = movements;

            if (ALLOW_QUANTUM) all_movements.insert(all_movements.end(), collapse_movements.begin(), collapse_movements.end());
            
            double bestEval = turn == "white" ? -1000 : 1000;

            for (int i = 0; i < all_movements.size(); ++i) {
                const auto& move = all_movements[i];
                bool is_split = (move.size() == 3);
                bool is_collapse = (i > movements.size() - 1);

                if (!ALLOW_QUANTUM && is_split) continue;

                QCTree new_tree = tree;
                if (is_collapse) { // Collapse move
                    new_tree.propagate(move[0], move[1]);
                    new_tree.collapse(move[0]);
                    // If the piece is real, capture the target
                    if(new_tree.q_board.board_matrix(move[0].row, move[0].col) != 0.0){
                        new_tree.propagate(move[0], move[1]);
                    }
                } else {
                    new_tree.propagate(move[0], move[1]);
                }
                
                std::string next_turn = turn == "white" ? "black" : "white";
                double eval = search_aux(new_tree, depth - 1, alpha, beta, next_turn);
                
                if (is_split && turn == "white") {
                    eval += SPLIT_BONUS;
                } else if (is_split && turn == "black") {
                    eval -= SPLIT_BONUS;
                } else if (is_collapse && turn == "white") {
                    eval += COLLAPSE_BONUS;
                } else if (is_collapse && turn == "black") {
                    eval -= COLLAPSE_BONUS;
                }

                if (turn == "white") { // Maximizing player
                    if (eval > bestEval) {
                        bestEval = eval;
                        if (depth == MAX_DEPTH) this->best_move = move;
                    }

                    alpha = std::max(alpha, eval);
                    if (beta <= alpha) break; // beta pruning

                } else { // Minimizing player
                    if (eval < bestEval) {
                        bestEval = eval;
                        if (depth == MAX_DEPTH) this->best_move = move;
                    }

                    beta = std::min(beta, eval);
                    if (beta <= alpha) break; // alpha pruning

                }
            }
                
            return bestEval;
        }

};