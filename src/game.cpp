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
  interface.loadPieces(B.board);
  interface.window.display();

  std::vector<std::pair<int,int>> movements;
  std::pair<int,int> selectedPiece;

  while(true){

      std::pair<int,int> input = interface.waitForInput();

      int row = input.first;
      int col = input.second;

      for (int i = 0; i < movements.size(); i++) {
        if (movements[i].first == row && movements[i].second == col) {
          B.movePiece(selectedPiece.first, selectedPiece.second, row, col);
          interface.loadBoard();
          interface.loadPieces(B.board);
          interface.window.display();
          turn = turn == "white" ? "black" : "white"; 
          movements.clear();
          std::pair<int,int> selectedPiece;
          break;
        }
      }

      if (turn == "white" && B.isWhite(row, col) == false) {
        continue;
      } else if (turn == "black" && B.isBlack(row, col) == false) {
        continue;
      }

      movements = B.getValidMoves(row, col);
      interface.loadBoard();
      interface.loadPieces(B.board);
      interface.loadMovements(movements);
      interface.window.display();

      selectedPiece = input;
  }
  
  thread_0.join();

  return 0;
}