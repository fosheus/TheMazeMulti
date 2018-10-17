#include "Move.h"

Move::Move(float deltaX, float delataY, int moveId)
{
	this->deltaX = deltaX;
	this->deltaY = delataY;
	this->moveId = moveId;
}

Move::~Move()
{
}

