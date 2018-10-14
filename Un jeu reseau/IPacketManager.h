#pragma once
#include <SFML/Network.hpp>
#include "EntityModel.h"
#include "MazeConfig.h"
#include <vector>
#include "CustomPacket.h"
#include "RemoteProxy.h"

class IPacketManager
{
public:
	virtual void packetReceived(CustomPacket & packet,RemoteProxy* remote) = 0;
	virtual void lostConnection() = 0;

private :
	virtual sf::Uint16 welcomeReceived(CustomPacket & packet) = 0;
	virtual std::vector<EntityModel> entityPosReceived(CustomPacket & packet) = 0;
	virtual int newGameReceived(CustomPacket & packet) = 0;
	virtual void startGameReceived(CustomPacket & packet) = 0;
	virtual MazeConfig generateLevelReceived(CustomPacket & packet) = 0;
	//TODO
	virtual int eventReceived(CustomPacket & packet) = 0;
	virtual sf::Uint16 levelCompletedReceived(CustomPacket & packet) = 0;
	virtual int levelGeneratedReceived(CustomPacket & packet) = 0;
	virtual int disconnectReceived(CustomPacket & packet) = 0;
	virtual std::pair<sf::Uint16, sf::String> nameReceived(CustomPacket & packet) = 0;
	virtual sf::Uint16 pingReceived (CustomPacket & packet) = 0;
	//virtual sf::Vector2f entityDeltaReceived(CustomPacket & packet) = 0;
};
