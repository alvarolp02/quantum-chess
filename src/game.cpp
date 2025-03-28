#include "quantum_chess/game.hpp"
#include "interface.cpp"

int MAX_DEPTH = 5;

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
	if (is_bot(WHITE_PLAYER) && is_bot(BLACK_PLAYER)){
		interface_->BOT = true;
	}

	std::thread thread_0(&Interface::openWindow, interface_); 
	turn_ = "white";

	print_interface();

	while(state_ == Playing){

		get_movements();

		if (is_bot(current_player())){
			bot_turn();
		} else {
			human_turn();
		}
		
	}

	std::cout << "Game is over. Result: " << string(state_) << std::endl;
	
	thread_0.join();

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

	state_ = tree_.state;
}


void Game::bot_turn(){
	std::cout<<"Bot is thinking..."<<std::endl;

	get_movements();
	if (movements_.empty()){
		std::cout << "No move is found for " << turn_ << " pieces." << std::endl;
		state_ = Draw;
		
	} else {

		std::vector<Tile> best_move = movements_[0];

		switch (current_player())
		{
			case Bot_AlphaBeta: {
				AlphaBeta ab;
				ab.MAX_DEPTH = 5;
				ab.search(tree_, turn_);
				best_move = ab.best_move;
				break;
			}
			case Bot_MCTS: {
				MCTS mcts;
				mcts.EXPLORATION_CONSTANT = 1.2;
				mcts.SIMULATION_DEPTH = 30;
				mcts.MAX_SIMULATIONS = 3000;
				mcts.search(tree_, turn_);
				best_move = mcts.best_move;
				break;
			}
			default: {
				std::cout << "Unknown bot" << std::endl;
				break;
			}
		}

		if (best_move.size()==2){
			tree_.propagate(best_move[0], best_move[1]);
			std::cout << turn_ << " bot move: "<<best_move[0].to_string()<<" "<<best_move[1].to_string()<<std::endl;
			state_ = tree_.state;
		}else{
			std::cout << "No move is found for " << turn_ << " pieces." << std::endl;
			state_ = Draw;
		}
	}
	
	turn_ = turn_ == "white" ? "black" : "white";
	tree_.print_tree();
	print_interface();
}



void Game::get_movements(){
	std::pair<std::vector<std::vector<Tile>>,std::vector<std::vector<Tile>>> movements = tree_.get_movements(turn_);
	movements_ = movements.first;
	collapse_movements_ = movements.second;
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