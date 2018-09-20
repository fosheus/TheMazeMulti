#include "GameServer.h"

GameServer::GameServer(GameDataRef data, int port): _data(data),packetManager(data,this,port),maze(data), threadServer(&GameServer::serverWork,this)
{
	indexNextClient = 1;
}

GameServer::~GameServer()
{
}

EntityModel* GameServer::newClientConnected()
{
	entities[indexNextClient] = new EntityModel(indexNextClient, 0, 0,0,0);
	indexNextClient++;
	return entities[indexNextClient-1];
}

bool GameServer::entityModel(EntityModel & em)
{
	float deltaX, deltaY;
	if (entities[em.getId()] != nullptr) {
		
		if (maze.getGenerated()) {
			int x = (int)em.getX();
			int y = (int)em.getY();
			if (x >= 0 || x < maze.getWidth() && y >= 0 && y < maze.getHeight()) {
				if (maze.getMaze()[x][y] != 0) {
					entities[em.getId()]->setX(em.getX());
					entities[em.getId()]->setY(em.getY());
				}
				sf::Vector2f pos = maze.getExitPos();
				if (x == pos.x && y == pos.y) {
					entities[em.getId()]->setScore(em.getScore() + 1);
					packetManager.broadcastLevelCompleted(em.getId());
					level++;
					maze.clearMaze();
				}
			}
		}
		else {
			entities[em.getId()]->setX(em.getX());
			entities[em.getId()]->setY(em.getY());
		}
		
		packetManager.broadcastEntityModel(*entities[em.getId()]);
		return true;
	}
	return false;

}

bool GameServer::removeClient(sf::Uint16 id)
{
	
	entities.erase(id);
	return true;
}

bool GameServer::newGame(sf::Uint16 id)
{
	sf::Uint32 seed;
	sf::Uint16 width;
	sf::Uint16 height;
	std::map<sf::Uint16, EntityModel*>::iterator it;
	
	if (id == 1 && !maze.getGenerated()) {
		width = 11 + level * 2;
		height = 11 + level * 2;
		srand(time(NULL));
		seed = rand();
		maze.clearMaze();
		maze.generateMaze(seed, width, height);
		for (it = entities.begin(); it != entities.end(); it++) {
			it->second->setX(0);
			it->second->setY(1);
			packetManager.broadcastEntityModel(*it->second);
		}
		MazeConfig config(seed, width, height);
		packetManager.sendSeedDimension(config);
		
		return true;
	}
	return false;
}

bool GameServer::levelGenerated(sf::Uint16 id)
{
	return false;
}

EntityModel* GameServer::getEntityModelById(sf::Uint16 id)
{
	return entities[id];
}

void GameServer::startServer()
{
	started = true;
	packetManager.startServer();
	threadServer.launch();
}

void GameServer::stopServer()
{
	packetManager.stopServer();
	started = false;
	threadServer.wait();

}

void GameServer::update(float dt)
{
	packetManager.handleClients();
}

sf::Uint16 GameServer::getNextEntityId()
{
	return indexNextClient;
}

std::map<sf::Uint16, EntityModel*> GameServer::getAllEntities()
{
	return entities;
}

void GameServer::levelCompleted(sf::Uint16 id)
{
}

void GameServer::serverWork()
{
	float dt;
	float sendTimeout = 0;
	float broadcastTimeout = 0;
	while (started) {
		dt = clock.getElapsedTime().asSeconds();
		clock.restart();
		sendTimeout += dt;
		broadcastTimeout += dt;
		packetManager.receiveData();

		if (sendTimeout > 4.0f/60.0f) {
			packetManager.sendWaitingData();
			sendTimeout = 0;
		}
		

	}
}

bool GameServer::collisionManagement(EntityModel * e)
{
	if (maze.getGenerated()) {
		int x = (int)e->getX();
		int y = (int)e->getY();
		if (x >= 0 || x < maze.getWidth() && y >= 0 && y < maze.getHeight()) {
			if (maze.getMaze()[x][y] != 0) {
				return false;
			} 
			return true;
		}
		else {
			e->setX(0);
			e->setY(1);
			return false;
		}
	}
	return false;
}

void GameServer::managePlayerWin(EntityModel * e)
{
	sf::Vector2f exit;
	if (maze.getGenerated()) {
		exit = maze.getExitPos();
		int x = e->getX();
		int y = e->getY();
		if (x == exit.x && y == exit.y) {
			maze.clearMaze();
			packetManager.broadcastLevelCompleted(e->getId());
			level++;
			e->setScore(e->getScore() + 1);
		}
	}
}

void GameServer::changeClientName(std::pair<sf::Uint16,sf::String> name)
{
	std::map<sf::Uint16, EntityModel*>::iterator it;
	entities[name.first]->setName(name.second);
	for (it = entities.begin(); it != entities.end(); it++) {
		packetManager.broadcastName(it->first, it->second->getName());
	}
}

void GameServer::updateFromMoveList(MoveList & moveList)
{
	EntityModel* e = entities[moveList.getClientId()];
	if (e != nullptr) {
		std::list<Move>::const_iterator it = moveList.getMoveListBeginIterator();
		while (it != moveList.getMoveListEndIterator()) {
			if (it->getMoveId() > e->getLastMoveId()) {
				e->setLastMoveId(it->getMoveId());
				e->updateFromMove(*it);
				managePlayerWin(e);
				if (collisionManagement(e)) {
					e->rollbackMove(*it);
				}
				
			}
			it++;
		}
		
		packetManager.broadcastEntityModel(*e);
	}
}
