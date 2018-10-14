#include "Level.h"



Level::Level() : movelist(0)
{
	
}


Level::~Level()
{
}

sf::Packet & operator<<(sf::Packet & packet, const Level & l)
{
	packet << (sf::Uint8)l.entities.size();
	for (int i = 0; i < l.entities.size(); i++) {
		packet << l.entities[i];
	}
	packet << l.movelist;

	return packet;
}

sf::Packet & operator>>(sf::Packet & packet, Level & l)
{
	std::vector<EntityModel> entities;
	MoveList moveList(0);
	sf::Uint8 size;
	packet >> size;
	EntityModel entity;
	for (int i = 0; i < size; i++) {
		packet >> entity;
		entities.push_back(entity);
	}

	packet >> moveList;

	l.entities = entities;
	l.movelist = moveList;

	return packet;


}
