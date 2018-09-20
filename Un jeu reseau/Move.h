#pragma once
#include <SFML/Network.hpp>

class Move 
{
public:
	Move() {}
	Move(float deltaX,float delataY,sf::Uint32 moveId);
	float getDeltaX()const { return deltaX; }
	float getDeltaY()const { return deltaY; }
	sf::Uint32 getMoveId()  const{ return moveId; }
	void setDeltaX(float deltaX) { this->deltaX = deltaX; }
	void setDeltaY(float deltaY) { this->deltaY = deltaY; }
	void setMoveId(sf::Uint32 moveId) { this->moveId = moveId; }
	~Move();

private:
	float deltaX;
	float deltaY;
	sf::Uint32 moveId;

};


sf::Packet& operator << (sf::Packet& packet, const Move& m);

sf::Packet& operator >> (sf::Packet& packet, Move& m);

