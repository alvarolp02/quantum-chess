#include "interface.cpp"
#include "qChessTree.cpp"
#include <thread>


int main(int argc, char * argv[])
{
  Board B = Board();
  qChessTree tree = qChessTree(B);

  Interface interface = Interface();
  std::thread thread_0(&Interface::openWindow, &interface); 
  std::string turn = "white";

 
  interface.loadBoard();
  interface.loadPieces(B.board_matrix);
  interface.window.display();

  std::vector<Tile> movements;
  Tile selectedPiece;

  while(true){

      std::vector<Tile> inputs = interface.waitForInput();

      //If left click, the input is either a simple move or selecting a piece
      if (inputs.size() == 1){
         Tile input = inputs[0];

        //Check if the input is a valid move
        bool is_target = std::find(movements.begin(), movements.end(), input) != movements.end();
        if (is_target) {
          Tile target = input;
          B.movePiece(selectedPiece, target);

          tree.propagate(selectedPiece, target);
          tree.print_tree();
          interface.loadBoard();
          interface.loadPieces(B.board_matrix);
          interface.window.display();
          turn = turn == "white" ? "black" : "white"; 
          movements.clear();
          Tile selectedPiece;
          continue;

        } else { // The input is not a move

          //Check if the input is a selecting a valid piece
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
        
      } else if (inputs.size() == 2){ // If right click, the input is a quantum move
        Tile target1 = inputs[0];
        Tile target2 = inputs[1];

        bool target1_ok = std::find(movements.begin(), movements.end(), target1) != movements.end();
        bool target2_ok = std::find(movements.begin(), movements.end(), target2) != movements.end();

        
        if (target1_ok && target2_ok && !(target1==target2)) {
          B.movePiece(selectedPiece, target1);

          tree.split(selectedPiece, target1, target2);
          tree.print_tree();
          interface.loadBoard();
          interface.loadPieces(B.board_matrix);
          interface.window.display();
          turn = turn == "white" ? "black" : "white"; 
          movements.clear();
          Tile selectedPiece;
          continue;
        }
        
      }

  }
  
  thread_0.join();

  return 0;
}