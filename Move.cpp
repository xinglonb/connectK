#include "Move.h"


Move::Move()
{
	row = 0;
	col = 0;
}
Move::Move(int col, int row){
	this->row=row;
	this->col=col;
}

//int Move::getRow(){
//	return row;
//}
//
//int Move::getCol(){
//	return col;
//}
//
//bool Move::operator == (const Move rhs) const{
//	return true;
//}
//
//bool Move::operator < (const Move rhs) const{
//	return true;
//}
//
//bool Move::operator > (const Move rhs) const{
//	return true;
//}

