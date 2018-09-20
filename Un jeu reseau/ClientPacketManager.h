#pragma once
#include "Common.h"
#include "ClientSocketManager.h"
#include "IPacketManager.h"
#include "IClientCallback.h"
#include "MoveList.h"
class ClientPacketManager : public IPacketManager
{
public:
	ClientPacketManager(GameDataRef data,IClientCallback* client,std::string address, unsigned short port);
	~ClientPacketManager();
	bool connect();
	void disconnect();
	virtual void packetReceived(CustomPacket & packet) override;
	void sendEntityModel(EntityModel& em);
	void sendMoveList(MoveList& moveList,sf::Uint16 id);
	void receivePackets(sf::Uint16 id);
	void sendWaitingPackets();
	void startGame(sf::Uint16 id);
	void sendName(sf::Uint16 id, sf::String name);
	void sendPing(sf::Uint16 id);


private: 
	GameDataRef _data;
	ClientSocketManager socketManager;
	IClientCallback* client;



	// Hérité via IPacketManager
	virtual sf::Uint16 welcomeReceived(CustomPacket & packet) override;

	virtual std::vector<EntityModel> entityPosReceived(CustomPacket & packet) override;

	virtual int newGameReceived(CustomPacket & packet) override;

	virtual void startGameReceived(CustomPacket & packet) override;

	virtual MazeConfig generateLevelReceived(CustomPacket & packet) override;

	virtual int eventReceived(CustomPacket & packet) override;

	virtual sf::Uint16 levelCompletedReceived(CustomPacket & packet) override;

	virtual int levelGeneratedReceived(CustomPacket & packet) override;

	virtual int disconnectReceived(CustomPacket & packet) override;

	virtual void lostConnection() override;


	// Hérité via IPacketManager
	virtual std::pair<sf::Uint16, sf::String> nameReceived(CustomPacket & packet) override;


	// Hérité via IPacketManager
	virtual sf::Uint16 pingReceived(CustomPacket & packet) override;

};

