#include "MazeConfig.h"




MazeConfig::MazeConfig(sf::Uint32 seed, sf::Uint16 width, sf::Uint16 height) : seed(seed), width(width), height(height)
{
}

sf::Uint32 MazeConfig::getSeed() const
{
	return seed;
}

sf::Uint16 MazeConfig::getWidth() const
{
	return width;
}

sf::Uint16 MazeConfig::getHeight() const
{
	return height;
}

void MazeConfig::setSeed(sf::Uint32 seed)
{
	this->seed = seed;
}

void MazeConfig::setWidth(sf::Uint16 width)
{
	this->width = width;
}

void MazeConfig::setHeight(sf::Uint16 height)
{
	this->height = height;
}

MazeConfig::~MazeConfig()
{
}


sf::Packet& operator << (sf::Packet& packet, const MazeConfig& mc) {
	return packet << mc.getSeed() << mc.getWidth() << mc.getHeight();
}

sf::Packet& operator >> (sf::Packet& packet, MazeConfig& mc) {
	sf::Uint32 seed;
	sf::Uint16 width;
	sf::Uint16 height;

	packet >> seed >> width >> height;

	mc.setSeed(seed);
	mc.setWidth(width);
	mc.setHeight(height);
	return packet;
}