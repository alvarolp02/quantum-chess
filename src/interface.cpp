#include "quantum_chess/interface.hpp"
#include <thread>

Interface::Interface() {
    window.create(sf::VideoMode(TILE_SIZE * BOARD_SIZE, TILE_SIZE * BOARD_SIZE), "Tablero de Ajedrez");
    
}

void Interface::openWindow() {
    while (window.isOpen()) {
    }
}

std::vector<Tile> Interface::waitForInput(){
    sf::Event event;
    std::vector<Tile> movements;
    while (window.waitEvent(event)) {
        // Close window
        if (event.type == sf::Event::Closed)
            window.close();

        // Detect mouse click
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.y / TILE_SIZE; // Rows
                int y = event.mouseButton.x / TILE_SIZE; // Columns

                return {Tile(x, y)};
            } else if (event.mouseButton.button == sf::Mouse::Right) {
                int x = event.mouseButton.y / TILE_SIZE; // Rows
                int y = event.mouseButton.x / TILE_SIZE; // Columns
                if (movements.size() == 0) {
                    movements.push_back(Tile(x, y));
                } else if (movements.size() == 1) {
                    movements.push_back(Tile(x, y));
                    return movements;
                }
            }
        }
    }
    return {Tile(-1, -1)};
}

void Interface::loadBoard(){
    sf::Color white(255, 255, 255);
    sf::Color black(100, 100, 100);

    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));

    window.clear();
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            tile.setPosition(j * TILE_SIZE, i * TILE_SIZE);
            tile.setFillColor((i + j) % 2 == 0 ? white : black); // Alternate colors
            window.draw(tile);
        }
    }
}

void Interface::loadTree(QCTree tree){
    
}

void Interface::loadPieces(Eigen::Matrix<int, 8, 8> board){

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board(i, j) == 0) {
                continue;
            }
            sf::Texture tileTexture;

            std::string path = "../assets/";
            switch (board(i, j)) {
                case 1:
                    path += "white-pawn.png";
                    break;
                case 2:
                    path += "white-rook.png";
                    break;
                case 3:
                    path += "white-knight.png";
                    break;
                case 4:
                    path += "white-bishop.png";
                    break;
                case 5:
                    path += "white-queen.png";
                    break;
                case 6:
                    path += "white-king.png";
                    break;
                case 7:
                    path += "black-pawn.png";
                    break;
                case 8:
                    path += "black-rook.png";
                    break;
                case 9:
                    path += "black-knight.png";
                    break;
                case 10:
                    path += "black-bishop.png";
                    break;
                case 11:
                    path += "black-queen.png";
                    break;
                case 12:
                    path += "black-king.png";
                    break;
            }

            if (!tileTexture.loadFromFile(path)) {
                std::cerr << "Error loading images." << std::endl;
                return;
            } 
            sf::Sprite tileSprite(tileTexture);
            tileSprite.setScale(
                            float(TILE_SIZE) / tileTexture.getSize().x, 
                            float(TILE_SIZE) / tileTexture.getSize().y
                        );
            tileSprite.setPosition(j * TILE_SIZE, i * TILE_SIZE);
            window.draw(tileSprite);
        }
    }

}

void Interface::loadMovements(std::vector<Tile> movements){
    sf::Color green(0, 255, 0);

    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    sf::CircleShape circle(TILE_SIZE/2,60);

    for (int i = 0; i < movements.size(); i++) {
        tile.setPosition(movements[i].col * TILE_SIZE, movements[i].row * TILE_SIZE);
        tile.setOutlineColor(green);
        tile.setOutlineThickness(5);
        tile.setFillColor(sf::Color::Transparent);
        window.draw(tile);

        // circle.setPosition(movements[i].col * TILE_SIZE, movements[i].row * TILE_SIZE);
        // circle.setOutlineColor(green);
        // circle.setOutlineThickness(5);
        // circle.setFillColor(sf::Color::Transparent);
        // window.draw(circle);
    }
}