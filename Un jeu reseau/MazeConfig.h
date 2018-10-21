#pragma once
#include <SFML\Graphics.hpp>
class MazeConfig
{
public:
	MazeConfig(){}
	MazeConfig(int seed, int width, int heigh);
	int getSeed() const;
	int getWidth() const;
	int getHeight()const;
	void setSeed(int seed);
	void setWidth(int width);
	void setHeight(int height);
	~MazeConfig();



private:
	int seed;
	int width;
	int height;
};


