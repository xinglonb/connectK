#include "AIShell.h"
#include <iostream>
#include <limits>

AIShell::AIShell(int numCols, int numRows, bool gravityOn, int** gameState, Move lastMove)
{
	this->deadline=0;
	this->numRows=numRows;
	this->numCols=numCols;
	this->gravityOn=gravityOn;
	this->gameState=gameState;
	this->lastMove=lastMove;
}


AIShell::~AIShell()
{
	
	//delete the gameState variable.
	for (int i =0; i<numCols; i++){
		delete [] gameState[i];
	}
	delete [] gameState;

}


int Max(int a, int b){
	if (a < b)
		return b;
	else
		return a;
}

int Min(int a, int b){
	if (a > b)
		return b;
	else
		return a;
}

int minOf(std::vector<int> v){
	int min = v[0];
	for (int i = 0; i < v.size(); i++){
		if (v[i] < min)
			min = v[i];
	}
	return min;
}


int AIShell::utility(int** state){
	int v = 0;
	int chance = 0;
	int h = 0;

	std::map<std::string, int> connects;
	connects["deadk-1"] = 0;
	connects["alivek-2"] = 0;
	connects["deadk-2"]=0;
	connects["deadk-3"]=0;

	std::map<std::string, int> attacks;
	attacks["deadk-1"] = 0;
	attacks["alivek-2"] = 0;
	std::vector<int> negative;

	for (int col = 0; col<numCols; col++){
		for(int row = 0; row<numRows; row++){
			bool AI = true;
			bool Human = true;

		    for (int i = 0; i < k; i++){
		    	if (col + i == numCols){
		    		AI = false;
		    		Human = false;
		    		break;
		    	}
		    	if (state[col+i][row] != AI_PIECE)
		    		AI = false;
		    	if (state[col+i][row] != HUMAN_PIECE)
		    		Human = false;

		    }
		    if (!AI && !Human){
		    	AI = true;
		    	Human = true;
		    	for (int i = 0; i < k; i++){
		    		if (row + i == numRows){
		    			AI = false;
		    			Human = false;
		    			break;
		    		}
		    		if (state[col][row+i] != AI_PIECE)
		    			AI = false;
		    		if (state[col][row+i] != HUMAN_PIECE)
		    			Human = false;
		    	}
		    }
		    if (!AI && !Human){
		    	AI = true;
		    	Human = true;
		    	for (int i = 0; i < k; i++){
		    		if (col + i == numCols){
		    			AI = false;
		    			Human = false;
		    		    break;
		    		}
		    		if (row + i == numRows){
		    			AI = false;
		    			Human = false;
		    			break;
		    		}
		    		if (state[col+i][row+i] != AI_PIECE)
		    		    AI = false;
		    		if (state[col+i][row+i] != HUMAN_PIECE)
		    			Human = false;
		    	}
		    }
		    if (!AI && !Human){
		    	AI = true;
		    	Human = true;
		    	for (int i = 0; i < k; i++){
		    		if (col + i == numCols){
		    			AI = false;
		    			Human = false;
		    		    break;
		    		}
		    		if (row - i < 0){
		    			AI = false;
		    			Human = false;
		    		    break;
		    		}
		    		if (state[col+i][row-i] != AI_PIECE)
		    		    AI = false;
		    		if (state[col+i][row-i] != HUMAN_PIECE)
		    			Human = false;
		    	}
		    }
		    if (AI){
		    	v = 100;
		    	//v+=heuristic(state, col, row);
		    	return v;
		    }
		    if (Human){
		    	v = -100;
		    	//v+=heuristic(state, col, row);
		    	return v;
		    }

		    h = heuristic(state, col, row);
		    if (h == 60)
		    	connects["deadk-1"]+=1;
		    if (h == 50)
		    	connects["alivek-2"]+=1;
		    if (h < 0){
		    	negative.push_back(h);
		    	if (h == -60)
		    		attacks["deadk-1"]+=1;
		    	else if (h == -50)
		    		attacks["alivek-2"]+=1;
		    }
		    chance = Max(h, chance);
		}
	}

	if (connects["deadk-1"] >= 2 || (connects["deadk-1"] >= 1 && connects["alivek-2"] >= 1))
		chance = Max(90, chance);
	else if (connects["alivek-2"] >= 2)
		chance = Max(80, chance);

	if (attacks["deadk-1"] >= 2 || (attacks["deadk-1"] >= 1 && attacks["alivek-2"] >= 1))
		negative.push_back(-90);
	else if (attacks["alivek-2"] >= 2)
		negative.push_back(-80);
	if (!negative.empty()){
		chance = minOf(negative);
	}
	return chance;
}

int maxOf(std::vector<int> v){
	int max = 0;
	for (int i = 0; i < v.size(); i++){
		if (v[i] > max)
			max = v[i];
	}
	return max;
}

int AIShell::heuristic(int** state, int col, int row){
	int threat = k-2;
	int chance = 0;

	std::vector<int> chances;
	std::vector<int> risks;
	chances.push_back(chance);

	std::map<std::string, int> connects;
	connects["deadk-1"] = 0;
	connects["alivek-2"] = 0;
	connects["deadk-2"]=0;
	connects["deadk-3"]=0;

	std::map<std::string, int> attacks;
	attacks["deadk-1"] = 0;
	attacks["alivek-2"] = 0;



	bool connectk_4 = false;
	if (state[col][row] == AI_PIECE)
		connectk_4 = true;

	//Human right direction
	if (col + threat <= numCols-1){
		bool connectk_2 = true;
		bool connectk_1 = true;
		bool connectk_3 = true;
		//bool connectk_3 = true;
		for (int i = 0; i < k-1; i++){
			if (col+i <= numCols-1){
				if (state[col+i][row] != HUMAN_PIECE){
					if (i < k-3){
						connectk_1 = false;
						connectk_2 = false;
						connectk_3 = false;
						break;
					}
					else if (i < threat){
						connectk_1 = false;
						connectk_2 = false;
						break;
					}
					else if (i < k-1){
						connectk_1 = false;
						break;
					}
				}
			}
			else{
				connectk_1 = false;
			}
		}
		if (connectk_1){
			if (col != 0 && state[col-1][row]!=AI_PIECE && col+k-1 <= numCols-1 && state[col+k-1][row] != AI_PIECE){
				chance = -90;
				risks.push_back(chance);
			}
			else if ((col != 0 && state[col-1][row]!=AI_PIECE) || (col+k-1 <= numCols-1 && state[col+k-1][row] != AI_PIECE)){
				chance = -60;
				risks.push_back(chance);
				attacks["deadk-1"]+=1;
			}
		}
		else if (connectk_2){
			if (col != 0 && state[col-1][row] != AI_PIECE){
				chance = -50;
				risks.push_back(chance);
				attacks["alivek-2"]+=1;
			}
		}
		else if (connectk_3){
			if (col != 0 && state[col-1][row]!=AI_PIECE && col+k-1 <= numCols-1 && state[col+k-3][row] != AI_PIECE && state[col+k-2][row] == HUMAN_PIECE && state[col+k-1][row] != AI_PIECE){
				chance = -50;
				risks.push_back(chance);
				attacks["alivek-2"]+=1;
			}
		}
	}
	//Human right direction
	if (row + threat <= numRows-1){
		bool connectk_2 = true;
		bool connectk_1 = true;
		bool connectk_3 = true;
		for (int i = 0; i < k-1; i++){
			if (row+i <= numRows-1){
				if (state[col][row+i] != HUMAN_PIECE){
					if (i < k-3){
						connectk_1 = false;
						connectk_2 = false;
						connectk_3 = false;
						break;
					}
					else if (i < threat){
						connectk_1 = false;
						connectk_2 = false;
						break;
					}
					else if (i < k-1){
						connectk_1 = false;
						break;
					}
				}
			}
			else{
				connectk_1 = false;
			}
		}
		if (connectk_1){
			if (row != 0 && state[col][row-1] != AI_PIECE && row+k-1 <= numRows-1 && state[col][row+k-1] != AI_PIECE){
				chance = -90;
				risks.push_back(chance);
			}
			else if ((row != 0 && state[col][row-1] != AI_PIECE) || (row+k-1 <= numRows-1 && state[col][row+k-1] != AI_PIECE)){
				chance = -60;
				risks.push_back(chance);
			}
		}
		else if (connectk_2){
			if (row != 0 && state[col][row-1] != AI_PIECE){
				chance = -50;
				risks.push_back(chance);
				attacks["alivek-2"]+=1;
			}
		}
		else if (connectk_3){
			if (row != 0 && state[col][row-1]!=AI_PIECE && row+k-1 <= numRows-1 && state[col][row+k-3] != AI_PIECE && state[col][row+k-2] == HUMAN_PIECE && state[col][row+k-1] != AI_PIECE){
				chance = -50;
				risks.push_back(chance);
				attacks["alivek-2"]+=1;
			}
		}
	}
	//human right up direction
	if (col + threat <= numCols-1 && row + threat <= numRows-1){
		bool connectk_2 = true;
		bool connectk_1 = true;
		bool connectk_3 = true;
		for (int i = 0; i < k-1; i++){
			if (row+i <= numRows-1 && col+i <= numCols-1){
				if (state[col+i][row+i] != HUMAN_PIECE){
					if (i < k-3){
						connectk_1 = false;
						connectk_2 = false;
						connectk_3 = false;
						break;
					}
					else if (i < threat){
						connectk_1 = false;
						connectk_2 = false;
						break;
					}
					else if (i < k-1){
						connectk_1 = false;
						break;
					}
				}
			}
			else{
				connectk_1 = false;
			}
		}
		if (connectk_1){
			if (row != 0 && col != 0 && state[col-1][row-1] != AI_PIECE && row+k-1 <= numRows-1 && col+k-1 <= numCols-1 && state[col+k-1][row+k-1] != AI_PIECE){
				chance = -90;
				risks.push_back(chance);
			}
			else if ((row != 0 && col != 0 && state[col-1][row-1] != AI_PIECE) || (row+k-1 <= numRows-1 && col+k-1 <= numCols-1 && state[col+k-1][row+k-1] != AI_PIECE)){
				chance = -60;
				risks.push_back(chance);
				attacks["deadk-1"]+=1;
			}
		}
		else if (connectk_2){
			if (row != 0 && col != 0 && state[col-1][row-1] != AI_PIECE){
				chance = -50;
				risks.push_back(chance);
				attacks["alivek-2"]+=1;
			}
		}
		else if (connectk_3){
			if (row != 0 && col != 0 && state[col-1][row-1]!=AI_PIECE && row+k-1 <= numRows && col+k-1 <= numCols-1 && state[col+k-3][row+k-3] != AI_PIECE && state[col+k-2][row+k-2] == HUMAN_PIECE && state[col+k-1][row+k-1] != AI_PIECE){
				chance = -50;
				risks.push_back(chance);
				attacks["alivek-2"]+=1;
			}
		}
	}

	//Human right low direction
	if ( col + threat <= numCols-1 && row - threat >= 0 ){
		bool connectk_2 = true;
		bool connectk_1 = true;
		bool connectk_3 = true;
		for (int i = 0; i < k-1; i++){
			if (row - i >= 0 && col+i <= numCols-1){
				if (state[col+i][row-i] != HUMAN_PIECE){
					if (i < k-3){
						connectk_1 = false;
						connectk_2 = false;
						connectk_3 = false;
						break;
					}
					else if (i < threat){
						connectk_1 = false;
						connectk_2 = false;
						break;
					}
					else if (i < k-1){
						connectk_1 = false;
						break;
					}
				}
			}
			else{
				connectk_1 = false;
			}
		}
		if (connectk_1){
			if (row != numRows-1 && col != 0 && state[col-1][row+1] != AI_PIECE && row-k+1 >= 0 && col+k-1 <= numCols-1 && state[col+k-1][row-k+1] != AI_PIECE){
				chance = -90;
				risks.push_back(chance);
			}
			else if ((row != numRows-1 && col != 0 && state[col-1][row+1] != AI_PIECE)||(row-k+1 >= 0 && col+k-1 <= numCols-1 && state[col+k-1][row-k+1] != AI_PIECE)){
				chance = -60;
				risks.push_back(chance);
				attacks["deadk-1"]+=1;
			}
		}
		else if (connectk_2){
			if (row != numRows-1 && col != 0 && state[col-1][row+1] != AI_PIECE){
				chance = -50;
				risks.push_back(chance);
				attacks["alivek-2"]+=1;
			}
		}
		else if (connectk_3){
			if (row != numRows-1 && col != 0 && state[col-1][row+1]!=AI_PIECE && row-k+1 >= 0 && col+k-1 <= numCols-1 && state[col+k-3][row-k+3] != AI_PIECE && state[col+k-2][row-k+2] == HUMAN_PIECE && state[col+k-1][row-k+1] != AI_PIECE){
				chance = -50;
				risks.push_back(chance);
				attacks["alivek-2"]+=1;
			}
		}
	}

	//evaluate AI_PIECE chance
	//right direction
	if (col + threat <= numCols-1 && state[col+threat][row] != HUMAN_PIECE){
		bool connectk_2 = true;
		bool connectk_1 = true;
		bool connectk_3 = true;
		for (int i = 0; i < k-1; i++){
			if (col+i <= numCols-1){
				if (state[col+i][row] != AI_PIECE){
					if (i < k-3){
						connectk_1 = false;
						connectk_2 = false;
						connectk_3 = false;
						break;
					}
					else if (i < threat){
						connectk_1 = false;
						connectk_2 = false;
						break;
					}
					else if (i < k-1){
						connectk_1 = false;
						break;
					}
				}
			}
			else{
				connectk_1 = false;
			}
		}
		if (connectk_1){
			if (col != 0 && state[col-1][row]!=HUMAN_PIECE && col+k-1 <= numCols-1 && state[col+k-1][row] != HUMAN_PIECE)
				chance = 90;
			else if ((col != 0 && state[col-1][row]!=HUMAN_PIECE) || (col+k-1 <= numCols-1 && state[col+k-1][row] != HUMAN_PIECE)){
				chance = 60;
				connects["deadk-1"]+=1;
			}
			chances.push_back(chance);
		}
		else if (connectk_2){
			if (col != 0 && state[col-1][row] != HUMAN_PIECE){
				chance = 50;
				connects["alivek-2"]+=1;
				chances.push_back(chance);
			}
			else if ((col != 0 && state[col-1][row] != HUMAN_PIECE) || (col + threat + 1 <= numCols-1 && state[col+threat][row] != HUMAN_PIECE && state[col+threat+1][row] != HUMAN_PIECE))
				connects["deadk-2"]+=1;
		}
		else if (connectk_3){
			if (col != 0 && state[col-1][row]!=HUMAN_PIECE && col+k-3 <= numCols-1 && state[col+k-3][row] != HUMAN_PIECE){
				chance = 30;
				chances.push_back(chance);
			}
		}
		else if (connectk_4 && k > 4){
			if (col != 0 && state[col-1][row]!=HUMAN_PIECE && col+1 <= numCols-1 && state[col+1][row] != HUMAN_PIECE)
				chance = 20;
			else if ((col != 0 && state[col-1][row]!=HUMAN_PIECE) || (col+1 <= numCols-1 && state[col+1][row] != HUMAN_PIECE))
				chance = 10;
			std::cout <<col<<row <<"chance1=" <<chance<<std::endl;
			chances.push_back(chance);
		}
	}

	//up direction
	if (row + threat <= numRows-1 && state[col][row+threat] != HUMAN_PIECE){
		bool connectk_2 = true;
		bool connectk_1 = true;
		bool connectk_3 = true;
		for (int i = 0; i < k-1; i++){
			if (row+i <= numRows-1){
				if (state[col][row+i] != AI_PIECE){
					if (i < k-3){
						connectk_1 = false;
						connectk_2 = false;
						connectk_3 = false;
						break;
					}
					else if (i < threat){
						connectk_1 = false;
						connectk_2 = false;
						break;
					}
					else if (i < k-1){
						connectk_1 = false;
						break;
					}
				}
			}
			else{
				connectk_1 = false;
			}
		}
		if (connectk_1){
			if (row != 0 && state[col][row-1] != HUMAN_PIECE && row+k-1 <= numRows-1 && state[col][row+k-1] != HUMAN_PIECE)
				chance = 90;
			else if ((row != 0 && state[col][row-1] != HUMAN_PIECE) || (row+k-1 <= numRows-1 && state[col][row+k-1] != HUMAN_PIECE)){
				chance = 60;
				connects["deadk-1"]+=1;
			}
			chances.push_back(chance);
		}
		else if (connectk_2){
			if (row != 0 && state[col][row-1] != HUMAN_PIECE){
				chance = 50;
				connects["alivek-2"]+=1;
				chances.push_back(chance);
			}
			else if ((row != 0 && state[col][row-1] != HUMAN_PIECE)||(row + threat + 1 <= numRows-1 && state[col][row+threat] != HUMAN_PIECE && state[col][row+threat+1] != HUMAN_PIECE))
				connects["deadk-2"]+=1;
		}
		else if (connectk_3){
			if (row != 0 && state[col][row-1]!=HUMAN_PIECE && row+k-3 <= numRows-1 && state[col][row+k-3] != HUMAN_PIECE){
				chance = 30;
				chances.push_back(chance);
			}
		}
		else if (connectk_4 && k>4){
			if (row != 0 && state[col][row-1] != HUMAN_PIECE && row+1 <= numRows-1 && state[col][row+1] != HUMAN_PIECE)
				chance = 20;
			else if ((row != 0 && state[col][row-1] != HUMAN_PIECE) || (row+1 <= numRows-1 && state[col][row+1] != HUMAN_PIECE))
				chance = 10;
			std::cout <<col<<row <<"chance2=" <<chance<<std::endl;
			chances.push_back(chance);
		}
	}


	//right up direction
	if (col + threat <= numCols-1 && row + threat <= numRows-1 && state[col+threat][row+threat] != HUMAN_PIECE ){
		bool connectk_2 = true;
		bool connectk_1 = true;
		bool connectk_3 = true;
		for (int i = 0; i < k-1; i++){
			if (row+i <= numRows-1 && col+i <= numCols-1){
				if (state[col+i][row+i] != AI_PIECE){
					if (i < k-3){
						connectk_1 = false;
						connectk_2 = false;
						connectk_3 = false;
						break;
					}
					else if (i < threat){
						connectk_1 = false;
						connectk_2 = false;
						break;
					}
					else if (i < k-1){
						connectk_1 = false;
						break;
					}
				}
			}
			else{
				connectk_1 = false;
			}
		}
		if (connectk_1){
			if (row != 0 && col != 0 && state[col-1][row-1] != HUMAN_PIECE && row+k-1 <= numRows-1 && col+k-1 <= numCols-1 && state[col+k-1][row+k-1] != HUMAN_PIECE)
				chance = 90;
			else if ((row != 0 && col != 0 && state[col-1][row-1] != HUMAN_PIECE) || (row+k-1 <= numRows-1 && col+k-1 <= numCols-1 && state[col+k-1][row+k-1] != HUMAN_PIECE)){
				chance = 60;
				connects["deadk-1"]+=1;
			}
			chances.push_back(chance);
		}
		else if (connectk_2){
			if (row != 0 && col != 0 && state[col-1][row-1] != HUMAN_PIECE){
				chance = 50;
				connects["alivek-2"]+=1;
				chances.push_back(chance);
			}
			else if ((row != 0 && col != 0 && state[col-1][row-1] != HUMAN_PIECE)||(col + threat + 1 <= numCols-1 && row + threat + 1 <= numRows-1 && state[col+threat][row+threat] != HUMAN_PIECE && state[col+threat+1][row+threat+1] != HUMAN_PIECE))
				connects["deadk-2"]+=1;
		}
		else if (connectk_3){
			if (row != 0 && col != 0 && state[col-1][row-1] != HUMAN_PIECE && row+k-3 <= numRows-1 && col+k-3 <= numCols-1 && state[col+k-3][row+k-3] != HUMAN_PIECE){
				chance = 30;
				chances.push_back(chance);
			}
		}
		else if (connectk_4 && k<4){
			if (row != 0 && col != 0 && state[col-1][row-1] != HUMAN_PIECE && row+1 <= numRows-1 && col+1 <= numCols-1 && state[col+1][row+1] != HUMAN_PIECE)
				chance = 20;
			else if ((row != 0 && col != 0 && state[col-1][row-1] != HUMAN_PIECE) || (row+1 <= numRows-1 && col+1 <= numCols-1 && state[col+1][row+1] != HUMAN_PIECE))
				chance = 10;
			std::cout <<col<<row <<"chance3=" <<chance<<std::endl;
			chances.push_back(chance);
		}
	}

	//right low corner
	if ( col + threat <= numCols-1 && row - threat >= 0 && state[col+threat][row-threat] != HUMAN_PIECE ){
		bool connectk_2 = true;
		bool connectk_1 = true;
		bool connectk_3 = true;
		for (int i = 0; i < k-1; i++){
			if (row - i >= 0 && col+i <= numCols-1){
				if (state[col+i][row-i] != AI_PIECE){
					if (i < k-3){
						connectk_1 = false;
						connectk_2 = false;
						connectk_3 = false;
						break;
					}
					else if (i < threat){
						connectk_1 = false;
						connectk_2 = false;
						break;
					}
					else if (i < k-1){
						connectk_1 = false;
						break;
					}
				}
			}
			else{
				connectk_1 = false;
			}
		}
		if (connectk_1){
			if (row != numRows-1 && col != 0 && state[col-1][row+1] != HUMAN_PIECE && row-k+1 >= 0 && col+k-1 <= numCols-1 && state[col+k-1][row-k+1] != HUMAN_PIECE)
				chance = 90;
			else if ((row != numRows-1 && col != 0 && state[col-1][row+1] != HUMAN_PIECE)||(row-k+1 >= 0 && col+k-1 <= numCols-1 && state[col+k-1][row-k+1] != HUMAN_PIECE)){
				chance = 60;
				connects["deadk-1"]+=1;
			}
			chances.push_back(chance);
		}
		else if (connectk_2){
			if (row != numRows-1 && col != 0 && state[col-1][row+1] != HUMAN_PIECE){
				chance = 50;
				connects["alivek-2"]+=1;
				chances.push_back(chance);
			}
			else if ((row != numRows-1 && col != 0 && state[col-1][row+1] != HUMAN_PIECE) || (col + threat + 1 <= numCols-1 && row - threat - 1 >= 0 && state[col+threat][row-threat] != HUMAN_PIECE && state[col+threat+1][row-threat-1] != HUMAN_PIECE))
				connects["deadk-2"]+=1;
		}
		else if (connectk_3){
			if (row != 0 && col != 0 && state[col-1][row-1] != HUMAN_PIECE && row-k+3 <= numRows-1 && col+k-3 <= numCols-1 && state[col+k-3][row-k+3] != HUMAN_PIECE){
				chance = 30;
				chances.push_back(chance);
			}
		}
		else if (connectk_4 && k>4){
			if (row != numRows-1 && col != 0 && state[col-1][row+1] != HUMAN_PIECE && row-1 >= 0 && col+1 <= numCols-1 && state[col+1][row-1] != HUMAN_PIECE)
				chance = 20;
			else if ((col != 0 && state[col-1][row]!=HUMAN_PIECE) || (col+1 <= numCols-1 && state[col+1][row] != HUMAN_PIECE))
				chance = 10;
			std::cout <<col<<row <<"chance4=" <<chance<<std::endl;
			chances.push_back(chance);
		}
	}

	if (connects["deadk-1"] >= 2 || (connects["deadk-1"] >= 1 && connects["alivek-2"] >= 1))
		chances.push_back(90);
	else if (connects["alivek-2"] >= 2)
		chances.push_back(80);
	else if (connects["deadk-2"]>=1 && connects["alivek-2"] >= 1)
		chances.push_back(70);
//	else if (connects["deadk-3"]>=2)
//		chances.push_back(30);

	chance = maxOf(chances);


	if (attacks["deadk-1"] >= 2 || (attacks["deadk-1"] >= 1 && attacks["alivek-2"] >= 1))
		risks.push_back(-90);
	else if (attacks["alivek-2"] >= 2)
		risks.push_back(-80);
	if (!risks.empty()){
		chance = minOf(risks);
	}
	return chance;
}

int** AIShell::Result(int** state, Move a, int player){
	int** result = NULL;
	result = new int*[numCols];
	for (int i =0; i<numCols; i++){
		result[i] = new int[numRows];
	}
	for (int col = 0; col<numCols; col++){
		for (int row = 0; row<numRows; row++){
			result[col][row] = state[col][row];
		}
	}
	if (gravityOn){
		for (int row = 0; row < numRows; row++){
			if (result[a.col][row] == NO_PIECE){
				result[a.col][row] = player;
				return result;
			}
		}
	}
	result[a.col][a.row] = player;
	return result;
}

bool AIShell::Terminal_Test(int** state){
	int u = utility(state);
	if (u == 100 || u == -100)
		return true;
	for (int col = 0; col<numCols; col++){
		for (int row = 0; row<numRows; row++){
			if (state[col][row] == NO_PIECE){
				return false;
			}
		}
	}
	return true;
}

std::vector<Move> AIShell::Actions(int** state){
	std::vector<Move> actions;
	if (gravityOn){
		for (int col = 0; col<numCols; col++){
			if (state[col][numRows-1] == NO_PIECE)
				actions.push_back(Move(col, numRows-1));
		}
		return actions;
	}
	for (int col = 0; col<numCols; col++){
		for (int row = 0; row<numRows; row++){
			if (state[col][row] == NO_PIECE)
				actions.push_back(Move(col, row));
		}
	}
	return actions;
}

//Move AIShell::Minimax_Decision(int** state){
//	Move m(0, 0);
//	std::vector<Move>::iterator it;
//	std::vector<Move> actions = Actions(state);
//	m = *actions.begin();
//	int v = std::numeric_limits<int>::min();
//	for (it = actions.begin(); it != actions.end(); it++){
//		int temp = v;
//		v = Max(v, Min_Value(Result(state, *it, AI_PIECE)));
//		if (v != temp)
//			m = *it;
//
//	}
//	return m;
//}

Move AIShell::Depth_limited_Alpha_Beta_Search(int** state, int depth){
	Move m(0, 0);
	if (depth == 0){
		for (int col = 0; col<numCols; col++){
			for (int row = 0; row<numRows; row++){
				if (state[col][row] == NO_PIECE){
					Move m(col, row);
					return m;
				}
			}
		}
	}

	int a = std::numeric_limits<int>::min();
	//int v = Max_Value(state, a, std::numeric_limits<int>::max());
	std::vector<Move>::iterator it;
	std::vector<Move>::iterator it2;
	std::vector<Move> actions = Actions(state);
	for (int i = 0; i < actions.size(); i++){
		if (actions[i].col == last_best.col && actions[i].row == last_best.row){
			Move temp = actions[0];
			actions[0] = actions[i];
			actions[i] = temp;
			break;
		}

	}
	m = *actions.begin();
	int v = std::numeric_limits<int>::min();
	for (it = actions.begin(); it != actions.end(); it++){
		int temp = v;
			//v = Max(v, Min_Value(Result(state, *it, AI_PIECE)));
		v = Max(v, Min_Value_Depth(Result(state, *it, AI_PIECE), a, std::numeric_limits<int>::max(), depth-1));
		if (v != temp)
			m = *it;
		ending = time(&ending);
		if (difftime(ending, start)*1000 >= deadline)
			return m;
		a = Max(a, v);

	}
//	for (it = actions.begin(); it != actions.end(); it++){
//		int temp = v;
//		//v = Max(v, Min_Value(Result(state, *it, AI_PIECE)));
//		v = Max_Value_Depth(Result(state, *it, AI_PIECE), std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), depth-1);
//		while (v > a){
//			a = v;
//			m = *it;
//		}
//		ending = time(&ending);
//		if (difftime(ending, start)*1000 >= deadline)
//			return m;
//	}
	return m;
}

//for (it = actions.begin(); it != actions.end(); it++){
//		int temp = v;
//		//v = Max(v, Min_Value(Result(state, *it, AI_PIECE)));
//		v = Max(v, Min_Value_Depth(Result(state, *it, AI_PIECE), a, std::numeric_limits<int>::max(), depth-1));
//		if (v != temp)
//			m = *it;
//		ending = time(&ending);
//		if (difftime(ending, start)*1000 >= deadline)
//			return m;
//		a = Max(a, v);
//
//	}

int AIShell::Max_Value_Depth(int** state, int a, int b, int depth){
	ending = time(&ending);
	if (Terminal_Test(state) || difftime(ending, start)*1000 >= deadline || depth == 0){
		int v = utility(state);
		for (int i =0; i<numCols; i++){
			delete [] state[i];
		}
		delete [] state;
		return v;
	}
	int v = std::numeric_limits<int>::min();
	std::vector<Move>::iterator it;
	std::vector<Move>::iterator it2;
	std::vector<Move> actions = Actions(state);
	for (int i = 0; i < actions.size(); i++){
		if (actions[i].col == last_best.col && actions[i].row == last_best.row){
			Move temp = actions[0];
			actions[0] = actions[i];
			actions[i] = temp;
			break;
		}

	}
	for (it = actions.begin(); it != actions.end(); it++){
		v = Max(v, Min_Value_Depth(Result(state, *it, AI_PIECE), a, b, depth-1));
		ending = time(&ending);
		if (v >= b || difftime(ending, start)*1000 >= deadline){
			for (int i =0; i<numCols; i++){
				delete [] state[i];
			}
			delete [] state;

			return v;

		}
		a = Max(a, v);
	}
	for (int i =0; i<numCols; i++){
		delete [] state[i];
	}
	delete [] state;
	return v;
}

int AIShell::Min_Value_Depth(int** state, int a, int b, int depth){
	ending = time(&ending);
	if (Terminal_Test(state) || difftime(ending, start)*1000 >= deadline || depth == 0){
		int v = utility(state);
		for (int i =0; i<numCols; i++){
			delete [] state[i];
		}
		delete [] state;
		return v;
	}
	int v = std::numeric_limits<int>::max();
	std::vector<Move>::iterator it;
	std::vector<Move>::iterator it2;
	std::vector<Move> actions = Actions(state);

	for (int i = 0; i < actions.size(); i++){
		if (actions[i].col == last_best.col && actions[i].row == last_best.row){
			Move temp = actions[0];
			actions[0] = actions[i];
			actions[i] = temp;
			break;
		}

	}

	for (it = actions.begin(); it != actions.end(); it++){
		v = Min(v, Max_Value_Depth(Result(state, *it, HUMAN_PIECE), a, b, depth-1));
		ending = time(&ending);
		if (v <= a || difftime(ending, start)*1000 >= deadline){
			for (int i =0; i<numCols; i++){
				delete [] state[i];
			}
			delete [] state;
			return v;
		}
		b = Min(v, b);
	}
	for (int i =0; i<numCols; i++){
		delete [] state[i];
	}
	delete [] state;
	return v;
}

Move AIShell::Iterative_Deeping_Search(int** state){
	Move m(0, 0);
	int depth = 0;
	ending = time(&ending);

	while(difftime(ending, start)*1000 <= deadline){
		m = Depth_limited_Alpha_Beta_Search(state, depth++);
		last_best = m;
		ending = time(&ending);
	}

	return m;
}

//Move AIShell::Alpha_Beta_Search(int** state){
//	Move m(0, 0);
//	int a = std::numeric_limits<int>::min();
//	//int v = Max_Value(state, a, std::numeric_limits<int>::max());
//	std::vector<Move>::iterator it;
//	std::vector<Move> actions = Actions(state);
//	m = *actions.begin();
//	int v = std::numeric_limits<int>::min();
//	for (it = actions.begin(); it != actions.end(); it++){
//		int temp = v;
//		//v = Max(v, Min_Value(Result(state, *it, AI_PIECE)));
//		v = Max(v, Min_Value(Result(state, *it, AI_PIECE), a, std::numeric_limits<int>::max()));
//		if (v != temp)
//			m = *it;
//		ending = time(&ending);
//		if (difftime(ending, start)*1000 >= deadline)
//			return m;
//		a = Max(a, v);
//
//	}
//	return m;
//}
//
//int AIShell::Max_Value(int** state, int a, int b){
//	ending = time(&ending);
//	if (Terminal_Test(state) || difftime(ending, start)*1000 >= deadline){
//		int v = utility(state);
//		for (int i =0; i<numCols; i++){
//			delete [] state[i];
//		}
//		delete [] state;
//		return v;
//	}
//	int v = std::numeric_limits<int>::min();
//	std::vector<Move>::iterator it;
//	std::vector<Move> actions = Actions(state);
//	for (it = actions.begin(); it != actions.end(); it++){
//		v = Max(v, Min_Value(Result(state, *it, AI_PIECE), a, b));
//		ending = time(&ending);
//		if (v >= b || difftime(ending, start)*1000 >= deadline)
//			return v;
//		a = Max(a, v);
//	}
//	for (int i =0; i<numCols; i++){
//		delete [] state[i];
//	}
//	delete [] state;
//	return v;
//}
//
//int AIShell::Min_Value(int** state, int a, int b){
//	ending = time(&ending);
//	if (Terminal_Test(state) || difftime(ending, start)*1000 >= deadline){
//		int v = utility(state);
//		for (int i =0; i<numCols; i++){
//			delete [] state[i];
//		}
//		delete [] state;
//		return v;
//	}
//	int v = std::numeric_limits<int>::max();
//	std::vector<Move>::iterator it;
//	std::vector<Move> actions = Actions(state);
//	for (it = actions.begin(); it != actions.end(); it++){
//		v = Min(v, Max_Value(Result(state, *it, HUMAN_PIECE), a, b));
//		ending = time(&ending);
//		if (v <= a || difftime(ending, start)*1000 >= deadline)
//			return v;
//		b = Min(v, b);
//	}
//	for (int i =0; i<numCols; i++){
//		delete [] state[i];
//	}
//	delete [] state;
//	return v;
//}

//int AIShell::Max_Value(int** state){
//	if (Terminal_Test(state)){
//		int v = utility(state);
//		for (int i =0; i<numCols; i++){
//			delete [] state[i];
//		}
//		delete [] state;
//		return v;
//	}
//	int v = std::numeric_limits<int>::min();
//	std::vector<Move>::iterator it;
//	std::vector<Move> actions = Actions(state);
//	for (it = actions.begin(); it != actions.end(); it++){
//		v = Max(v, Min_Value(Result(state, *it, AI_PIECE)));
//	}
//	for (int i =0; i<numCols; i++){
//		delete [] state[i];
//	}
//	delete [] state;
//	return v;
//}

//int AIShell::Min_Value(int** state){
//	if (Terminal_Test(state)){
//		int v = utility(state);
//		for (int i =0; i<numCols; i++){
//			delete [] state[i];
//		}
//		delete [] state;
//		return v;
//	}
//	int v = std::numeric_limits<int>::max();
//	std::vector<Move>::iterator it;
//	std::vector<Move> actions = Actions(state);
//	for (it = actions.begin(); it != actions.end(); it++){
//		v = Min(v, Max_Value(Result(state, *it, HUMAN_PIECE)));
//	}
//	for (int i =0; i<numCols; i++){
//		delete [] state[i];
//	}
//	delete [] state;
//	return v;
//}


Move AIShell::makeMove(){
	//this part should be filled in by the student to implement the AI
	//Example of a move could be: Move move(1, 2); //this will make a move at col 1, row 2
	
	start = time(&start);
//	return Minimax_Decision(gameState);
//	return Alpha_Beta_Search(gameState);
	return Iterative_Deeping_Search(gameState);

	//this will move to the left-most column possible.
//	for (int col = 0; col<numCols; col++){
//		for (int row = 0; row<numRows; row++){
//			if (gameState[col][row] == NO_PIECE){
//				Move m(col, row);
//				return m;
//			}
//		}
//	}
//	for (int row = 0; row<numRows; row++){
//			for (int col = 0; col<numCols; col++){
//				if (gameState[col][row] == NO_PIECE){
//					Move m(col, row);
//					return m;
//				}
//			}
//	}
	Move m(0, 0);
	return m;
	 
}
