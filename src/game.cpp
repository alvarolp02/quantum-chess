#include "board.cpp"
#include "interface.cpp"
#include <thread>


int main(int argc, char * argv[])
{
  Board B = Board();

  Interface interface = Interface();
  std::thread thread_0(&Interface::openWindow, &interface); 
  std::string turn = "white";

 
  interface.loadBoard();
  interface.loadPieces(B.board_matrix);
  interface.window.display();

  std::vector<Tile> movements;
  Tile selectedPiece;

  while(true){

      Tile input = interface.waitForInput();

      for (int i = 0; i < movements.size(); i++) {
        if (movements[i]==input) {
          B.movePiece(selectedPiece, input);
          interface.loadBoard();
          interface.loadPieces(B.board_matrix);
          interface.window.display();
          turn = turn == "white" ? "black" : "white"; 
          movements.clear();
          Tile selectedPiece;
          break;
        }
      }

      if (turn == "white" && B.isWhite(input) == false) {
        continue;
      } else if (turn == "black" && B.isBlack(input) == false) {
        continue;
      }

      movements = B.getValidMoves(input);
      interface.loadBoard();
      interface.loadPieces(B.board_matrix);
      interface.loadMovements(movements);
      interface.window.display();

      selectedPiece = input;
  }
  
  thread_0.join();

  return 0;
}