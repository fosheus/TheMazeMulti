#pragma once
#include <yojimbo.h>
using namespace yojimbo;

//TODO rajouter serialization yojimbo

class Move 
{
public:
	Move() {}
	Move(float deltaX,float delataY,int moveId);
	float getDeltaX()const { return deltaX; }
	float getDeltaY()const { return deltaY; }
	int getMoveId()  const{ return moveId; }
	void setDeltaX(float deltaX) { this->deltaX = deltaX; }
	void setDeltaY(float deltaY) { this->deltaY = deltaY; }
	void setMoveId(int moveId) { this->moveId = moveId; }
	~Move();

private:
	float deltaX;
	float deltaY;
	int moveId;

};
