#include "quantum_chess/interface.hpp"
#include <thread>

Interface::Interface() {
    // Crear la ventana
    window.create(sf::VideoMode(TILE_SIZE * BOARD_SIZE, TILE_SIZE * BOARD_SIZE), "Tablero de Ajedrez");
    
}

void Interface::openWindow() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Cerrar la ventana
            if (event.type == sf::Event::Closed)
                window.close();
            
            // Detectar clic del mouse
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = event.mouseButton.x / TILE_SIZE; // Columna
                    int y = event.mouseButton.y / TILE_SIZE; // Fila
                    std::cout << "Clic en la casilla: (" << x << ", " << y << ")" << std::endl;
                }
            }
        }
    }
}

void Interface::loadBoard(){
    // Definir colores
    sf::Color white(255, 255, 255);
    sf::Color black(100, 100, 100);

    // Crear el tablero
    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));

    // // Dibujar el tablero
    window.clear();
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            tile.setPosition(j * TILE_SIZE, i * TILE_SIZE);
            tile.setFillColor((i + j) % 2 == 0 ? white : black); // Alternar colores
            window.draw(tile);
        }
    }
    // window.display();
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
                std::cerr << "Error al cargar la imagen." << std::endl;
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

void Interface::loadMovements(){
    // Definir colores
    sf::Color green(0, 255, 0);

    // Crear el tablero
    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));

    tile.setPosition(0,0);
    tile.setFillColor(green);
    window.draw(tile);
    // // Dibujar el tablero
    // window.clear();
    // for (int i = 0; i < BOARD_SIZE; ++i) {
    //     for (int j = 0; j < BOARD_SIZE; ++j) {
    //         tile.setPosition(j * TILE_SIZE, i * TILE_SIZE);
    //         tile.setFillColor((i + j) % 2 == 0 ? green : green); // Alternar colores
    //         window.draw(tile);
    //     }
    // }
    std::cout << "Displaying movements" << std::endl;
}