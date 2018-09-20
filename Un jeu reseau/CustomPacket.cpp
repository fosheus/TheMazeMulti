#include "CustomPacket.h"



CustomPacket::CustomPacket()
{
}

CustomPacket::CustomPacket(sf::Uint16 id, bool sendAsDatagram)
{
	this->id = id;
	this->sendAsDatagram = sendAsDatagram;
}

sf::Uint16 CustomPacket::getId()
{
	return id;
}

bool CustomPacket::getSendAsDatagram()
{
	return sendAsDatagram;
}

void CustomPacket::setId(sf::Uint16 id)
{
	this->id = id;
}

void CustomPacket::setSetAsDatagram(bool sendAsDatagram)
{
	this->sendAsDatagram = sendAsDatagram;
}


CustomPacket::~CustomPacket()
{
}
