#pragma once
#include "Move.h"
#include <list>


class MoveList
{
public:
	MoveList(int clientId);
	void addMove(float deltaX,float deltaY);
	void addMove(Move move);
	int getMoveIndex();
	Move getOldestMove();
	void removeOldestMove();
	std::list<Move>::const_iterator getMoveListBeginIterator()const;
	std::list<Move>::const_iterator getMoveListEndIterator() const;
	int getMoveListSize()const;
	int getClientId() const { return clientId; }
	void setClientId(int clientId) { this->clientId = clientId; }
	~MoveList();
private:
	std::list<Move> moveList;
	int moveIndex;
	int clientId;
};
