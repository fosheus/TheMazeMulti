#pragma once
#include "Maze.h"
#include <yojimbo.h>
#include "shared.h"
#include "EntityModel.h"
#include "IServerConnection.h"

class GameServer :public IServerConnection
{
public:
	GameServer(GameDataRef data );
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

	void sendLevel();


	bool collisionManagement(EntityModel* e);
	void managePlayerWin(EntityModel * e);

	void broadcastPlayerWon(int clientIndex);

	
private :
	GameDataRef _data;
	sf::Thread threadServer;

	bool started;

	GameConnectionConfig m_connectionConfig;
	GameAdapter m_adapter;
	yojimbo::Server server;
	yojimbo::Address endpoint;
	

	sf::Clock clock;
	Level level;
	Maze maze;
	int levelNbr;



	


};

