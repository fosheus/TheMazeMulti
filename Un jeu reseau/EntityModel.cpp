#include "EntityModel.h"

EntityModel::EntityModel(int id,float x,float y, int score, int lastMoveId)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->score = score;
	this->lastMoveId = lastMoveId;
}



EntityModel::EntityModel(int id)
{
	this->id = id;
	this->x = 0;
	this->y = 0;
	this->score = 0;
	this->lastMoveId = -1;
}

void EntityModel::setId(int id)
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

int EntityModel::getId() const
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

void EntityModel::setName(std::string name)
{
	this->name = name;
}

void EntityModel::setScore(int score)
{
	this->score = score;
}

void EntityModel::setLastMoveId(int lastMoveId)
{
	this->lastMoveId = lastMoveId;
}

std::string EntityModel::getName() const
{
	return this->name;
}

int EntityModel::getScore() const
{
	return score;
}

int EntityModel::getLastMoveId() const
{
	return lastMoveId;
}

void EntityModel::updateFromMove(const Move & move)
{
	this->x += move.getDeltaX();
	this->y += move.getDeltaY();
	this->lastMoveId = move.getMoveId();
}

void EntityModel::rollbackMove(const Move & move)
{
	this->x -= move.getDeltaX();
	this->y -= move.getDeltaY();
}

EntityModel::~EntityModel()
{
}

