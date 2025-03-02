#include <iostream>
#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include "quantum_chess/qc_tree.hpp"
#include "quantum_chess/structs.hpp"


class Interface {
    public:
        int TILE_SIZE = 100;
        int BOARD_SIZE;

        sf::RenderWindow window;

        // Constructors
        Interface();
        Interface(int);

        void openWindow();
        void loadBoard();
        void loadTree(QCTree);
        void loadPieces(Eigen::MatrixXi);
        void loadPonderation(Eigen::MatrixXd);
        void loadMovements(std::vector<Tile>);
        std::vector<Tile> waitForInput();
};