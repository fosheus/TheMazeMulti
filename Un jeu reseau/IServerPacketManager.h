#pragma once
#include "IPacketManager.h"
#include "MoveList.h"
class IServerPacketManager :
	public IPacketManager
{
public:

	// Hérité via IPacketManager
	virtual void packetReceived(CustomPacket & packet,RemoteProxy * remote) override = 0;
	virtual int handleNewClient() = 0;
	virtual void clientLostConnection(sf::Uint16 id) = 0;
	virtual sf::Uint16 getNextClientId() = 0;

private :

	virtual sf::Uint16 welcomeReceived(CustomPacket & packet) override = 0;
	virtual std::vector<EntityModel> entityPosReceived(CustomPacket & packet) override = 0;
	virtual int newGameReceived(CustomPacket & packet) override = 0;
	virtual void startGameReceived(CustomPacket & packet) override = 0;
	virtual MazeConfig generateLevelReceived(CustomPacket & packet) override = 0;
	virtual int eventReceived(CustomPacket & packet) override = 0;
	virtual sf::Uint16 levelCompletedReceived(CustomPacket & packet) override = 0;
	virtual int levelGeneratedReceived(CustomPacket & packet) override = 0;
	virtual int disconnectReceived(CustomPacket & packet) override = 0;
	virtual std::pair<sf::Uint16, sf::String> nameReceived(CustomPacket & packet) override = 0;
	virtual void lostConnection() override = 0;
	virtual sf::Uint16 pingReceived(CustomPacket & packet) override = 0;
	virtual MoveList moveListReceived(CustomPacket & packet) = 0;


};

