#include <vector>
#include <string>
#include <limits>
#include <ctime>
#include <cstdlib>
#include "quantum_chess/qc_tree.hpp"
#include "quantum_chess/interface.hpp"
#include "quantum_chess/board.hpp"
#include "quantum_chess/qc_node.hpp"
#include "quantum_chess/utils.hpp"


class MCTSNode {
public:
    MCTSNode* parent;
    std::vector<MCTSNode*> children;
    double score;
    std::vector<Tile> move;
    int visits;
    
    MCTSNode(MCTSNode* parent = nullptr) : parent(parent), score(0.0), visits(0) {}
};


class MCTS {
    public:

        double EXPLORATION_CONSTANT = 1.2;
        int SIMULATION_DEPTH = 50;
        int MAX_SIMULATIONS = 10000;

        std::vector<Tile> best_move;

        MCTS() {}

        void search(QCTree& tree, std::string turn) {
            MCTSNode root(nullptr);

            // Compute simulations
            for (int i = 0; i < MAX_SIMULATIONS; ++i) {
                MCTSNode* node = &root;
                QCTree new_tree = tree;

                // Selection and expansion
                node = select_and_expand(node, new_tree, turn);

                // Random simulation from the node chosen
                double result = random_simulation(new_tree, turn);

                // Backpropagation of the result of the simulation
                backpropagate(node, result);
            }

            // Choose best move based on the score of the children
            MCTSNode* best_node = nullptr;
            double max_score = -std::numeric_limits<double>::infinity();
            for (MCTSNode* child : root.children) {
                // std::cout << "score: " << child->score << " visits: " << child->visits << std::endl;
                if (child->score/(child->visits+0.1) > max_score) {
                    max_score = child->score/(child->visits+0.1);
                    best_node = child;
                }
            }

            // std::cout << "max score: " << max_score << std::endl;

            best_move = best_node->move; 
        }

    private: 
    
        double random_simulation(QCTree& tree, std::string turn) {
            double moves_played = 0;
            std::string initial_turn = turn;
            while (tree.state == Playing && moves_played < SIMULATION_DEPTH) {
                auto movements = tree.get_movements(turn);
                if (movements.first.empty()) {
                    break;
                }
                auto random_move = movements.first[rand() % movements.first.size()];
                
                tree.propagate(random_move[0], random_move[1]);
                
                turn = (turn == "white") ? "black" : "white";
                moves_played++;
            }


            double final_score = 0.0;
            if (tree.state == WhiteWins && initial_turn == "white" || tree.state == BlackWins && initial_turn == "black") {
                final_score = 1.0;
            } else if (tree.state == BlackWins && initial_turn == "white" || tree.state == WhiteWins && initial_turn == "black") {
                final_score = - 1.0;
            } else if (tree.state == Draw){
                final_score = 0.5;
            } else {
                // Score must be positive for the current player
                final_score = (initial_turn == "white") ? tree.score : -tree.score; 
            }

            // Greater score if the game is won faster
            return final_score*(double(SIMULATION_DEPTH)/(moves_played+0.001)); 

        }


        
        MCTSNode* select_and_expand(MCTSNode* node, QCTree& tree, std::string turn) {
            // Select node based on uct until a leaf node is reached
            if (!node->children.empty()) {
                MCTSNode* best_node = nullptr;
                double best_uct = -std::numeric_limits<double>::infinity();
                
                for (MCTSNode* child : node->children) {
                    double uct = (child->score/(child->visits+0.000001)) + EXPLORATION_CONSTANT * sqrt(2 * log(node->visits + 0.0000000001) / (child->visits + 0.00000001));
                    if (uct > best_uct) {
                        best_uct = uct;
                        best_node = child;
                    }
                }
        
                return select_and_expand(best_node, tree, turn); // Recursivamente expandimos
            } else {
                // When we reach a leaf node, we expand it
                auto movements = tree.get_movements(turn).first;
                for (const auto& move : movements) {
                    if (move.size() == 2) {
                        QCTree new_tree = tree;
                        new_tree.propagate(move[0], move[1]);
        
                        MCTSNode* child_node = new MCTSNode(node);
                        child_node->move = move;
                        node->children.push_back(child_node);
                    }
                }
        
                return node;
            }
        }
        
        void backpropagate(MCTSNode* node, double result) {
            while (node != nullptr) {
                node->visits += 1;
                node->score += result; // Accumulate the result of the simulation in the score
                node = node->parent;
            }
        }

};





