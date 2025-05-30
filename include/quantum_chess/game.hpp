#include "quantum_chess/qc_tree.hpp"
#include "quantum_chess/interface.hpp"
#include "quantum_chess/board.hpp"
#include "quantum_chess/qc_node.hpp"
#include "quantum_chess/utils.hpp"
#include "quantum_chess/MCTS.hpp"
#include "quantum_chess/alpha_beta.hpp"
#include <thread>
#include <random>
#include <filesystem>
#include <yaml-cpp/yaml.h>

class Game {
    public:
        int N_ROWS;
        int N_COLS;
        GameState state_ = Playing;
        std::string turn_ = "white";
        GamePlayer WHITE_PLAYER = Human;
        GamePlayer BLACK_PLAYER = Human; 

	    QCTree tree_;
        Interface* interface_;
        Tile selected_piece_ = Tile(-1, -1);
	    std::vector<std::vector<Tile>> movements_;
	    std::vector<std::vector<Tile>> collapse_movements_;

        Game(int argc, char * argv[]);

        bool DEBUG = false;

        void human_turn();
        void bot_turn();
        void get_movements();

        GamePlayer current_player(){
            return turn_ == "white" ? WHITE_PLAYER : BLACK_PLAYER;
        }

        void print_interface(){
            interface_->loadBoard();
            interface_->loadPieces(tree_.q_board.board_matrix);
            interface_->loadPonderation(tree_.pond_matrix);
            interface_->window.display();
        }
            
        void print_interface(std::vector<Tile> movements){
            interface_->loadBoard();
            interface_->loadPieces(tree_.q_board.board_matrix);
            interface_->loadPonderation(tree_.pond_matrix);
            interface_->loadMovements(movements);
            interface_->window.display();
        }

        Eigen::MatrixXi load_config(const std::string& filename){
            YAML::Node config = YAML::LoadFile(filename);
            std::map<char, int> pieceToInt = {
              {'.', 0},  {'P', 1},  {'R', 2},  {'N', 3},  {'B', 4}, {'Q', 5},  {'K', 6},
              {'p', 7}, {'r', 8}, {'n', 9}, {'b', 10}, {'q', 11}, {'k', 12} };
          
          
            auto values = config["board"];
            
            int n = values.size();
            int m = values[0].size();
          
            Eigen::MatrixXi matrix = Eigen::MatrixXi::Zero(n, m);
          
          
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < m; ++j) {
                  matrix(i, j) = pieceToInt[values[i][j].as<char>()];
                }
            }
          
            return matrix;
        }

    private:

};