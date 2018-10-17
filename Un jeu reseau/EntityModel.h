#pragma once
#include <SFML\Network.hpp>
#include "Move.h"
#include <yojimbo.h>

using namespace yojimbo;

class EntityModel : public yojimbo::Serializable
{
public:
	EntityModel(sf::Uint16 id,float x,float y,sf::Uint16 score,sf::Uint32 lastMoveId);
	EntityModel() {}
	EntityModel(int id);
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

	template <typename Stream> bool Serialize(Stream & stream) {
		
		serialize_int(stream, id, 0, 64);
		serialize_float(stream, x);
		serialize_float(stream, y);
		serialize_bits(stream, score,32);
		serialize_bits(stream, lastMoveId,32);
		return true;
	}

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

	~EntityModel();

private :
	float x;
	float y;
	int id;
	sf::String name;
	sf::Uint16 score;
	sf::Uint32 lastMoveId;
};


