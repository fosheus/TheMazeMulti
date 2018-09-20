#include "ClientSocketManager.h"


ClientSocketManager::ClientSocketManager(GameDataRef data, IPacketManager * packetManager, std::string address, unsigned short port):_data(data)
{
	this->packetManager = packetManager;
	this->address = address;
	this->port = port;
	timeoutCount = 0;
}

bool ClientSocketManager::connect()
{
	if (this->tcpSocket.connect(address,port) != sf::Socket::Done) {
		return false;
	}
	tcpSocket.setBlocking(false);
	udpSocket.setBlocking(false);
	//udpSocket.bind(this->port, sf::IpAddress::LocalHost);
	return true;
}


void ClientSocketManager::sendPacket(CustomPacket & packet)
{
	dataToSend.push(packet);
}


void ClientSocketManager::close()
{
	closeAll();
}

void ClientSocketManager::receiveReliablePacket()
{
	CustomPacket packet;
	sf::Socket::Status status;

	if (tcpSocket.getLocalPort() != 0) {

		status = tcpSocket.receive(packet);
		if (status == sf::Socket::Done) {
			//ok
			packetManager->packetReceived(packet);
		}
		else if (status == sf::Socket::NotReady) {
		}
		else if (status == sf::Socket::Error) {
			std::cout << "Error while receiving tcp data" << std::endl;
		}
		else if (status == sf::Socket::Disconnected) {
			std::cout << "Disconnected from remote system" << std::endl;
			packetManager->lostConnection();
		}
	}

}

void ClientSocketManager::receivePacket()
{
	CustomPacket packet;
	sf::Socket::Status status;
	sf::IpAddress address;
	unsigned short port;
	status = udpSocket.receive(packet,address, port);
	if (status == sf::Socket::Done) {
		packetManager->packetReceived(packet);
	}
	else if (status == sf::Socket::NotReady) {

	}
	else if (status == sf::Socket::Error) {
		std::cout << "Error while receiving udp data" << std::endl;
	}
}


			
void ClientSocketManager::sendUdpPacket(CustomPacket & packet)
{
	udpSocket.send(packet, address, port);
}

void ClientSocketManager::sendTcpPacket(CustomPacket & packet)
{
	sf::Socket::Status status = tcpSocket.send(packet);
	if (status == sf::Socket::Done) {
		//ok
	}
	else if (status == sf::Socket::NotReady) {
		//std::cout << "Stream not ready" << std::endl;
		dataToSend.push(packet);
		std::cout << "client stream Not ready " << std::endl;

	}
	else  if (status == sf::Socket::Partial) {
		dataToSend.push(packet);
		std::cout << "partially sent packet Client " << std::endl;
	}
	else if (status == sf::Socket::Error) {
		std::cout << "Error while sending data" << std::endl;
	}
	else if (status == sf::Socket::Disconnected) {
		std::cout << "Disconnected from remote system" << std::endl;
		packetManager->lostConnection();
	}
}

void ClientSocketManager::sendWaitingPackets()
{
	CustomPacket packet;
	sf::Socket::Status status;
	if (tcpSocket.getLocalPort() != 0) {

		int nb = dataToSend.size();
		int i = 0;
		while (i < nb) {
			packet = dataToSend.front();
			status = udpSocket.send(packet,address,port);
			i++;
			dataToSend.pop();
		}
	}
}


ClientSocketManager::~ClientSocketManager()
{
}


void ClientSocketManager::closeAll()
{
	this->tcpSocket.disconnect();
}



