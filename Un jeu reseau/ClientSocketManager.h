#pragma once
#include <SFML\Network.hpp>
#include "IPacketManager.h"
#include "Game.h"
#include <queue>
#include <iostream>
class ClientSocketManager
{
public:
	ClientSocketManager(GameDataRef data,IPacketManager* packetManager,std::string address,unsigned short port);
	bool connect();
	void sendPacket(CustomPacket & packet);
	void close();
	void receiveReliablePacket();
	void receivePacket();
	void sendWaitingPackets();
	~ClientSocketManager();

private :

	void closeAll();
	void sendUdpPacket(CustomPacket & packet);
	void sendTcpPacket(CustomPacket & packet);

private :

	const int MAX_TIMEOUT = 20;
	int timeoutCount;
	GameDataRef _data;
	IPacketManager* packetManager;

	sf::IpAddress address;
	short port;
	sf::TcpSocket tcpSocket;
	sf::UdpSocket udpSocket;

	sf::Uint32 packetCounter;


	std::queue<CustomPacket> dataToSend;

};

