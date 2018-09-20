#pragma once
#include <SFML/Network.hpp>
class CustomPacket : public sf::Packet
{
public:
	CustomPacket();
	CustomPacket(sf::Uint16 id,bool sendAsDatagram);
	sf::Uint16 getId();
	bool getSendAsDatagram();
	void setId(sf::Uint16 id);
	void setSetAsDatagram(bool sendAsDatagram);
	~CustomPacket();

private :
	sf::Uint16 id;
	bool sendAsDatagram;

};

