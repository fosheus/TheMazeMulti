#include "MazeConfig.h"




MazeConfig::MazeConfig(int seed, int width, int height) : seed(seed), width(width), height(height)
{
}

int MazeConfig::getSeed() const
{
	return seed;
}

int MazeConfig::getWidth() const
{
	return width;
}

int MazeConfig::getHeight() const
{
	return height;
}

void MazeConfig::setSeed(int seed)
{
	this->seed = seed;
}

void MazeConfig::setWidth(int width)
{
	this->width = width;
}

void MazeConfig::setHeight(int height)
{
	this->height = height;
}

MazeConfig::~MazeConfig()
{
}

