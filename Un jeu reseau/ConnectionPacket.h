#pragma once
#include <SFML/Network.hpp>
class ConnectionPacket
{
public:
	ConnectionPacket();
	~ConnectionPacket();
	
	sf::Uint32 protocolId;
	sf::Uint8 packetType;

};

