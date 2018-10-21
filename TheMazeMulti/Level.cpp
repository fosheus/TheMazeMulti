#include "Level.h"



Level::Level()
{
}


Level::~Level()
{
}


std::vector<EntityModel>& Level::getPlayers() {
	return players;
}

EntityModel* Level::getPlayerByIndex(int playerIndex) {
	for (int i = 0; i < players.size(); i++) {
		if (players[i].getId() == playerIndex) {
			return &players[i];
		}
	}
	return NULL;
}

void Level::newPlayer(EntityModel player) {
	players.push_back(player);
}

void Level::removePlayer(int clientIndex) {
	std::vector<EntityModel>::iterator it;
	for (it = players.begin(); it != players.end(); it++) {
		if (it->getId() == clientIndex) {
			players.erase(it);
			return;
		}
	}
}

void Level::updatePlayerName(int clientIndex,std::string playerName)
{
	players[clientIndex].setName(playerName);
}
