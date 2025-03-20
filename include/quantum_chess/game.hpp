#include "quantum_chess/qc_tree.hpp"
#include "quantum_chess/interface.hpp"
#include "quantum_chess/board.hpp"
#include "quantum_chess/qc_node.hpp"
#include "quantum_chess/utils.hpp"
#include <thread>
#include <random>
#include <filesystem>
#include <yaml-cpp/yaml.h>

class Game {
    public:
        int N_ROWS;
        int N_COLS;
        std::string turn_ = "white";
        std::string WHITE_PLAYER = "bot";
        std::string BLACK_PLAYER = "bot";

	    QCTree tree_;
        Interface* interface_;
        Tile selected_piece_ = Tile(-1, -1);
	    std::vector<std::vector<Tile>> movements_;
	    std::vector<std::vector<Tile>> collapse_movements_;

        Game(const std::string& config_file);

        bool ALLOW_ENTANGLEMENT = true;

        void human_turn();
        void bot_turn();
        std::vector<Tile> explore_tree(QCTree tree, int depth, std::string turn);
        std::vector<Tile> alpha_beta(QCTree tree, int depth, double alpha, double beta,
                                        std::string turn, std::vector<Tile> prev_move);
        void get_movements();
        std::pair<std::vector<std::vector<Tile>>,std::vector<std::vector<Tile>>> 
                get_movements(QCTree tree, std::string turn);

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