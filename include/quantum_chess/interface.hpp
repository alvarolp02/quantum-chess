#include <iostream>
#include <SFML/Graphics.hpp>


class Interface {
    public:
        Interface();
        void openWindow();
        void loadBoard();
        void loadPieces();
        void loadMovements();

        sf::RenderWindow window;

    private:
        int TILE_SIZE = 100;
        int BOARD_SIZE = 8;
};