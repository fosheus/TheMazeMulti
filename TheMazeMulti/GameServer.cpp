#include "GameServer.h"

GameServer::GameServer(): maze(), threadServer(&GameServer::run,this), m_adapter(this),
	server(yojimbo::GetDefaultAllocator(),DEFAULT_PRIVATE_KEY,Address(sf::IpAddress::getLocalAddress().toString().c_str(),ServerPort),m_connectionConfig,m_adapter,0.0f)
{
}

GameServer::~GameServer()
{
	server.Stop();
}

void GameServer::run()
{
	float dt=0.0f;
	float fixedDt = 2.0f / 60.0f;
	float accumulator=0.0f;
	serverTime = 0.0f;
	while (started) {
		dt = clock.restart().asSeconds();
		accumulator += dt;
		if (accumulator >= fixedDt) {
			update(dt);
			accumulator = 0;
		}
	}
	server.DisconnectAllClients();
	
}

void GameServer::update(float dt)
{
	if (!server.IsRunning()) {
		started = false;
		return;
	}
	serverTime += dt;

	server.AdvanceTime(serverTime);
	server.ReceivePackets();
	processMessages();

	sendLevel();

	server.SendPackets();
}

void GameServer::processMessages()
{
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (server.IsClientConnected(i)) {
			for (int j = 0; j < m_connectionConfig.numChannels; j++) {
				Message* message = server.ReceiveMessage(i, j);
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
	case (int)GameMessageType::PLAYER_NAME_MESSAGE:
		processPlayerNameMessage(clientIndex, (PlayerNameMessage*)message);
		break;
	case (int)GameMessageType::GAME_EVENT_MESSAGE:
		processGameEventMessage(clientIndex, (GameEventMessage*)message);
		break;
	default:
		break;
	}
}

void GameServer::processMoveMessage(int clientIndex, MoveMessage* message)
{
	MoveMessage* moveMessage = (MoveMessage*)message;
	EntityModel *e = level.getPlayerByIndex(clientIndex);
	if (moveMessage->moveId > e->getLastMoveId()) {
		Move mX(moveMessage->deltaX, 0, moveMessage->moveId);
		Move mY(0, moveMessage->deltaY, moveMessage->moveId);
		
		e->updateFromMove(mX);
		if (collisionManagement(e)) {
			e->rollbackMove(mX);
		}
		e->updateFromMove(mY);
		if (collisionManagement(e)) {
			e->rollbackMove(mY);
		}
		managePlayerWin(e);

	}
}

void GameServer::processPlayerNameMessage(int clientIndex, PlayerNameMessage* message) {

	std::cout << "server receive player name message from : " << clientIndex << " with name = " << message->name<<std::endl;
	if (message->name.size() > 0) {
		level.updatePlayerName(clientIndex, message->name);
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (server.IsClientConnected(i)) {
				PlayerNameMessage* outMessage = (PlayerNameMessage*)server.CreateMessage(i, (int)GameMessageType::PLAYER_NAME_MESSAGE);
				outMessage->clientIndex = clientIndex;
				outMessage->name = message->name;
				std::cout << "server sends player name message to : " << i << " with name = " << outMessage->name<< "and clientIndex =" <<clientIndex<< std::endl;
				server.SendMessage(i, (int)GameChannel::RELIABLE, outMessage);
			}
		}
	}
	else {
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (server.IsClientConnected(i)) {
				EventCDPlayerMessage* outMessage = (EventCDPlayerMessage*)server.CreateMessage(i, (int)GameMessageType::EVENT_CD_PLAYER_MESSAGE);
				outMessage->clientIndex = clientIndex;
				outMessage->action = 0;
				server.SendMessage(i, (int)GameChannel::RELIABLE, outMessage);
			}
		}
		server.DisconnectClient(clientIndex);
	}
}

void GameServer::processGameEventMessage(int clientIndex, GameEventMessage* message) {
	switch(message->gameEventType){
	case (int)GameEventType::NEW_GAME:
		processNewGame(clientIndex);
		break;
	default:
		break;
	}
}

void GameServer::processNewGame(int clientIndex)
{
	int seed;
	int width;
	int height;
	if (clientIndex == 0 && !maze.isGenerated()) {
		width = 11 + difficulty * 2;
		height = 11 + difficulty * 2;
		srand(time(NULL));
		seed = rand();
		maze.clearMaze();
		mazeConfig = MazeConfig(true,seed, width, height);
		maze.generateMaze(seed, width, height);
		level.setMazeStatus(true);
		for (int i = 0; i < MAX_PLAYERS; i++) {
			EntityModel* current = level.getPlayerByIndex(i); 
			if (current != NULL) {
				current->setX(0);
				current->setY(1);
				GenerateMazeMessage* outMessage = (GenerateMazeMessage*)server.CreateMessage(i, (int)GameMessageType::GENERATE_MAZE_MESSAGE);
				outMessage->mazeConfig = mazeConfig;
				server.SendMessage(i, (int)GameChannel::RELIABLE, outMessage);
			}
		}
	}
}


void GameServer::sendLevel()
{
	std::vector<EntityModel> playerModels = level.getPlayers();
	for (int i = 0; i < playerModels.size(); i++) {
		if (playerModels[i].getId() >= 0) {
			LevelStateMessage* message = (LevelStateMessage*)server.CreateMessage(playerModels[i].getId(), (int)GameMessageType::LEVEL_STATE_MESSAGE);
			message->level = level;
			server.SendMessage(playerModels[i].getId(), (int)GameChannel::UNRELIABLE, message);
		}

	}
}


void GameServer::startServer()
{
	server.Start(MAX_PLAYERS);
	if (!server.IsRunning()) {
		server.Stop();
		throw std::runtime_error("Could not start server at port " + std::to_string(this->endpoint.GetPort()));
	}
	started = true;
	difficulty = 0;
	threadServer.launch();
}

void GameServer::stopServer()
{
	started = false;
	mazeConfig.status = false;
	maze.clearMaze();
	threadServer.wait();
	
}
bool GameServer::collisionManagement(EntityModel * e)
{
	if (maze.isGenerated()) {
		int x = (int)e->getX();
		int y = (int)e->getY();
		if (x >= 0 && x < maze.getWidth() && y >= 0 && y < maze.getHeight()) {
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
	if (maze.isGenerated()) {
		exit = maze.getExitPos();
		int x = e->getX();
		int y = e->getY();
		if (x == exit.x && y == exit.y) {
			maze.clearMaze();
			level.setMazeStatus(false);
			broadcastPlayerWon(e->getId());
			difficulty++;
			e->setScore(e->getScore() + 1);
			mazeConfig.status = false;
			
		}
	}
}

void GameServer::broadcastPlayerWon(int clientIndex)
{
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (server.IsClientConnected(i)) {
			PlayerWonMessage* message = (PlayerWonMessage*)server.CreateMessage(i, (int)GameMessageType::PLAYER_WON_MESSAGE);
			message->playerIndex = clientIndex;
			server.SendMessage(i, (int)GameChannel::RELIABLE,message);
		}
	}
}
void GameServer::clientConnection(int clientIndex)
{
	std::cout << "server detects client connection on slot :" << clientIndex << std::endl;
	//send initial state of the game to the new client: players position and maze configuration 
	InitialLevelStateMessage* message = (InitialLevelStateMessage*)server.CreateMessage(clientIndex, (int)GameMessageType::INITIAL_LEVEL_STATE_MESSAGE);
	message->mazeConfig = mazeConfig;
	message->level = level;
	server.SendMessage(clientIndex, (int)GameChannel::RELIABLE, message);
	
	//send player creation to all other players
	for (int i = 0; i < MAX_PLAYERS; i++) {
		EntityModel* em = level.getPlayerByIndex(i);
		if (em != NULL) {
			//send client creation to other clients
			EventCDPlayerMessage* createPlayerMessage = (EventCDPlayerMessage*)server.CreateMessage(i, (int)GameMessageType::EVENT_CD_PLAYER_MESSAGE);
			createPlayerMessage->action = 1;
			createPlayerMessage->clientIndex = clientIndex;
			std::cout << "server sends player creation to client " << i << " for client " << clientIndex << std::endl;
			server.SendMessage(i, (int)GameChannel::RELIABLE, createPlayerMessage);
		}
	}
	//add player to level 
	level.newPlayer(EntityModel(clientIndex));
	//set his position if the maze is generated
	if (maze.isGenerated()) {
		level.getPlayers()[clientIndex].setX(0);
		level.getPlayers()[clientIndex].setY(1);

	}

}

void GameServer::clientDisconnection(int clientIndex)
{
	level.removePlayer(clientIndex);
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (server.IsClientConnected(i)) {
			EventCDPlayerMessage * message = (EventCDPlayerMessage*)server.CreateMessage(i, (int)GameMessageType::EVENT_CD_PLAYER_MESSAGE);
			message->action = 0;
			message->clientIndex = clientIndex;
			server.SendMessage(i, (int)GameChannel::RELIABLE, message);
		}
	}
}
