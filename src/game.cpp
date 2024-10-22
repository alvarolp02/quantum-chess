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

  while(true){

      std::pair<int,int> input = interface.waitForInput();

      int row = input.first;
      int col = input.second;

      // if (turn == "white" && B.isWhite(row, col) == false) {
      //   std::cout << "Not a white piece" << std::endl;
      //   continue;
      // } else if (turn == "black" && B.isBlack(row, col) == false) {
      //   std::cout << "Not a black piece" << std::endl;
      //   continue;
      // }

      std::vector<std::pair<int,int>> movements = B.getValidMoves(row, col);
      interface.loadBoard();
      interface.loadPieces(B.board);
      interface.loadMovements(movements);
      interface.window.display();
      // for (int i = 0; i < moves.size(); i++) {
      //   std::cout << moves[i].first << " " << moves[i].second << std::endl;
      // }

      // if (moves.size() == 0) {
      //   std::cout << "No valid moves" << std::endl;
      //   continue;
      // }
      // B.movePiece(input1, input2, moves[0].first, moves[0].second);

      // B.printBoard();

      turn = turn == "white" ? "black" : "white"; 
  }
  
  thread_0.join();

  return 0;
}