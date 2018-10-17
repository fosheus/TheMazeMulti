#pragma once
#include "Maze.h"
#include <yojimbo.h>
#include "shared.h"
#include "EntityModel.h"

class GameServer 
{
public:
	GameServer(GameDataRef data,int port );
	~GameServer();

	void startServer();
	void stopServer();
	void update(float dt);

	void clientConnected(int clientIndex);
	void clientDisconnected(int clientIndex);

private:
	void run();
	void processMessages();
	void processMessage(int clientIndex, yojimbo::Message* message);
	void processMoveMessage(int clientIndex, MoveMessage* message);


	bool collisionManagement(EntityModel* e);
	void managePlayerWin(EntityModel * e);

	void broadcastPlayerWon(int clientIndex);

	
private :
	GameDataRef _data;
	sf::Thread threadServer;

	bool started;

	yojimbo::Server server;
	yojimbo::Address endpoint;
	yojimbo::GameConnectionConfig connectionConfig;
	GameAdapter adapter;

	sf::Clock clock;

	Maze maze;
	int level;
	EntityModel players[MAX_PLAYERS];



	
};

