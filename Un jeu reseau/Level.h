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

	std::vector<EntityModel>& getPlayers();
	EntityModel* getPlayerByIndex(int playerIndex);
	void newPlayer(EntityModel player);
	void removePlayer(int clientIndex);
	void updatePlayerName(int clientIndex,std::string playerName);
	~Level();

private :
	std::vector<EntityModel> players;
};



