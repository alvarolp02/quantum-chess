#include "quantum_chess/game.hpp"
#include "interface.cpp"

Game::Game(const std::string& config_file = ""){
	N_ROWS=8;
	N_COLS=8;
	if (config_file != ""){
		Eigen::MatrixXi board_matrix = load_config(config_file);
		tree_ = QCTree(board_matrix);
		N_ROWS = board_matrix.rows();
		N_COLS = board_matrix.cols();
	} else {
		tree_ = QCTree();
	}

	Interface interface(N_ROWS, N_COLS);

	std::thread thread_0(&Interface::openWindow, &interface); 
	std::string turn = "white";

	print_interface(&interface, &tree_);

	Tile selectedPiece;

	while(true){

		get_movements(turn);
		std::vector<Tile> inputs = interface.waitForInput();

		if (inputs.size()==1 && contains(movements_, {selectedPiece, inputs[0]})){
			Tile target = inputs[0];
			tree_.propagate(selectedPiece, target);
			std::cout<< "Simple move: "<<selectedPiece.to_string()<<" "<<target.to_string()<<std::endl;
			turn = turn == "white" ? "black" : "white";
			tree_.print_tree();
			print_interface(&interface, &tree_);
			Tile selectedPiece;
			continue;
		} else if (inputs.size()==1 && contains(collapse_movements_, {selectedPiece, inputs[0]})){
			Tile target = inputs[0];
			tree_.collapse(selectedPiece);
			// If the piece is real, capture the target
			if(tree_.q_board.board_matrix(selectedPiece.row, selectedPiece.col) != 0.0){
				tree_.propagate(selectedPiece, target);
				std::cout<< "Collapse move: "<<selectedPiece.to_string()<<" "<<target.to_string()<<std::endl;
			}
			tree_.print_tree();
			turn = turn == "white" ? "black" : "white";
			print_interface(&interface, &tree_);
			Tile selectedPiece;
			continue;
		} else if (inputs.size()==2 && contains(movements_, {selectedPiece, inputs[0], inputs[1]})){
			Tile target1 = inputs[0];
			Tile target2 = inputs[1];
			tree_.split(selectedPiece, target1, target2);
			std::cout<<"Split move: "<<selectedPiece.to_string()<<" "<< 
				target1.to_string()<<" "<<target2.to_string()<<std::endl;
			tree_.print_tree();
			turn = turn == "white" ? "black" : "white";
			print_interface(&interface, &tree_);
			Tile selectedPiece;
			continue;
		} else {
			selectedPiece = inputs[0];
			std::vector<Tile> selected_moves;
			for (std::vector<Tile> move : movements_) {
				if (move[0] == selectedPiece) selected_moves.push_back(move[1]);
			}
			for (std::vector<Tile> move : collapse_movements_) {
				if (move[0] == selectedPiece) selected_moves.push_back(move[1]);
			}
			print_interface(&interface, &tree_, selected_moves);
			continue;
		}

	}
	
	thread_0.join();

}

void Game::get_movements(std::string turn){
	movements_.clear();
	collapse_movements_.clear();

	for (int i = 0; i < N_ROWS; i++){
		for (int j = 0; j < N_COLS; j++){
			Tile source = Tile(i, j);

			if (turn == "white" && tree_.q_board.isWhite(source) ||
					turn == "black" && tree_.q_board.isBlack(source)){
				std::vector<Tile> validMoves = tree_.q_board.getValidMoves(source);
				
				if (ALLOW_ENTANGLEMENT){
					for (Board* board : tree_.get_leaf_boards()){
						for (Tile move : board->getValidMoves(source)){
							if (tree_.q_board.board_matrix(move.row, move.col) == gap || 
									tree_.q_board.board_matrix(move.row, move.col) == board->board_matrix(source.row, source.col)){
								validMoves.push_back(move);
							}
						}
					}
				}
				

				for (Tile target1 : validMoves){
					// Check if the move would be a quantum capture
					if (tree_.pond_matrix(source.row, source.col) != 1.00
						&& ((turn=="white" && tree_.q_board.isBlack(target1))
						|| (turn=="black" && tree_.q_board.isWhite(target1)))){
							
						collapse_movements_.push_back({source, target1});
						continue;
					} else {
						// Get simple moves
						movements_.push_back({source, target1});
					}

					// Get split moves only to gap targets
					if (tree_.q_board.board_matrix(target1.row, target1.col) != gap){
						continue;
					}
					for (Tile target2 : validMoves){
						if (!(target1==target2) && tree_.q_board.board_matrix(target2.row, target2.col) == gap){
							movements_.push_back({source, target1, target2});
						}
					}
				}
			} 
		}
	}

	// if (ALLOW_ENTANGLEMENT){
	// 	// If entanglement is allowed, get all valid non-capture moves on any possible board
	// 	movements_.clear();
	// 	for (Board* board : tree_.get_leaf_boards()){
	// 		std::vector<Tile> validMoves = board->getValidMoves(input);
	// 		for (Tile move : validMoves){
	// 			if (tree_.q_board.board_matrix(move.row, move.col) == gap || 
	// 					tree_.q_board.board_matrix(move.row, move.col) == board->board_matrix(input.row, input.col)){
	// 				movements_.push_back(move);
	// 			}
	// 		}
	// 	}
	// 	std::vector<Tile> validCaptures = tree_.q_board.getValidMoves(input);
	// 	movements_.insert(movements_.end(), validCaptures.begin(), validCaptures.end());
	// } else {
	// 	// If entanglement is not allowed, get only moves allowed on all boards simultaneously
	// 	movements_ = tree_.q_board.getValidMoves(input);
	// }

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