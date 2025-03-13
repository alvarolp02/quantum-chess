#include "quantum_chess/game.hpp"
#include "interface.cpp"

Game::Game(const std::string& config_file = ""){
	QCTree tree;
	int rows=8;
	int cols=8;
	if (config_file != ""){
		Eigen::MatrixXi board_matrix = load_config(config_file);
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

}

int main(int argc, char * argv[])
{
	if (argc == 2){
		Game(std::string("../") + argv[1]);
	} else {
		Game();
	}
  	return 0;
}