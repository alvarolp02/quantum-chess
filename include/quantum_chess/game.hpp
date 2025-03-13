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
	    QCTree tree_;
	    std::vector<std::vector<Tile>> movements_;
	    std::vector<std::vector<Tile>> collapse_movements_;

        Game(const std::string& config_file);

        bool ALLOW_ENTANGLEMENT = false;

        void get_movements(std::string turn);

        void print_interface(Interface* interface, QCTree* tree){
            interface->loadBoard();
            interface->loadPieces(tree->q_board.board_matrix);
            interface->loadPonderation(tree->pond_matrix);
            interface->window.display();
        }
            
        void print_interface(Interface* interface, QCTree* tree, std::vector<Tile> movements){
            interface->loadBoard();
            interface->loadPieces(tree->q_board.board_matrix);
            interface->loadPonderation(tree->pond_matrix);
            interface->loadMovements(movements);
            interface->window.display();
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