#include <iostream>
#include <SFML/Graphics.hpp>
#include <Eigen/Dense>


class Interface {
    public:
        Interface();
        void openWindow();
        void loadBoard();
        void loadPieces(Eigen::Matrix<int, 8, 8>);
        void loadMovements(std::vector<std::pair<int,int>>);
        std::pair<int,int> waitForInput();

        sf::RenderWindow window;

    private:
        int TILE_SIZE = 100;
        int BOARD_SIZE = 8;
};