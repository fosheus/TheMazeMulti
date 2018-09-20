#pragma once
#include <SFML\Graphics.hpp>
#include "EntityModel.h"
#include "MoveList.h"
class IServerCallback
{
public:
	
	virtual void levelCompleted(sf::Uint16 id)=0;
	virtual EntityModel* newClientConnected() = 0;
	virtual bool entityModel(EntityModel& em) = 0;
	virtual bool newGame(sf::Uint16 id) = 0;
	virtual bool levelGenerated(sf::Uint16 id) = 0;
	virtual bool removeClient(sf::Uint16 id) = 0;
	virtual void changeClientName(std::pair<sf::Uint16, sf::String> name) = 0;
	virtual void updateFromMoveList(MoveList & moveList) = 0;

	virtual EntityModel* getEntityModelById(sf::Uint16 id) =0;
	virtual std::map<sf::Uint16, EntityModel*> getAllEntities() = 0;
	virtual sf::Uint16 getNextEntityId() = 0;
};

