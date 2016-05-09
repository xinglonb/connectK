#ifndef AISHELL_H
#define AISHELL_H

#pragma once
#include "Move.h"
#include <iostream>
#include <vector>
#include <map>
#include <time.h>

// A new AIShell will be created for every move request.
class AIShell{

public:
	//these represent the values for each piece type. 
	static const int AI_PIECE=1;
	static const int HUMAN_PIECE = -1;
	static const int NO_PIECE=0;


private:
	//Do not alter the values of numRows or numcols. 
	//they are used for deallocating the gameState variable.
	int numRows; //the total number of rows in the game state.
	int numCols; //the total number of columns in the game state.
	int **gameState; //a pointer to a two-dimensional array representing the game state.
	bool gravityOn; //this will be true if gravity is turned on. It will be false if gravity is turned off.
	Move lastMove; //this is the move made last by your oppenent. If your opponent has not made a move yet (you move first) then this move will hold the value (-1, -1) instead. 
	

public: 
	int deadline; //this is how many milliseconds the AI has to make move. 
	int k;        // k is the number of pieces a player must get in a row/column/diagonal to win the game. IE in connect 4, this variable would be 4 

	AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove);
	~AIShell();
	Move makeMove();

	time_t start;
	time_t ending;
	int utility(int** state);
	int heuristic(int** state, int col, int row);
	//Move Minimax_Decision(int** state);
	Move Iterative_Deeping_Search(int** state);
	Move Depth_limited_Alpha_Beta_Search(int** state, int depth);
	int Max_Value_Depth(int** state, int a, int b, int depth);
	int Min_Value_Depth(int** state, int a, int b, int depth);
	Move last_best;

	Move Alpha_Beta_Search(int** state);
	int Max_Value(int** state, int a, int b);
	int Min_Value(int** state, int a, int b);
	int** Result(int** state, Move a, int player);
	bool Terminal_Test(int** state);
	std::vector<Move> Actions(int** state);
};

#endif //AISHELL_H
