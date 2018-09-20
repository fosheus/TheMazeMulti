#pragma once
#include <SFML/Network.hpp>
class RemoteClient
{
public:
	RemoteClient(){}
	RemoteClient(sf::Uint16 id, sf::IpAddress address, unsigned short port);
	sf::Uint16 getId();
	sf::IpAddress getAddress();
	unsigned short getPort();

	~RemoteClient();

private :
	sf::Uint16 id;
	sf::IpAddress address;
	unsigned short port;

};

