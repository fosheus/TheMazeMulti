#pragma once
#include "Maze.h"
#include <yojimbo.h>
#include "shared.h"
#include "EntityModel.h"
#include "IServerConnection.h"
#include "Move.h"
#include <SFML/Network/IpAddress.hpp>
#include <time.h>
#include <stdlib.h>
#include "MazeConfig.h"


class GameServer :public IServerConnection
{
public:
	GameServer();
	~GameServer();

	void startServer();
	void stopServer();
	void update(float dt);

	virtual void clientConnection(int clientIndex) override;

	virtual void clientDisconnection(int clientIndex) override;

private:
	void run();
	void processMessages();
	void processMessage(int clientIndex, yojimbo::Message* message);
	void processMoveMessage(int clientIndex, MoveMessage* message);
	void processPlayerNameMessage(int clientIndex,PlayerNameMessage* message);
	void processGameEventMessage(int clientIndex,GameEventMessage* message);

	void processNewGame(int clientIndex);

	void sendLevel();


	bool collisionManagement(EntityModel* e);
	void managePlayerWin(EntityModel * e);

	void broadcastPlayerWon(int clientIndex);

	
private :
	sf::Thread threadServer;

	bool started;

	GameConnectionConfig m_connectionConfig;
	GameAdapter m_adapter;
	yojimbo::Server server;
	yojimbo::Address endpoint;
	float serverTime;
	
	MazeConfig mazeConfig;

	sf::Clock clock;
	Level level;
	Maze maze;
	int difficulty;



	


};

