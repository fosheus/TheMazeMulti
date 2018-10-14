#pragma once
#include "Common.h"
#include "IPacketManager.h"
#include "Game.h"
#include "IServerCallback.h"
#include "ServerSocketManager.h"
#include <iostream>

class ServerPacketManager 
{
private :
	static const int MAX_CONNECTION = 5;

public:
	ServerPacketManager(GameDataRef data,IServerCallback* server,unsigned short port);
	/*
	Calls socketManager receive method and get the list of all received packets
	*/
	void receiveData();
	/*
	Calls socketManager sendWaitingPacket method to send all waiting packets.
	*/
	void sendWaitingData();
	/*
	create a packet header and append application data to send it to the specified client
	*/
	void sendBodyPacketToClient(sf::Packet bodyPacket, int clientId);
	/*
	create a packet header and append application data to send it to all clients
	*/
	void broadcastBodyPacketToClients(sf::Packet bodyPacket);

	void broadcastEntityModel(EntityModel & em);
	void broadcastLevelState(std::map<sf::Uint16,EntityModel*> & entities);
	void broadcastLevelCompleted(sf::Uint16 id);
	void broadcastSeedAndDimensions(MazeConfig config);
	void broadcastName(sf::Uint16 id, sf::String name);

	~ServerPacketManager();


private :

	CustomPacket generatePacketHeaderFor(int clientId);
	/*
	* This method verify if the protocol id of the packet is correct and routes according to the packetType to
	* the right method
	*/
	void handlePacket(CustomPacket&packet);

	/*
	Iterates throw all clients and generate ack bits for each of them
	*/
	void updateClientsAcks();
	/*
	* according to the packetType, it calls the connectionRequestPacketType
	* this method verify that the client is a new client or not thanks to the socketProxy pointer
	* if it is nullptr, it's a new client and we need to find free slot and send him conenction accepted or denied
	* if it's not nullptr, it reference a conencted client and we just need to send him connection accepted
	*/
	void connectionRequestPacket(CustomPacket& packet);
	/*
	* this method iterates throw all the clients proxy and verify if the address and port correspond to the current client proxy
	* if yes, the client already exists and it returns a pointer to this client
	* if no, the client is not connected and it returns null pointer
	*/
	RemoteProxy* isPacketFromNewRemoteHost(const sf::IpAddress & address, const unsigned short port);

	/*
	* finds the first empty slot and return it, -1 otherwise
	*/
	int findFirstEmptySlot();
	/*
	* look if the server can welcome a new client and if it's allowed to connect
	* if it doesn't exist, it is added to the client list and a connection accepted packet is sent
	* connection denied packet otherwise
	*/
	void handleNewClient(const sf::IpAddress& address, const unsigned short port);

	void sendConnectionAcceptedTo(const sf::Uint8 clientId);
	void sendConnectionDeniedTo(const sf::IpAddress& address, const unsigned short port);

	/*
	* verify the integrity of data in the packet to make sure the right client has requested a disconnection
	* if everything is ok, we broadcast a disconnection packet to every clients and notify the gameServer
	*/
	void disconnectionRequestPacket(CustomPacket& packet);

	/*
	* verify the integrity of data in the packet to make sure the client has sent the packet himself
	* then calls the delivery packet manager to ack the received data
	*/
	void gameDataPacket(CustomPacket& packet);
	/*
	* verify the integrity of data in the packet to make sure the client has sent the packet himself
	* then calls the delivery packet manager to ack the received data
	*/
	void gameEventPacket(CustomPacket& packet);

	void broadcastClientDisconnected(sf::Uint8 clientId);

	
	
private :
	GameDataRef _data;
	IServerCallback* server;
	ServerSocketManager socketManager;

private :

	RemoteProxy clientsProxy[MAX_CONNECTION];
	bool clientsConnected[MAX_CONNECTION];
	int nbConnectedClients;
	sf::Uint16 nextConnectionSequenceNumber;
};

