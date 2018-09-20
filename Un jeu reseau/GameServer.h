#pragma once
#include "ServerPacketManager.h"
#include "IServerCallback.h"
#include "Maze.h"

class GameServer :public IServerCallback
{
public:
	GameServer(GameDataRef data,int port );
	~GameServer();

	void startServer();
	void stopServer();
	void update(float dt);

private:
	// Hérité via IServerCallback
	virtual EntityModel* newClientConnected() override;
	virtual bool entityModel(EntityModel & em) override;
	virtual bool removeClient(sf::Uint16 id) override;
	virtual bool newGame(sf::Uint16 id) override;
	virtual bool levelGenerated(sf::Uint16 id) override;
	virtual EntityModel* getEntityModelById(sf::Uint16 id) override;
	// Hérité via IServerCallback
	virtual sf::Uint16 getNextEntityId() override;
	virtual std::map<sf::Uint16, EntityModel*> getAllEntities() override;
	// Hérité via IServerCallback
	virtual void levelCompleted(sf::Uint16 id) override;
	// Hérité via IServerCallback
	virtual void changeClientName(std::pair<sf::Uint16, sf::String> name) override;


	// Hérité via IServerCallback
	virtual void updateFromMoveList(MoveList & moveList) override;


	void serverWork();
	bool collisionManagement(EntityModel* e);
	void managePlayerWin(EntityModel * e);
	
private :
	GameDataRef _data;
	sf::Thread threadServer;
	sf::Clock clock;
	ServerPacketManager packetManager;
	std::map<sf::Uint16, EntityModel*> entities;
	int indexNextClient;

	Maze maze;
	int level;
	bool started;


	
};

