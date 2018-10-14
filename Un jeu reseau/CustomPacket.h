#pragma once
#include <SFML/Network.hpp>
struct CustomPacket
{
public :
	sf::IpAddress address;
	unsigned short port;
	sf::Packet packet;
	
};

