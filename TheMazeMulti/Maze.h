#pragma once
#include <vector>
#include <SFML\Graphics.hpp>
class Maze
{
public :
	enum CellValue {
		ALL = 0,
		TOP_LEFT_BOT = -1,
		LEFT_BOT_RIGHT = -2,
		BOT_RIGHT_TOP = -3,
		RIGHT_TOP_LEFT = -4,
		TOP_LEFT = -5,
		LEFT_BOT = -6,
		BOT_RIGHT = -7,
		RIGHT_TOP = -8,
		TOP = -9,
		LEFT = -10,
		BOT = -11,
		RIGHT = -12,
		LEFT_RIGHT=-13,
		TOP_BOT=-14

	};
public:
	Maze();
	const std::vector<std::vector<int>>& generateMaze(unsigned int seed,int width, int height);
	const std::vector<std::vector<int>>& getMaze();
	sf::Vector2f getExitPos();
	void clearMaze();
	bool isGenerated() { return generated; }
	int getWidth() const;
	int getHeight()const;
	void optimizeMazeForRendering();
	~Maze();

private :
	bool updateMaze(int x, int y, int overWallCellX, int overWallCellY, int wallX, int WallY);
	

private :
	std::vector<std::vector<int>> maze;
	int width;
	int height;
	bool generated;
};

