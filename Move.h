#ifndef MOVE_H
#define MOVE_H

#pragma once
class Move
{
public:
	int row; //the row to move to. 
	int col; //the col to move to.
	Move();
	Move(int col, int row);

//	int getRow();
//	int getCol();
//	virtual bool operator == (const Move rhs) const;
//	virtual bool operator < (const Move rhs) const;
//	virtual bool operator > (const Move rhs) const;
};

#endif //MOVE_H
