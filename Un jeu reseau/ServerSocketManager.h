#pragma once
#include <SFML\Network.hpp>
#include "Game.h"
#include "Common.h"
#include "IServerPacketManager.h"
#include "RemoteClient.h"
#include <queue>
#include <unordered_map>

class ServerSocketManager
{
private:

	const int MAX_TIMEOUT = 20;
	enum ServerStatus {
		LOBBY,
		RUNNING,
		END
	};

public:
	ServerSocketManager(GameDataRef data, IServerPacketManager* packetManager, int port);
	~ServerSocketManager();

	void startServer();
	void stopServer();
	/**
	sends packet to indicated client, or broadcast if 0
	*/
	void sendPacket(CustomPacket & packet);
	void receivePackets();
	void receiveReliablePackets();
	void sendWaitingPackets();

	sf::Uint32 getPacketCounter();



private :

	bool handleTcpSocket(sf::Uint16 id, sf::TcpSocket* socket);
	void sendUdpPacket(CustomPacket & packet);
	void sendTcpPacket(CustomPacket & packet);
	void broadcastUdpPacket(CustomPacket & packet);
	void broadcastTcpPacket(CustomPacket & packet);

private:
	GameDataRef _data;

	unsigned short port;
	IServerPacketManager* packetManager;
	sf::TcpListener listener;
	sf::UdpSocket udpListener;
	std::queue<CustomPacket> packetsToSend;
	std::map<sf::Uint16, sf::TcpSocket*> tcpClients;
	std::unordered_map<sf::Uint16, RemoteClient> udpClients;
	std::map <sf::IpAddress, sf::Uint16> ipAddressIdMap;
	
	bool started;
	sf::Uint32 packetCounter;

};

