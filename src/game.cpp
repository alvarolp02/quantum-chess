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

	interface_ = new Interface(N_ROWS, N_COLS);

	std::thread thread_0(&Interface::openWindow, interface_); 
	turn_ = "white";

	print_interface();

	while(true){

		get_movements();

		if ((turn_=="white" && WHITE_PLAYER=="human") || (turn_=="black" && BLACK_PLAYER=="human")){
			human_turn();
		} else if ((turn_=="white" && WHITE_PLAYER=="bot") || (turn_=="black" && BLACK_PLAYER=="bot")){
			bot_turn();
		}
		
	}
	
	thread_0.detach();

}


void Game::human_turn(){
	std::vector<Tile> inputs = interface_->waitForInput();

	if (selected_piece_.row!=-1 && inputs.size()==1 && contains(movements_, {selected_piece_, inputs[0]})){
		Tile target = inputs[0];
		tree_.propagate(selected_piece_, target);
		std::cout<< "Simple move: "<<selected_piece_.to_string()<<" "<<target.to_string()<<std::endl;
		turn_ = turn_ == "white" ? "black" : "white";
		tree_.print_tree();
		print_interface();
		selected_piece_ = Tile(-1, -1);
	} else if (selected_piece_.row!=-1 && inputs.size()==1 && contains(collapse_movements_, {selected_piece_, inputs[0]})){
		Tile target = inputs[0];
		tree_.collapse(selected_piece_);
		// If the piece is real, capture the target
		if(tree_.q_board.board_matrix(selected_piece_.row, selected_piece_.col) != 0.0){
			tree_.propagate(selected_piece_, target);
			std::cout<< "Collapse move: "<<selected_piece_.to_string()<<" "<<target.to_string()<<std::endl;
		}
		tree_.print_tree();
		turn_ = turn_ == "white" ? "black" : "white";
		print_interface();
		selected_piece_ = Tile(-1, -1);
	} else if (selected_piece_.row!=-1 && inputs.size()==2 && contains(movements_, {selected_piece_, inputs[0], inputs[1]})){
		Tile target1 = inputs[0];
		Tile target2 = inputs[1];
		tree_.split(selected_piece_, target1, target2);
		std::cout<<"Split move: "<<selected_piece_.to_string()<<" "<< 
			target1.to_string()<<" "<<target2.to_string()<<std::endl;
		tree_.print_tree();
		turn_ = turn_ == "white" ? "black" : "white";
		print_interface();
		selected_piece_ = Tile(-1, -1);
	} else if (inputs.size()==1){
		selected_piece_ = inputs[0];
		std::vector<Tile> selected_moves;
		for (std::vector<Tile> move : movements_) {
			if (move[0] == selected_piece_) selected_moves.push_back(move[1]);
		}
		for (std::vector<Tile> move : collapse_movements_) {
			if (move[0] == selected_piece_) selected_moves.push_back(move[1]);
		}
		print_interface(selected_moves);
	}
}


void Game::bot_turn(){

	// auto best_move = explore_tree(tree_, 2, turn_);
	std::cout<<"Bot is thinking..."<<std::endl;
	auto best_move = alpha_beta(tree_, 4, -1000, 1000, turn_, {});
	tree_.propagate(best_move[0], best_move[1]);
	std::cout<< "Bot move: "<<best_move[0].to_string()<<" "<<best_move[1].to_string()<<std::endl;

	turn_ = turn_ == "white" ? "black" : "white";
	tree_.print_tree();
	print_interface();
}

std::vector<Tile> Game::explore_tree(QCTree tree, int depth, std::string turn){
	std::vector<Tile> best_move;
	double best_score = 1000;
	for (auto move : get_movements(tree, turn).first){
		if (move.size() == 2){
			QCTree new_tree = tree;
			new_tree.propagate(move[0], move[1]);
			double score = new_tree.score;
			if (depth > 0){
				std::vector<Tile> next_move = explore_tree(new_tree, depth-1, turn == "white" ? "black" : "white");
				new_tree.propagate(next_move[0], next_move[1]);
				score = new_tree.score;
			}
			if (score < best_score){
				best_score = score;
				best_move = move;
			}
		}
	}
	return best_move;
}

std::vector<Tile> Game::alpha_beta(QCTree tree, int depth, double alpha, double beta, 
									std::string turn, std::vector<Tile> prev_move) {
	std::vector<Tile> best_move;

	if (depth == 0) {
		return prev_move;
	}

	if (turn == "white") { // Maximizing player
		double maxEval = -1000;
		for (auto move : get_movements(tree, turn).first) {
			if (move.size() == 2) {
				QCTree new_tree = tree;
				new_tree.propagate(move[0], move[1]);
				std::vector<Tile> next_move = alpha_beta(new_tree, depth - 1, alpha, beta, "black", move);
				new_tree.propagate(next_move[0], next_move[1]);
				double eval = new_tree.score;
				if (eval > maxEval) {
					maxEval = eval;
					best_move = move;
				}
				alpha = std::max(alpha, eval);
				if (beta <= alpha) {
					break;
				}
			}
		}
	} else { // Minimizing player
		double minEval = 1000;
		for (auto move : get_movements(tree, turn).first) {
			if (move.size() == 2) {
				QCTree new_tree = tree;
				new_tree.propagate(move[0], move[1]);
				std::vector<Tile> next_move = alpha_beta(new_tree, depth - 1, alpha, beta, "white", move);
				new_tree.propagate(next_move[0], next_move[1]);
				double eval = new_tree.score;
				if (eval < minEval) {
					minEval = eval;
					best_move = move;
				}
				beta = std::min(beta, eval);
				if (beta <= alpha) {
					break;
				}
			}
		}
	}
	return best_move;
}


void Game::get_movements(){
	std::pair<std::vector<std::vector<Tile>>,std::vector<std::vector<Tile>>> movements = get_movements(tree_, turn_);
	movements_ = movements.first;
	collapse_movements_ = movements.second;
}


std::pair<std::vector<std::vector<Tile>>,std::vector<std::vector<Tile>>> 
			Game::get_movements(QCTree tree, std::string turn){
	std::vector<std::vector<Tile>> movements;
	std::vector<std::vector<Tile>> collapse_movements;

	for (int i = 0; i < N_ROWS; i++){
		for (int j = 0; j < N_COLS; j++){
			Tile source = Tile(i, j);

			if (turn == "white" && tree.q_board.isWhite(source) ||
					turn == "black" && tree.q_board.isBlack(source)){
				std::vector<Tile> validMoves = tree.q_board.getValidMoves(source);
				
				if (ALLOW_ENTANGLEMENT){
					for (Board* board : tree.get_leaf_boards()){
						for (Tile move : board->getValidMoves(source)){
							if (tree.q_board.board_matrix(move.row, move.col) == gap || 
									tree.q_board.board_matrix(move.row, move.col) == board->board_matrix(source.row, source.col)){
								validMoves.push_back(move);
							}
						}
					}
				}
				

				for (Tile target1 : validMoves){
					// Check if the move would be a quantum capture
					if (tree.pond_matrix(source.row, source.col) != 1.00
						&& ((turn=="white" && tree.q_board.isBlack(target1))
						|| (turn=="black" && tree.q_board.isWhite(target1)))){
							
						collapse_movements.push_back({source, target1});
						continue;
					} else {
						// Get simple moves
						movements.push_back({source, target1});
					}

					// Get split moves only to gap targets
					if (tree.q_board.board_matrix(target1.row, target1.col) != gap){
						continue;
					}
					for (Tile target2 : validMoves){
						if (!(target1==target2) && tree.q_board.board_matrix(target2.row, target2.col) == gap){
							movements.push_back({source, target1, target2});
						}
					}
				}
			} 
		}
	}

	return {movements, collapse_movements};
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