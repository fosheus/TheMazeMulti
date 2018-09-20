#include "EntityModel.h"

EntityModel::EntityModel(sf::Uint16 id,float x,float y,sf::Uint16 score,sf::Uint32 lastMoveId)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->score = score;
	this->lastMoveId = lastMoveId;
}



void EntityModel::setId(sf::Uint16 id)
{
	this->id = id;
}


void EntityModel::setX(float x)
{
	this->x = x;
}

void EntityModel::setY(float y)
{
	this->y = y;
}

sf::Uint16 EntityModel::getId() const
{
	return this->id;
}

float EntityModel::getX() const
{
	return this->x;
}

float EntityModel::getY() const
{
	return this->y;
}

void EntityModel::setName(sf::String name)
{
	this->name = name;
}

void EntityModel::setScore(sf::Uint16 score)
{
	this->score = score;
}

void EntityModel::setLastMoveId(sf::Uint32 lastMoveId)
{
	this->lastMoveId = lastMoveId;
}

sf::String EntityModel::getName() const
{
	return this->name;
}

sf::Uint16 EntityModel::getScore() const
{
	return score;
}

sf::Uint32 EntityModel::getLastMoveId() const
{
	return lastMoveId;
}

void EntityModel::updateFromMove(const Move & move)
{
	this->x += move.getDeltaX();
	this->y += move.getDeltaY();
}

void EntityModel::rollbackMove(const Move & move)
{
	this->x -= move.getDeltaX();
	this->y -= move.getDeltaY();
}

EntityModel::~EntityModel()
{
}

sf::Packet& operator << (sf::Packet& packet, const EntityModel& em) {
	return packet << em.getId() << em.getX() << em.getY() << em.getScore()<<em.getLastMoveId();
}

sf::Packet& operator >> (sf::Packet& packet, EntityModel& em) {
	sf::Uint16 id;
	float x;
	float y;
	sf::Uint16 score;
	sf::Uint32 lastMoveId;

	packet >> id >> x >> y >> score>>lastMoveId;

	em.setId(id);
	em.setX(x);
	em.setY(y);
	em.setScore(score);
	em.setLastMoveId(lastMoveId);

	return packet;
}
