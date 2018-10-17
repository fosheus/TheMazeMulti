#pragma once
#include <SFML/Network.hpp>
#include "Move.h"
#include <list>


class MoveList
{
public:
	MoveList(sf::Uint16 clientId);
	void addMove(float deltaX,float deltaY);
	void addMove(Move move);
	sf::Uint32 getMoveIndex();
	Move getOldestMove();
	void removeOldestMove();
	std::list<Move>::const_iterator getMoveListBeginIterator()const;
	std::list<Move>::const_iterator getMoveListEndIterator() const;
	int getMoveListSize()const;
	sf::Uint16 getClientId() const { return clientId; }
	void setClientId(sf::Uint16 clientId) { this->clientId = clientId; }
	~MoveList();
private:
	std::list<Move> moveList;
	sf::Uint32 moveIndex;
	sf::Uint16 clientId;
};
