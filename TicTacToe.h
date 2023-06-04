#pragma once
#include <string>

class Board {
private:
	std::string board[3][3] = {
		{" "," "," "},
		{" "," "," "},
		{" "," "," "}
	};
public:
	void print_grid(); //Prints board
	void add_point(int player,int point[]); //Adds a move to the board
	void player_turn(int player,std::string name); //Does one player turn
	void bot_turn(); //Does one bot move
	bool check_move(int point[]); //Checks a move is valid
	void choose_game(); //Chooses whether to play the bot or a human
	void play_game_bot(); //runs game against a bot
	void play_game_player(); //runs game against another player
	bool evaluate_winner(); //checks if a winner is on the board
};