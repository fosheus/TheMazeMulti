#include "DeliveryPacketManager.h"



DeliveryPacketManager::DeliveryPacketManager()
{
}

void DeliveryPacketManager::processIncomingPacketSeqNumber(sf::Uint32 inPacketSeqNumber)
{
	if (inPacketSeqNumber == nextExpectedSequenceNumber) {
		//ack ok, ack it
		lastpacketSeqNumber.push_back(inPacketSeqNumber);
		nextExpectedSequenceNumber++;
	}
	else if (inPacketSeqNumber < nextExpectedSequenceNumber) {
		//too late, we don't ack this packet, it will be resent
	}
	else if (inPacketSeqNumber > nextExpectedSequenceNumber) {
		//we ack this packet knowing that some packets may be lost
		lastpacketSeqNumber.push_back(inPacketSeqNumber);
		nextExpectedSequenceNumber++;
	}
}

void DeliveryPacketManager::processIncomingAcks(sf::Uint32 lastAck, sf::Uint32 ackBits)
{

}


DeliveryPacketManager::~DeliveryPacketManager()
{
}
