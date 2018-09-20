#include "Move.h"

Move::Move(float deltaX, float delataY, sf::Uint32 moveId)
{
	this->deltaX = deltaX;
	this->deltaY = delataY;
	this->moveId = moveId;
}

Move::~Move()
{
}

sf::Packet & operator<<(sf::Packet & packet, const Move & m)
{
	return packet << m.getDeltaX() << m.getDeltaY() << m.getMoveId();
	
}

sf::Packet & operator>>(sf::Packet & packet, Move & m)
{
	float deltaX;
	float deltaY;
	sf::Uint32 moveId;

	packet >> deltaX >> deltaY >> moveId;

	m.setDeltaX(deltaX);
	m.setDeltaY(deltaY);
	m.setMoveId(moveId);

	return packet;
}
