#include "GameServer.h"

GameServer::GameServer(GameDataRef data, int port): _data(data),maze(data), threadServer(&GameServer::run,this),
	endpoint("127.0.0.1",port),adapter(this),
	server(yojimbo::GetDefaultAllocator(),DEFAULT_PRIVATE_KEY,endpoint,connectionConfig,adapter,0.0f)
{
	if (!server.IsRunning()) {
		throw std::runtime_error("Could not start server at port " + std::to_string(this->endpoint.GetPort()));
	}
}

GameServer::~GameServer()
{
}

void GameServer::run()
{
	float dt=0.0f;
	float fixedDt = 2.0f / 60.0f;
	float accumulator=0.0f;
	while (started) {
		dt = clock.restart().asSeconds();
		accumulator += dt;
		if (accumulator >= fixedDt) {
			update(dt);
			accumulator = 0;
		}

	}
}

void GameServer::update(float dt)
{
	if (!server.IsRunning()) {
		started = false;
		return;
	}

	server.AdvanceTime(dt);
	server.ReceivePackets();
	processMessages();

	server.SendPackets();
}

void GameServer::processMessages()
{
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (server.IsClientConnected(i)) {
			for (int j = 0; i < connectionConfig.numChannels; j++) {
				yojimbo::Message* message = server.ReceiveMessage(i, j);
				while (message != NULL) {
					processMessage(i, message);
					server.ReleaseMessage(i, message);
					message = server.ReceiveMessage(i, j);
				}
			}
		}
	}
}

void GameServer::processMessage(int clientIndex, Message * message)
{
	switch (message->GetType())
	{
	case (int)GameMessageType::MOVE_MESSAGE:
		processMoveMessage(clientIndex,(MoveMessage*) message);
		break;
	default:
		break;
	}
}

void GameServer::processMoveMessage(int clientIndex, MoveMessage* message)
{
	MoveMessage* moveMessage = (MoveMessage*)message;
	EntityModel* e = &players[clientIndex];
	Move m(moveMessage->deltaX, moveMessage->deltaY, moveMessage->moveId);

	if (moveMessage->moveId > e->getLastMoveId()) {
		e->setLastMoveId(moveMessage->moveId);
		e->updateFromMove(m);
		managePlayerWin(e);
		if (collisionManagement(e)) {
			e->rollbackMove(m);
		}

	}
}


void GameServer::startServer()
{
	started = true;
	threadServer.launch();
}

void GameServer::stopServer()
{
	server.DisconnectAllClients();
	started = false;
	threadServer.wait();

}

/*

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




void GameServer::clientConnected(int clientIndex)
{
	entities[indexNextClient] = new EntityModel(clientIndex, 0, 0, 0, 0);
}

void GameServer::clientDisconnected(int clientIndex)
{
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
*/

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
			broadcastPlayerWon(e->getId());
			level++;
			e->setScore(e->getScore() + 1);
		}
	}
}

void GameServer::broadcastPlayerWon(int clientIndex)
{
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (server.IsClientConnected(i)) {
			PlayerWonMessage* message = (PlayerWonMessage*)server.CreateMessage(i, PLAYER_WON_MESSAGE);
			message->playerIndex = clientIndex;
			server.SendMessage(i, (int)GameChannel::RELIABLE,message);
		}
	}
}
/*
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
*/