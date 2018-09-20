#pragma once
#include "EntityModel.h"
#include "MazeConfig.h"
#include <vector>

class IClientCallback
{
public:
	virtual void welcomed(sf::Uint16 identifier)=0;
	virtual void updateEntityModel(std::vector<EntityModel>& em) = 0;
	virtual void lostConnection() = 0;
	virtual void removeEntity(sf::Uint16 id) = 0;
	virtual void generateLevel(MazeConfig config) = 0;
	virtual void startGame() = 0;
	virtual void levelCompleted(sf::Uint16 id) = 0;
	virtual void updateEntityName(std::pair<sf::Uint16, sf::String> nameIdPair)=0;
	virtual void ping() = 0;
};

