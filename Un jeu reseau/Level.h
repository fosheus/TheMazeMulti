#pragma once
#include "yojimbo.h"
#include "EntityModel.h"
#include <vector>

class Level : public yojimbo::Serializable
{

public:
	Level();
	template <typename Stream>bool Serialize(Stream &stream) {
		serialize_int(stream, players.size(), 0, 64);
		for (int i = 0; i < players.size(); i++) {
			serialize_object(stream, players[i]);
		}
	}
	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

	std::vector<EntityModel> getPlayers() { return players; }
	~Level();

private :
	std::vector<EntityModel> players;
};



