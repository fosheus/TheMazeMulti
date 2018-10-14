#include "ServerSocketManager.h"
#include <iostream>

ServerSocketManager::ServerSocketManager()
{
	udpListener.setBlocking(false);
}


ServerSocketManager::~ServerSocketManager()
{

}


void ServerSocketManager::bind(unsigned short port)
{
	udpListener.bind(port);
}

void ServerSocketManager::unbind()
{
	udpListener.unbind();
}

void ServerSocketManager::addToSendList(CustomPacket & packet)
{
	this->packetsToSend.push(packet);
}


void ServerSocketManager::receivePackets()
{

	CustomPacket packet;
	sf::Socket::Status status;
	bool hasPackets = true;
	while (hasPackets) {
		hasPackets = false;
		status = udpListener.receive(packet.packet, packet.address, packet.port);
		switch (status)
		{
		case sf::Socket::Done:
			receivedPackets.push(packet);
			hasPackets = true;
			break;
		case sf::Socket::NotReady:
			break;
		case sf::Socket::Error:
			std::cout << "Error while receiving udp packet" << std::endl;
			break;
		default:
			break;
		}
	}
}

std::queue<CustomPacket>& ServerSocketManager::getReceivedPackets()
{
	return receivedPackets;
}


void ServerSocketManager::sendWaitingPackets()
{
	CustomPacket packet;
	int nb = packetsToSend.size();
	int i = 0;
	while (i < nb) {
		packet = packetsToSend.front();
		udpListener.send(packet.packet, packet.address, packet.port);
		packetsToSend.pop();
		i++;
	}

}




