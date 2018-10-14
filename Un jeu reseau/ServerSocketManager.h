#pragma once
#include <SFML\Network.hpp>
#include "CustomPacket.h"
#include <queue>

/**
* Class dedicated to receive and send udp packets
*/
class ServerSocketManager
{
public:
	ServerSocketManager();
	~ServerSocketManager();

	/*
	Starts the server
	*/
	void bind(unsigned short port);
	/*
	Stops the server
	*/
	void unbind();
	
	/**
	* Add packet to waiting list
	*/
	void addToSendList(CustomPacket & packet);
	/*
	Try receiving packets from remote hosts
	*/
	void receivePackets();
	/*
	Returns the received packets queue
	*/
	std::queue<CustomPacket>& getReceivedPackets();
	/*
	* Iterates throw all waiting packets and determinate if its a broadcast packet
	* or a packet destinated to one remote host
	*/
	void sendWaitingPackets();

private:
	sf::UdpSocket udpListener;
	std::queue<CustomPacket> packetsToSend;
	std::queue<CustomPacket> receivedPackets;


};

