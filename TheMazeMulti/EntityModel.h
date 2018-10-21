#pragma once
#include <yojimbo.h>
#include <iostream>
#include "Move.h"
using namespace yojimbo;

class EntityModel : public yojimbo::Serializable
{
public:
	EntityModel(int id,float x,float y,int score,int lastMoveId);
	EntityModel() {}
	EntityModel(int id);
	void setId(int id);
	void setX(float x);
	void setY(float y);
	void setName(std::string name);
	void setScore(int score);
	void setLastMoveId(int lastMoveId);
	int getId() const;
	float getX() const;
	float getY() const;
	std::string getName() const;
	int getScore() const;
	int getLastMoveId() const;
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
	std::string name;
	int score;
	int lastMoveId;
};


