#include "interface.cpp"
#include "qc_tree.cpp"
#include "board.cpp"
#include "qc_node.cpp"
#include <thread>
#include <random>

void print_interface(Interface* interface, QCTree* tree){
  interface->loadBoard();
  interface->loadPieces(tree->q_board.board_matrix);
  interface->loadPonderation(tree->pond_matrix);
  interface->window.display();
}

void print_interface(Interface* interface, QCTree* tree, std::vector<Tile> movements){
  interface->loadBoard();
  interface->loadPieces(tree->q_board.board_matrix);
  interface->loadPonderation(tree->pond_matrix);
  interface->loadMovements(movements);
  interface->window.display();
}


int main(int argc, char * argv[])
{
  QCTree tree = QCTree();

  Interface interface = Interface();
  std::thread thread_0(&Interface::openWindow, &interface); 
  std::string turn = "white";

  tree.get_ponderated_board();
  print_interface(&interface, &tree);

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

          bool source_is_quantum = tree.pond_matrix(selectedPiece.row, selectedPiece.col) != 1.00;
          bool target_occupied = tree.q_board.board_matrix(target.row, target.col) != 0;
          
          // If a quantum piece is used to capture an occupied tile, collapse the piece
          if(source_is_quantum && target_occupied){
            tree.collapse(selectedPiece);
            tree.get_ponderated_board();

            // If the piece is real, capture the target
            if(tree.q_board.board_matrix(selectedPiece.row, selectedPiece.col) != 0){
              tree.propagate(selectedPiece, target);
              std::cout<< "Move: "<<selectedPiece.to_string()<<" "<<target.to_string()<<std::endl;
            }

          // In other cases, just move the piece
          } else {
            tree.propagate(selectedPiece, target);
            std::cout<< "Move: "<<selectedPiece.to_string()<<" "<<target.to_string()<<std::endl;
          }

          tree.print_tree();
          tree.get_ponderated_board();

          print_interface(&interface, &tree);
          turn = turn == "white" ? "black" : "white"; 
          movements.clear();
          Tile selectedPiece;
          continue;

        } else { // The input is not a move

          //Check if the input is a selecting a valid piece
          if (turn == "white" && tree.q_board.isWhite(input) == false) {
            continue;
          } else if (turn == "black" && tree.q_board.isBlack(input) == false) {
            continue;
          }

          tree.get_ponderated_board();
          movements = tree.q_board.getValidMoves(input);
          print_interface(&interface, &tree, movements);

          selectedPiece = input;
        }
        
      } else if (inputs.size() == 2){ // If right click, the input is a quantum move
        Tile target1 = inputs[0];
        Tile target2 = inputs[1];

        bool target1_ok = std::find(movements.begin(), movements.end(), target1) != movements.end();
        bool target2_ok = std::find(movements.begin(), movements.end(), target2) != movements.end();

        
        if (target1_ok && target2_ok && !(target1==target2)) {

          tree.split(selectedPiece, target1, target2);
          std::cout<<"Split: "<<selectedPiece.to_string()<<" "<< 
                target1.to_string()<<" "<<target2.to_string()<<std::endl;

          tree.print_tree();
          tree.get_ponderated_board();
          print_interface(&interface, &tree);
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