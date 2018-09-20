#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network\Packet.hpp>
class MazeConfig
{
public:
	MazeConfig(){}
	MazeConfig(sf::Uint32 seed, sf::Uint16 width, sf::Uint16 heigh);
	sf::Uint32 getSeed() const;
	sf::Uint16 getWidth() const;
	sf::Uint16 getHeight()const;
	void setSeed(sf::Uint32 seed);
	void setWidth(sf::Uint16 width);
	void setHeight(sf::Uint16 height);
	~MazeConfig();



private:
	sf::Uint32 seed;
	sf::Uint16 width;
	sf::Uint16 height;
};




sf::Packet& operator << (sf::Packet& packet, const MazeConfig& mc);

sf::Packet& operator >> (sf::Packet& packet, MazeConfig& mc);
