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

int N_ROWS = 6;
int N_COLS = 6;
bool ALLOW_ENTANGLEMENT = false;

// Parámetros configurables
#define EXPLORATION_CONSTANT 1.2
#define SIMULATION_DEPTH 5

class MCTSNode {
public:
    MCTSNode* parent;
    std::vector<MCTSNode*> children;
    double score;
    std::vector<Tile> move;
    int visits;
    
    MCTSNode(MCTSNode* parent = nullptr) : parent(parent), score(0.0), visits(0) {}

    // Método para agregar un hijo
    void addChild(MCTSNode* child) {
        children.push_back(child);
    }
};


std::pair<std::vector<std::vector<Tile>>,std::vector<std::vector<Tile>>> 
			get_movements(QCTree tree, std::string turn){
	std::vector<std::vector<Tile>> movements;
	std::vector<std::vector<Tile>> collapse_movements;

	for (int i = 0; i < N_ROWS; i++){
		for (int j = 0; j < N_COLS; j++){
			Tile source = Tile(i, j);

			if (turn == "white" && tree.q_board.isWhite(source) ||
					turn == "black" && tree.q_board.isBlack(source)){
				std::vector<Tile> validMoves = tree.q_board.getValidMoves(source);
				
				if (ALLOW_ENTANGLEMENT){
					for (std::shared_ptr<Board> board : tree.get_leaf_boards()){
						for (Tile move : board->getValidMoves(source)){
							if (tree.q_board.board_matrix(move.row, move.col) == gap || 
									tree.q_board.board_matrix(move.row, move.col) == board->board_matrix(source.row, source.col)){
								validMoves.push_back(move);
							}
						}
					}
				}
				

				for (Tile target1 : validMoves){
					// Check if the move would be a quantum capture
					if (tree.pond_matrix(source.row, source.col) != 1.00
						&& ((turn=="white" && tree.q_board.isBlack(target1))
						|| (turn=="black" && tree.q_board.isWhite(target1)))){
							
						collapse_movements.push_back({source, target1});
						continue;
					} else {
						// Get simple moves
						movements.push_back({source, target1});
					}

					// Get split moves only to gap targets
					if (tree.q_board.board_matrix(target1.row, target1.col) != gap){
						continue;
					}
					for (Tile target2 : validMoves){
						if (!(target1==target2) && tree.q_board.board_matrix(target2.row, target2.col) == gap){
							movements.push_back({source, target1, target2});
						}
					}
				}
			} 
		}
	}

	return {movements, collapse_movements};
}

GameState check_state(QCTree& tree) {
	bool white_king = false;
	bool black_king = false;
    GameState state;

	for (int i = 0; i < N_ROWS && !(white_king && black_king); i++){
		for (int j = 0; j < N_COLS && !(white_king && black_king); j++){
			if (tree.q_board.board_matrix(i, j) == w_king){
				white_king = true;
			} else if (tree.q_board.board_matrix(i, j) == b_king){
				black_king = true;
			}
		}
	}

	if (!white_king && black_king){
		state = BlackWins;
	} else if (!black_king && white_king){
		state = WhiteWins;
	} else if (!white_king && !black_king){
		state = Draw;
	} else {
		state = Playing;
	}

    return state;
}

// Función de simulación aleatoria
double random_simulation(QCTree& tree, std::string turn) {
    int moves_played = 0;
    std::string initial_turn = turn;
    while (check_state(tree) == Playing && moves_played < SIMULATION_DEPTH) {
    // while (!tree.is_terminal()) {
        // Genera un movimiento aleatorio
        auto movements = get_movements(tree, turn);
        if (movements.first.empty()) {
            break;
        }
        auto random_move = movements.first[rand() % movements.first.size()];
        
        // Aplica el movimiento a la nueva posición
        tree.propagate(random_move[0], random_move[1]);
        
        // Cambia de turno
        turn = (turn == "white") ? "black" : "white";
        moves_played++;
    }


    double final_score = 0.0;
    auto state = check_state(tree);
    if (state == WhiteWins && initial_turn == "white" || state == BlackWins && initial_turn == "black") {
        final_score = 1.0;
    } else if (state == BlackWins && initial_turn == "white" || state == WhiteWins && initial_turn == "black") {
        final_score = - 1.0;
    } else if (state == Draw){
        final_score = 0.5;
    } else {
        final_score = (initial_turn == "white") ? tree.score/100 : -tree.score/100;
    }
    return final_score; // Devuelve la puntuación final del estado del juego

}

// Selección y expansión de los nodos
MCTSNode* select_and_expand(MCTSNode* node, QCTree& tree, std::string turn) {
    // Si el nodo tiene hijos, seleccionamos uno
    if (!node->children.empty()) {
        MCTSNode* best_node = nullptr;
        double best_uct = -std::numeric_limits<double>::infinity();
        
        for (MCTSNode* child : node->children) {
            // Cálculo de UCT para cada hijo
            double uct = (child->score/(child->visits+0.000001)) + EXPLORATION_CONSTANT * sqrt(2 * log(node->visits + 0.0000000001) / (child->visits + 0.00000001));
            if (uct > best_uct) {
                best_uct = uct;
                best_node = child;
            }
        }

        return select_and_expand(best_node, tree, turn); // Recursivamente expandimos
    } else {
        // Si el nodo no tiene hijos, lo expandimos
        auto movements = get_movements(tree, turn).first;
        for (const auto& move : movements) {
            if (move.size() == 2) {
                QCTree new_tree = tree;
                new_tree.propagate(move[0], move[1]);

                MCTSNode* child_node = new MCTSNode(node);
                child_node->move = move;
                node->addChild(child_node);
            }
        }

        return node; // Ahora tenemos los nodos hijos, podemos seleccionar uno y continuar
    }
}

// Función de retropropagación
void backpropagate(MCTSNode* node, double result) {
    while (node != nullptr) {
        node->visits += 1;
        node->score += result; // Actualiza el puntaje del nodo con el resultado
        node = node->parent;
    }
}

// Función principal de MCTS
std::vector<Tile> monte_carlo_tree_search(QCTree& tree, std::string turn, int max_simulations) {
    MCTSNode root(nullptr); // Nodo raíz del MCTS

    // Realizamos simulaciones
    for (int i = 0; i < max_simulations; ++i) {
        MCTSNode* node = &root;
        QCTree new_tree = tree;

        // Selección y expansión
        node = select_and_expand(node, new_tree, turn);

        // Simulación aleatoria desde este nodo
        double result = random_simulation(new_tree, turn);

        // Retropropagación del resultado
        backpropagate(node, result);
    }

    // Elegir el mejor movimiento basado en el nodo más visitado
    MCTSNode* best_node = nullptr;
    double max_score = -std::numeric_limits<double>::infinity();
    for (MCTSNode* child : root.children) {
        std::cout << "score: " << child->score << " visits: " << child->visits << std::endl;
        if (child->score/(child->visits+0.1) > max_score) {
            max_score = child->score/(child->visits+0.1);
            best_node = child;
        }
    }
    
    // for (MCTSNode* child : root.children) {
    //     if (child->visits > max_visits) {
    //         max_visits = child->visits;
    //         best_node = child;
    //     }
    // }

    std::cout << "max score: " << max_score << std::endl;
    std::cout << "hijos: " <<  root.children.size() << std::endl;

    return best_node->move; // Retorna el mejor movimiento según las simulaciones
}
