#include "quantum_chess/interface.hpp"
#include <thread>
#include <sstream>
#include <iomanip>

Interface::Interface() {
    N_ROWS = 8;
    N_COLS = 8;
    BOT = false;

    window.create(sf::VideoMode(TILE_SIZE * N_COLS, TILE_SIZE * N_ROWS), "Quantum Chess");
}

Interface::Interface(int rows, int cols) {
    N_ROWS = rows;
    N_COLS = cols;

    window.create(sf::VideoMode(TILE_SIZE * N_COLS, TILE_SIZE * N_ROWS), "Quantum Chess");
}

void Interface::openWindow() {
    while (window.isOpen()) {
        if (BOT) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }
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
    for (int i = 0; i < N_ROWS; ++i) {
        for (int j = 0; j < N_COLS; ++j) {
            tile.setPosition(j * TILE_SIZE, i * TILE_SIZE);
            tile.setFillColor((i + j) % 2 == 0 ? white : black); // Alternate colors
            window.draw(tile);
        }
    }
}


void Interface::loadPieces(Eigen::MatrixXi board){

    for (int i = 0; i < N_ROWS; ++i) {
        for (int j = 0; j < N_COLS; ++j) {
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

void Interface::loadPonderation(Eigen::MatrixXd pond_board){
    sf::Font font;
    if (!font.loadFromFile("../assets/montserrat/Montserrat-Bold.otf")) {
        std::cerr << "Error loading font." << std::endl;
        return;
    }

    for (int i = 0; i < N_ROWS; ++i) {
        for (int j = 0; j < N_COLS; ++j) {
            if (pond_board(i, j) == 0) {
                continue;
            }

            double pond = pond_board(i, j);

            sf::Text text;
            text.setFont(font);
            std::ostringstream out;
            out << std::fixed << std::setprecision(2) << pond;
            text.setString(out.str());
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::Blue);
            text.setPosition(j*TILE_SIZE + TILE_SIZE*3/4-20, i*TILE_SIZE + TILE_SIZE*3/4);
            window.draw(text);

            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE*0.1, 0.9*TILE_SIZE*pond));
            tile.setPosition(j*TILE_SIZE+TILE_SIZE*0.05, i*TILE_SIZE+TILE_SIZE*0.05);
            tile.setFillColor(sf::Color::Blue);
            window.draw(tile);
        }
    }
}

void Interface::loadMovements(std::vector<Tile> movements){
    sf::Color green(0, 255, 0);

    sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));

    for (int i = 0; i < movements.size(); i++) {
        tile.setPosition(movements[i].col * TILE_SIZE, movements[i].row * TILE_SIZE);
        tile.setOutlineColor(green);
        tile.setOutlineThickness(5);
        tile.setFillColor(sf::Color::Transparent);
        window.draw(tile);
    }
}

