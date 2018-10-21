#pragma once
#include <SFML/Network.hpp>
#include <list>
#include "DeliveryPacketManager.h"
class RemoteProxy
{
public:
	RemoteProxy();
	void resetProxy(sf::IpAddress address, unsigned short port);
	~RemoteProxy();

public:
	sf::Uint8 id;
	sf::IpAddress address;
	unsigned short port;
	sf::Uint16 connectionSequenceNumber; //connection sequence number established by the server
	DeliveryPacketManager deliveryPacketManager;


};

