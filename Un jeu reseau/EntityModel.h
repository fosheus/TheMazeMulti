#pragma once
#include <SFML\Network.hpp>
#include "Move.h"
class EntityModel
{
public:
	EntityModel(sf::Uint16 id,float x,float y,sf::Uint16 score,sf::Uint32 lastMoveId);
	EntityModel() {}
	void setId(sf::Uint16 id);
	void setX(float x);
	void setY(float y);
	void setName(sf::String name);
	void setScore(sf::Uint16 score);
	void setLastMoveId(sf::Uint32 lastMoveId);
	sf::Uint16 getId() const;
	float getX() const;
	float getY() const;
	sf::String getName() const;
	sf::Uint16 getScore() const;
	sf::Uint32 getLastMoveId() const;
	void updateFromMove(const Move& move);
	void rollbackMove(const Move& move);

	~EntityModel();

private :
	float x;
	float y;
	sf::Uint16 id;
	sf::String name;
	sf::Uint16 score;
	sf::Uint32 lastMoveId;
};



sf::Packet& operator << (sf::Packet& packet, const EntityModel& em);

sf::Packet& operator >> (sf::Packet& packet, EntityModel& em);
