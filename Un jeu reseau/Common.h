#pragma once
#include "Level.h"
#include <SFML/System.hpp>
class Common 
{
public:

	static const sf::Uint32 PROTOCOL_ID = 0xa2d69c12;

	enum GameDataType : sf::Uint8 {
		ENTITYPOS = 0x01, //client or server receives 1 or more entityModel
		MOVELIST = 0x02
	};
	enum PacketType : sf::Uint8 {
		CONNECTION_REQUEST = 0x51,
		CONNECTION_ACCEPTED = 0x52,
		CONNECTION_DENIED = 0x53,
		GAME_DATA = 0x54,
		GAME_EVENT = 0x55,
		DISCONNECTION_REQUEST = 0x56
	};

	enum GameEventType : sf::Uint8 {
		NEWGAME=0xA1,
		LEVELCOMPLETED = 0xA2, //server informs clients that player x won the round,clients go to lobby
		GENERATELEVEL = 0xA3, //server sends seed, clients go to lobby
		NAME = 0xA4,
		DISCONNECT = 0xA5,
	};


	struct PacketHeader
	{
		sf::Uint32 protocolId;
		sf::Uint8 packetType;
	};

	struct PacketReliablilityLayer {
		sf::Uint16 connectionSequenceNumber;
		sf::Uint8 clientId;
		sf::Uint32 packetSequenceNumber;
		sf::Uint32 lastAck;
		sf::Uint32 ackBits;
	};

	struct PacketConnection{
		PacketHeader header;
	};

	struct PacketDisconnection {
		PacketHeader header;
		sf::Uint16 connectionSequenceNumber;
		sf::Uint8 clientId;
	};


	struct PacketGameData {
		PacketHeader header;
		PacketReliablilityLayer reliability;
		Level level;
	};
	struct PacketEvent{
		PacketHeader header;
		PacketReliablilityLayer reliability;
		std::vector<GameEventType> events;
	};

};

sf::Packet& operator << (sf::Packet& packet, const Common::PacketHeader& ph);
sf::Packet& operator >> (sf::Packet& packet, Common::PacketHeader& ph);

sf::Packet& operator << (sf::Packet& packet, const Common::PacketReliablilityLayer& prl);
sf::Packet& operator >> (sf::Packet& packet, Common::PacketReliablilityLayer& prl);

sf::Packet& operator << (sf::Packet& packet, const Common::PacketConnection& pcr);
sf::Packet& operator >> (sf::Packet& packet, Common::PacketConnection& pcr);

sf::Packet& operator << (sf::Packet& packet, const Common::PacketDisconnection& pd);
sf::Packet& operator >> (sf::Packet& packet, Common::PacketDisconnection& pd);

sf::Packet& operator << (sf::Packet& packet, const Common::PacketGameData& pgd);
sf::Packet& operator >> (sf::Packet& packet, Common::PacketGameData& pgd);

sf::Packet& operator << (sf::Packet& packet, const Common::PacketEvent& pe);
sf::Packet& operator >> (sf::Packet& packet, Common::PacketEvent& pe);