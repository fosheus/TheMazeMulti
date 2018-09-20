#include "RemoteClient.h"


RemoteClient::RemoteClient(sf::Uint16 id, sf::IpAddress address, unsigned short port)
{
	this->id = id;
	this->address = address;
	this->port = port;
}

sf::Uint16 RemoteClient::getId()
{
	return id;
}

sf::IpAddress RemoteClient::getAddress()
{
	return address;
}

unsigned short RemoteClient::getPort()
{
	return port;
}

RemoteClient::~RemoteClient()
{
}
