#pragma once
#include "yojimbo.h"
#include "EntityModel.h"
#include <vector>

using namespace yojimbo;

class Level : public yojimbo::Serializable
{

public:
	Level();
	template <typename Stream>bool Serialize(Stream &stream) {
		
		int size;
		if (Stream::IsReading) {
			players.clear();
			serialize_int(stream, size, 0, 64);
			for (int i = 0; i < size; i++) {
				players.push_back(EntityModel());
				serialize_object(stream, players[i]);
			}
			return true;
		}
		if (Stream::IsWriting){
			size = players.size();
			serialize_int(stream, size, 0, 64);
			for (int i = 0; i < size; i++) {
				serialize_object(stream, players[i]);
			}
			return true;
		}
		return false;
		
	}
	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

	std::vector<EntityModel>& getPlayers() {
		return players;
	}
	EntityModel* getPlayerByIndex(int playerIndex) {
		for (int i = 0; i < players.size(); i++) {
			if (players[i].getId() == playerIndex) {
				return &players[i];
			}
		}
		return NULL;
	}
	void newPlayer(EntityModel player) {
		players.push_back(player);
	}
	void removePlayer(int clientIndex) {
		std::vector<EntityModel>::iterator it;
		for (it = players.begin(); it != players.end();it++) {
			if (it->getId() == clientIndex) {
				players.erase(it);
				return;
			}
		}
	}
	~Level();

private :
	std::vector<EntityModel> players;
};



