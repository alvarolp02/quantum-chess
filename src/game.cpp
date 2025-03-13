#include "interface.cpp"
#include "quantum_chess/qc_tree.hpp"
#include "quantum_chess/board.hpp"
#include "quantum_chess/qc_node.hpp"
#include "quantum_chess/utils.hpp"
#include <thread>
#include <random>
#include <filesystem>
#include <yaml-cpp/yaml.h>

bool ALLOW_ENTANGLEMENT = true;

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

Eigen::MatrixXi load_config(const std::string& filename){
  YAML::Node config = YAML::LoadFile(filename);
  std::map<char, int> pieceToInt = {
    {'.', 0},  {'P', 1},  {'R', 2},  {'N', 3},  {'B', 4}, {'Q', 5},  {'K', 6},
    {'p', 7}, {'r', 8}, {'n', 9}, {'b', 10}, {'q', 11}, {'k', 12} };


  auto values = config["board"];
  
  int n = values.size();
  int m = values[0].size();

  Eigen::MatrixXi matrix = Eigen::MatrixXi::Zero(n, m);


  for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        matrix(i, j) = pieceToInt[values[i][j].as<char>()];
      }
  }

  return matrix;

}


int main(int argc, char * argv[])
{
  QCTree tree;
  int rows=8;
  int cols=8;
  if (argc == 2){
    Eigen::MatrixXi board_matrix = load_config(std::string("../") + argv[1]);
    tree = QCTree(board_matrix);
    rows = board_matrix.rows();
    cols = board_matrix.cols();
  } else {
    tree = QCTree();
  }

  Interface interface(rows, cols);

  std::thread thread_0(&Interface::openWindow, &interface); 
  std::string turn = "white";

  print_interface(&interface, &tree);

  std::vector<Tile> movements;
  Tile selectedPiece;

  while(true){

      std::vector<Tile> inputs = interface.waitForInput();

      //If left click, the input is either a simple move or selecting a piece
      if (inputs.size() == 1) {
        std::cout << tree.score << std::endl;
        Tile input = inputs[0];

        //Check if the input is a valid move
        if (contains(movements, input)) {

          Tile target = input;
          
          std::cout << tree.pond_matrix << std::endl;
          bool source_is_quantum = tree.pond_matrix(selectedPiece.row, selectedPiece.col) != 1.00;
          bool target_occupied = tree.q_board.board_matrix(target.row, target.col) != gap
                  && tree.q_board.board_matrix(target.row, target.col) != tree.q_board.board_matrix(selectedPiece.row, selectedPiece.col);
          
          // If a quantum piece is used to capture an occupied tile, collapse the piece
          if(source_is_quantum && target_occupied){
            tree.collapse(selectedPiece);

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

          print_interface(&interface, &tree);
          turn = turn == "white" ? "black" : "white"; 
          movements.clear();
          Tile selectedPiece;
          continue;

        } else { // The input is selecting a piece

          //Check if the input is a selecting a valid piece
          if (turn == "white" && tree.q_board.isWhite(input) == false) {
            continue;
          } else if (turn == "black" && tree.q_board.isBlack(input) == false) {
            continue;
          }

          if (ALLOW_ENTANGLEMENT){
            // If entanglement is allowed, get all valid non-capture moves on any possible board
            movements.clear();
            for (Board* board : tree.get_leaf_boards()){
              std::vector<Tile> validMoves = board->getValidMoves(input);
              for (Tile move : validMoves){
                if (tree.q_board.board_matrix(move.row, move.col) == gap || 
                      tree.q_board.board_matrix(move.row, move.col) == board->board_matrix(input.row, input.col)){
                  movements.push_back(move);
                }
              }
            }
            std::vector<Tile> validCaptures = tree.q_board.getValidMoves(input);
            movements.insert(movements.end(), validCaptures.begin(), validCaptures.end());
          } else {
            // If entanglement is not allowed, get only moves allowed on all boards simultaneously
            movements = tree.q_board.getValidMoves(input);
          }


          print_interface(&interface, &tree, movements);

          selectedPiece = input;
        }
        
      } else if (inputs.size() == 2){ // If right click, the input is a quantum move
        Tile target1 = inputs[0];
        Tile target2 = inputs[1];


        // Check both movements are valid and the target tiles are empty
        bool target1_ok = contains(movements, target1) 
                            && tree.q_board.board_matrix(target1.row, target1.col) == gap;
        bool target2_ok = contains(movements, target2)
                            && tree.q_board.board_matrix(target2.row, target2.col) == gap;

        
        if (target1_ok && target2_ok && !(target1==target2)) {

          tree.split(selectedPiece, target1, target2);
          std::cout<<"Split: "<<selectedPiece.to_string()<<" "<< 
                target1.to_string()<<" "<<target2.to_string()<<std::endl;

          tree.print_tree();
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