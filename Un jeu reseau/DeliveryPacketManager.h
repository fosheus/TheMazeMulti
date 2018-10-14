#pragma once
#include <SFML/System.hpp>
#include <list>
class DeliveryPacketManager
{
public:
	DeliveryPacketManager();
	void processIncomingPacketSeqNumber(sf::Uint32 inPacketSeqNumber);
	void processIncomingAcks(sf::Uint32 lastAck, sf::Uint32 ackBits);
	void updateAcks();
	~DeliveryPacketManager();

private :
	sf::Uint32 nextOutgoingSequenceNumber;
	sf::Uint32 nextExpectedSequenceNumber;
	std::list<sf::Uint32> lastpacketSeqNumber; //32 last packet sequence number received
	sf::Uint32 ackBits; //ack of the last 32 packets
	sf::Int32 timeLastPacketReceived;
};

