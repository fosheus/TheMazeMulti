#pragma once
#include <SFML\Graphics.hpp>
class MazeConfig
{
public:
	MazeConfig(){}
	MazeConfig(bool status,int seed, int width, int heigh);
	~MazeConfig();

	int seed;
	int width;
	int height;
	bool status;
};


