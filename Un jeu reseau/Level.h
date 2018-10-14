#pragma once
#include <vector>
#include "MoveList.h"
#include "EntityModel.h"
class Level
{
public:
	Level();
	~Level();
	std::vector<EntityModel> entities;
	MoveList movelist;

};

sf::Packet& operator << (sf::Packet& packet, const Level & l);

sf::Packet& operator >> (sf::Packet& packet, Level & l);

