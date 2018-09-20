#pragma once
#include "Common.h"
#include "IPacketManager.h"
#include "Game.h"
#include "IServerCallback.h"
#include "ServerSocketManager.h"
#include <iostream>
class ServerPacketManager :
	public IServerPacketManager
{
public:
	ServerPacketManager(GameDataRef data,IServerCallback* server,unsigned short port);
	

	void startServer();
	void stopServer();

	void handleClients();
	void receiveData();
	void sendWaitingData();

	void broadcastEntityModel(EntityModel & em);
	void broadcastLevelState(std::map<sf::Uint16,EntityModel*> & entities);
	void broadcastLevelCompleted(sf::Uint16 id);
	void sendSeedDimension(MazeConfig config);
	void broadcastName(sf::Uint16 id, sf::String name);

	~ServerPacketManager();

private :

	// Hérité via IPacketManager
	virtual sf::Uint16 welcomeReceived(CustomPacket & packet) override;

	virtual std::vector<EntityModel> entityPosReceived(CustomPacket & packet) override;

	virtual int newGameReceived(CustomPacket & packet) override;

	virtual void startGameReceived(CustomPacket & packet) override;

	virtual int eventReceived(CustomPacket & packet) override;

	virtual int levelGeneratedReceived(CustomPacket & packet) override;

	virtual int disconnectReceived(CustomPacket & packet) override;


	// Hérité via IPacketManager
	virtual MazeConfig generateLevelReceived(CustomPacket & packet) override;

	virtual sf::Uint16 levelCompletedReceived(CustomPacket & packet) override;

	virtual void packetReceived(CustomPacket & packet) override;
	virtual int handleNewClient() override;

	// Hérité via IServerPacketManager
	virtual void clientLostConnection(sf::Uint16 id) override;
	
private :
	GameDataRef _data;
	IServerCallback* server;
	ServerSocketManager socketManager;




	// Hérité via IServerPacketManager
	virtual sf::Uint16 getNextClientId() override;


	// Hérité via IServerPacketManager
	virtual void lostConnection() override;


	// Hérité via IServerPacketManager
	virtual std::pair<sf::Uint16, sf::String> nameReceived(CustomPacket & packet) override;


	// Hérité via IServerPacketManager
	virtual sf::Uint16 pingReceived(CustomPacket & packet) override;


	// Hérité via IServerPacketManager
	virtual MoveList moveListReceived(CustomPacket & packet) override;

};

