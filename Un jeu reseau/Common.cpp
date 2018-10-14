#include "Common.h"


sf::Packet & operator<<(sf::Packet & packet, const Common::PacketHeader & ph)
{
	return packet << ph.protocolId << ph.packetType;
}

sf::Packet & operator>>(sf::Packet & packet, Common::PacketHeader & ph)
{
	sf::Uint32 protocolId;
	sf::Uint8 packetType;

	packet >> protocolId >> packetType ;

	ph.protocolId = protocolId;
	ph.packetType = packetType;

	return packet;
}

sf::Packet & operator<<(sf::Packet & packet, const Common::PacketReliablilityLayer & prl)
{
	return packet << prl.connectionSequenceNumber << prl.clientId << prl.packetSequenceNumber << prl.lastAck << prl.ackBits;
}

sf::Packet & operator>>(sf::Packet & packet, Common::PacketReliablilityLayer & prl)
{
	sf::Uint16 connectionSequenceNumber;
	sf::Uint8 clientId;
	sf::Uint32 packetSequenceNumber;
	sf::Uint32 lastAck;
	sf::Uint32 ackBits;

	packet >> connectionSequenceNumber >> clientId >> packetSequenceNumber >> lastAck >> ackBits;

	prl.connectionSequenceNumber = connectionSequenceNumber;
	prl.clientId = clientId;
	prl.lastAck = lastAck;
	prl.ackBits = ackBits;

	return packet;
}


sf::Packet & operator<<(sf::Packet & packet, const Common::PacketConnection & crp)
{
	return packet << crp.header;
}

sf::Packet & operator>>(sf::Packet & packet, Common::PacketConnection& crp)
{
	Common::PacketHeader header;
	packet >> header;
	crp.header = header;
	return packet;
}

sf::Packet& operator << (sf::Packet& packet, const Common::PacketDisconnection& pd) {
	return packet << pd.header << pd.connectionSequenceNumber << pd.clientId;
}
sf::Packet& operator >> (sf::Packet& packet, Common::PacketDisconnection& pd) {

	Common::PacketHeader header;
	sf::Uint16 connectionSequenceNumber;
	sf::Uint8 clientId;

	packet >> header >> connectionSequenceNumber >> clientId;

	pd.header = header;
	pd.connectionSequenceNumber = connectionSequenceNumber;
	pd.clientId = clientId;

	return packet;
}

sf::Packet & operator<<(sf::Packet & packet, const Common::PacketGameData & pgd)
{
	return packet << pgd.header << pgd.reliability<< pgd.level;
}

sf::Packet & operator>>(sf::Packet & packet, Common::PacketGameData & pgd)
{
	Common::PacketHeader header;
	Common::PacketReliablilityLayer reliability;
	Level level;

	packet >> header >> reliability >> level;

	pgd.header = header;
	pgd.reliability = reliability;
	pgd.level = level;

	return packet;
}

sf::Packet & operator<<(sf::Packet & packet, const Common::PacketEvent & pe)
{
	packet << pe.header << pe.reliability;

	packet << pe.events.size();
	for (int i = 0; i < pe.events.size(); i++) {
		packet << pe.events[i];
	}
	return packet;
}

sf::Packet & operator>>(sf::Packet & packet, Common::PacketEvent & pe)
{
	Common::PacketHeader header;
	Common::PacketReliablilityLayer reliability;
	std::vector<Common::GameEventType> events;
	sf::Uint8 size;

	packet >> header >> reliability >> size;
	
	sf::Uint8 event;
	for (int i = 0; i < size; i++) {
		packet >> event; 
		events.push_back((Common::GameEventType)event);
	}
	
	pe.header = header;
	pe.reliability = reliability;
	pe.events = events;

	return packet;
}

