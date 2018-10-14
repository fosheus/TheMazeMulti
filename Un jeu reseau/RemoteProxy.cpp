#include "RemoteProxy.h"



RemoteProxy::RemoteProxy()
{

}

void RemoteProxy::resetProxy(sf::IpAddress address, unsigned short port)
{
	this->address = address;
	this->port = port;
	deliveryPacketManager = DeliveryPacketManager();
}


RemoteProxy::~RemoteProxy()
{
}
