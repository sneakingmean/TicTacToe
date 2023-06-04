#include <iostream>
#include <string>
#include <random>
#include "TicTacToe.h"

//Prints out the current board
void Board::print_grid() {
	std::cout << std::endl;
	std::cout << "            Column       \n";
	std::cout << "        1     2     3   \n";
	std::cout << "     |-----------------|\n";
	std::cout << "   1 |  " << board[0][0] << "  |  " << board[0][1] << "  |  " << board[0][2] << "  |\n";
	std::cout << "R    |-----------------|\n";
	std::cout << "O  2 |  " << board[1][0] << "  |  " << board[1][1] << "  |  " << board[1][2] << "  |\n";
	std::cout << "W    |-----------------|\n";
	std::cout << "   3 |  " << board[2][0] << "  |  " << board[2][1] << "  |  " << board[2][2] << "  |\n";
	std::cout << "     |-----------------|\n\n";
}

//Adds a point to the board
void Board::add_point(int player,int point[]) {
	int row = point[0]-1;
	int col = point[1]-1;
 	if (player) {
		board[row][col] = "O";
	}
	else {
		board[row][col] = "X";
	}
}

//Asks a player to move and adds move to the board, then prints updated board
void Board::player_turn(int player,std::string name) {
	int point[2] = { 0,0 };
	std::string input1;
	std::string input2;
    std::string symbol;

	if (player)	symbol = "O";
	else symbol = "X";
	std::cout << name << "'s Turn (" << symbol << ")\n\n";

	while (true) {
		try {
			std::cout << "Choose a point\nRow: ";
			std::cin >> input1;
			std::cout << "Column: ";
			std::cin >> input2;
			point[0] = std::stoi(input1);
			point[1] = std::stoi(input2);
			if (check_move(point)) break;
			else std::cout << "\nMOVE NOT VALID\n";
		}
		catch (...) {
			std::cout << "\nMOVE NOT VALID\n";
		}
	}
	
	add_point(player, point);
	print_grid();
}

void Board::bot_turn() {
	bool move = true;
	std::cout << "Bot's Turn (O)\n\n";

	//Determine if there is a winning move
	int i = 0;
	while (move && i < 3) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == " ") {
				board[i][j] = "O";
				if (evaluate_winner()) {
					move = false;
					break;
				}
				board[i][j] = " ";
			}
		}
		i++;
	}
	//Determine if there is a forced move
	i = 0;
	while (move && i < 3) {
		for (int j = 0; j < 3; j++) {
			if (board[i][j] == " ") {
				board[i][j] = "X";
				if (evaluate_winner()) {
					move = false;
					board[i][j] = "O";
					break;
				}
				board[i][j] = " ";
			}
		}
		i++;
	}
	//Special case where bot prioritizes side over corner
	if (move) {
		int count = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (board[i][j] == "X") count++;
			}
		}
		if (count == 2 && board[0][0] == "X" && board[2][2] == "X" && board[1][1] == "O") {
			move = false;
			board[0][1] = "O";
		}
		else if (count == 2 && board[0][2] == "X" && board[2][0] == "X" && board[1][1] == "O") {
			move = false;
			board[0][1] = "O";
		}
	}

	/*If no forced move, try to move in the center. If not available,
	go in the next available corner going from top left, across the row
	and then down*/
	if (move) {
		if (board[1][1] == " ") {
			board[1][1] = "O";
			move = false;
		}
		else {
			i = 0;
			while (move && i < 3) {
				for (int j = 0; j < 3; j += 2) {
					if (board[i][j] == " ") {
						move = false;
						board[i][j] = "O";
						break;
					}
				}
				i += 2;
			}
		}
	}
	//Go into the next available spot
	if (move) {
		i = 0;
		while (move && i<3) {
			for (int j = 0; j < 3; j++) {
				if (board[i][j] == " ") {
					move = false;
					board[i][j] = "O";
					break;
				}
			}
			i++;
		}
	}
	print_grid();
}

//Checks if a move is valid
bool Board::check_move(int point[]) {
	bool good_move = false;
	int x = point[0];
	int y = point[1];
	if (0 < x  && x < 4 && 0 < y && y < 4 && board[x-1][y-1] == " ") good_move = true;
	return good_move;
}

//Chooses to play against the computer or another player
void Board::choose_game() {
	int choice=5;
	std::cout << "Play against the computer or another person?\n";
	std::cout << "For Computer Type 0\n";
	std::cout << "For Another Person Type 1\n";

	while (true) {
		try {
			std::string input;
			std::cin >> input;
			choice = std::stoi(input);
			if (choice==1 || choice==0) break;
			else std::cout << "Invalid Choice\nChoose Again (0 Bot/ 1 Human): ";
		}
		catch (...) {
			std::cout << "Invalid Choice\nChoose Again (0 Bot/ 1 Human): ";
		}
	}

	if (choice) play_game_player();
	else play_game_bot();
}

//Function for the player to play the game against another player
void Board::play_game_player() {
	std::string names[2];
	std::cout << "Player 1 Name: ";
	std::cin >> names[0];
	std::cout << "Player 2 Name: ";
	std::cin >> names[1];
	int turn = 0;
	print_grid();
	while (!evaluate_winner() && turn < 9) {
		player_turn(turn % 2,names[turn%2]);
		turn++;
	}
	std::cout << "Game Over\n\n";
	if (turn == 9) {
		std::cout << "It's A Tie!\n\n";
	}else std::cout << names[(turn - 1) % 2] << " Wins!\n\n";
}

//Function for a player to play the game against the computer
void Board::play_game_bot() {
	std::string name;
	std::cout << "Enter Player Name: ";
	std::cin >> name;
	std::random_device rd;

	//Randomly decides who starts
	int turn = 0;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, 1);
	auto start = uni(rng);
	if (start) {
		turn = 1;
	} // Bot goes first if turn=1

	const int end = turn + 9;
	print_grid();
	while (!evaluate_winner() && turn < end) {
		if (turn % 2) bot_turn(); //Bot Goes
		else player_turn(0, name); // Player Goes
		turn++;
	}
	std::cout << "Game Over\n\n";
	if (turn == end) {
		std::cout << "It's A Tie!\n\n";
	}
	else {
		if (turn % 2) std::cout << name << " Wins!\n\n";
		else std::cout << "Bot Wins!\n\n";
	}
}

//Checks if there is a winner on the board
bool Board::evaluate_winner() {
	bool winner = false;
	for (int i = 0; i < 3; i++) {
		const std::string row_check=board[i][0];
		const std::string col_check = board[0][i];
		if (row_check != " " && row_check == board[i][1] && row_check == board[i][2]) {
			winner = true;
			break;
		}
		if (col_check != " " && col_check == board[1][i] && col_check == board[2][i]) {
			winner = true;
			break;
		}
	}
	const std::string diag_check_1 = board[0][0];
	const std::string diag_check_2 = board[2][0];
	if (diag_check_1 != " " && diag_check_1 == board[1][1] && diag_check_1 == board[2][2]) winner = true;
	if (diag_check_2 != " " && diag_check_2 == board[1][1] && diag_check_2 == board[0][2]) winner = true;
	return winner;
}