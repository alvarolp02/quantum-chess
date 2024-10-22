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
    sf::Color black(0, 0, 0);

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

void Interface::loadPieces(){
    sf::Texture tileTexture;
    if (!tileTexture.loadFromFile("../assets/white-pawn.png")) {
        std::cerr << "Error al cargar la imagen." << std::endl;
        return;
    } 
    sf::Sprite tileSprite(tileTexture);
    tileSprite.setScale(
                    float(TILE_SIZE) / tileTexture.getSize().x, 
                    float(TILE_SIZE) / tileTexture.getSize().y
                );
    tileSprite.setPosition(0,TILE_SIZE);
    window.draw(tileSprite);

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