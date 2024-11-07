#include <iostream>
#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include "quantum_chess/qc_tree.hpp"
#include "quantum_chess/structs.hpp"


class Interface {
    public:
        Interface();
        void openWindow();
        void loadBoard();
        void loadTree(QCTree);
        void loadPieces(Eigen::Matrix<int, 8, 8>);
        void loadMovements(std::vector<Tile>);
        std::vector<Tile> waitForInput();

        sf::RenderWindow window;

    private:
        int TILE_SIZE = 100;
        int BOARD_SIZE = 8;
};