#include "ServerSocketManager.h"
#include <iostream>

ServerSocketManager::ServerSocketManager(GameDataRef data, IServerPacketManager * packetManager, int port) : _data(data)
{
	this->port = port;
	this->packetManager = packetManager;
	listener.setBlocking(false);
	udpListener.setBlocking(false);
}


ServerSocketManager::~ServerSocketManager()
{

}

void ServerSocketManager::startServer()
{
	started = true;
	listener.listen(this->port);
	udpListener.bind(this->port,sf::IpAddress::getLocalAddress());

}

void ServerSocketManager::stopServer()
{
	listener.close();
	udpListener.unbind();
	started = false;
}

void ServerSocketManager::sendPacket(CustomPacket & packet)
{
	this->packetsToSend.push(packet);
}

void ServerSocketManager::receivePackets()
{
	if (started) {
		std::map<sf::IpAddress, sf::Uint16>::iterator it;
		CustomPacket packet;
		sf::IpAddress address;
		unsigned short port;
		sf::Socket::Status status;
		status = udpListener.receive(packet, address, port);
		switch (status) 
		{
		case sf::Socket::Done :
			it = ipAddressIdMap.find(address);
			if (it != ipAddressIdMap.end()) {
				udpClients[it->second] = RemoteClient(it->second, address, port);
				packetManager->packetReceived(packet);
			}
			break;
		case sf::Socket::NotReady :
			break;
		case sf::Socket::Error :
			std::cout << "Error while receiving udp packet" << std::endl;
			break;
		default:
			break;
		}
		
	}
}

void ServerSocketManager::receiveReliablePackets()
{
	if (started) {
		std::map<sf::Uint16, sf::TcpSocket*>::iterator it;
		sf::TcpSocket * client = new sf::TcpSocket();
		if (listener.accept(*client) == sf::Socket::Done) {
			sf::Uint16 id;
			id = packetManager->handleNewClient();
			if (id > 0) {
				std::cout << "client accepted " + client->getRemoteAddress().toString() << std::endl;
				ipAddressIdMap[client->getRemoteAddress()] = id;
				tcpClients[id] = client;
				client->setBlocking(false);
			}
			else {
				delete client;
			}
		}
		else {
			delete client;
		}
		for (it = tcpClients.begin(); it != tcpClients.end(); ++it)
		{
			if (!handleTcpSocket(it->first, it->second)) {
				delete tcpClients[it->first];
				it = tcpClients.erase(it);

				break;
			}
		}
	}
}

void ServerSocketManager::sendUdpPacket(CustomPacket & packet)
{
	RemoteClient rc = udpClients[packet.getId()];
	udpListener.send(packet, rc.getAddress(), rc.getPort());
}

void ServerSocketManager::sendTcpPacket(CustomPacket & packet)
{
	sf::Socket::Status status;
	status = tcpClients[packet.getId()]->send(packet);
	if (status == sf::Socket::Done) {

	}
	else if (status == sf::Socket::NotReady || status == sf::Socket::Partial) {
		packetsToSend.push(packet);
	}
	else if (status == sf::Socket::Error) {
		std::cout << "Error while sending packet to client " + std::to_string(packet.getId()) << std::endl;
	}
	else if (status == sf::Socket::Disconnected) {
		std::cout << "Client lost connection " + std::to_string(packet.getId()) << std::endl;
		packetManager->clientLostConnection(packet.getId());
		delete tcpClients[packet.getId()];
	}
}

void ServerSocketManager::broadcastUdpPacket(CustomPacket & packet)
{
	std::unordered_map<sf::Uint16, RemoteClient>::iterator it;
	for (it = udpClients.begin(); it != udpClients.end(); it++) {
		udpListener.send(packet, it->second.getAddress(), it->second.getPort());
	}
}

void ServerSocketManager::broadcastTcpPacket(CustomPacket & packet)
{
	std::map<sf::Uint16, sf::TcpSocket*>::iterator it;
	sf::Socket::Status status;
	for (it = tcpClients.begin(); it != tcpClients.end(); it++)
	{
		status = it->second->send(packet);
		if (status == sf::Socket::Done) {
			//ok
		}
		else if (status == sf::Socket::NotReady || status == sf::Socket::Partial) {
			packetsToSend.push(packet);
		}
		else if (status == sf::Socket::Error) {
			std::cout << "Error while sending packet to client " + std::to_string(it->first) << std::endl;
		}
		else if (status == sf::Socket::Disconnected) {
			std::cout << "Client lost connection " + std::to_string(it->first) << std::endl;
			packetManager->clientLostConnection(it->first);
			delete it->second;
			it = tcpClients.erase(it);
			udpClients.erase(it->first);
		}
	}
}


void ServerSocketManager::sendWaitingPackets()
{
	std::unordered_map<sf::Uint16, RemoteClient>::iterator it;
	CustomPacket packet;
	if (started) {
		int nb = packetsToSend.size();
		int i = 0;
		while (i < nb) {
			packet = packetsToSend.front();
			if (packet.getId() == 0) {
				//broadcast
				if (packet.getSendAsDatagram()) {
					broadcastUdpPacket(packet);
				}
				else {
					broadcastTcpPacket(packet);
				}
			}
			else {
				//send to specific client
				if (packet.getSendAsDatagram()) {
					sendUdpPacket(packet);
				}
				else {
					sendTcpPacket(packet);
				}
				
			}
			packetsToSend.pop();
			i++;
		}
	}

}

bool ServerSocketManager::handleTcpSocket(sf::Uint16 id,sf::TcpSocket* socket)
{
	CustomPacket packet;
	sf::Socket::Status status;

	status = socket->receive(packet);
	if (status == sf::Socket::Done) {
		packetManager->packetReceived(packet);
	}
	else if (status == sf::Socket::NotReady) {
		//no data received
	}
	else if (status == sf::Socket::Error) {
		std::cout << "Error while receiving from client " + std::to_string(id) << std::endl;
	}
	else if (status == sf::Socket::Disconnected) {
		std::cout << "Disconnection from remote client " + std::to_string(id) << std::endl;
		packetManager->clientLostConnection(id);
		return false;
	}
	return true;
}