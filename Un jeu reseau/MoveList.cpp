#include "MoveList.h"



MoveList::MoveList(sf::Uint16 clientId)
{
	moveIndex = 0;
	this->clientId = clientId;
}

void MoveList::addMove(float deltaX,float deltaY)
{
	moveList.push_back(Move(deltaX,deltaY,moveIndex++));
}

void MoveList::addMove(Move move)
{
	moveList.push_back(move);
}

sf::Uint32 MoveList::getMoveIndex()
{
	return moveIndex;
}

Move MoveList::getOldestMove()
{
	return moveList.front();
}

void MoveList::removeOldestMove()
{
	moveList.pop_front();
}

std::list<Move>::const_iterator MoveList::getMoveListBeginIterator() const 
{
	return moveList.cbegin();
}

std::list<Move>::const_iterator MoveList::getMoveListEndIterator() const
{
	return moveList.cend();
}

int MoveList::getMoveListSize() const
{
	return moveList.size();
}



MoveList::~MoveList()
{
}
