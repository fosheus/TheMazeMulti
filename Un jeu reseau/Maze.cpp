#include "Maze.h"
#include <iostream>


Maze::Maze()
{
}

const std::vector<std::vector<int>>& Maze::generateMaze(unsigned int seed,int width, int height)
{
	int randCol = 0, randLine = 0;
	int i, j;
	int value = 1;
	int wallToOpen = 0;
	int iter = 0;
	int randDir;


	if (width % 2 == 0 || height % 2 == 0 || width<=0 || height <=0)
	{
		return std::vector<std::vector<int>>();
	}
	this->width = width;
	this->height = height;

	for (i = 0; i < width; i++) {
		maze.push_back(std::vector<int>());
		for (j = 0; j < height; j++) {
			maze[i].push_back(0);
			if (i % 2 == 1 && j % 2 == 1) {
				maze[i][j] = value++;
			}

		}
	}

	srand(seed);
	wallToOpen = ((width / 2)*(height / 2)) - 1;
	while (iter < wallToOpen) {
		while ((randCol = (int)(rand() / (double)RAND_MAX * (width - 1))) % 2 != 1);
		while ((randLine = (int)(rand() / (double)RAND_MAX * (height - 1))) % 2 != 1);
		randDir = (int)(rand() % 4);

		switch (randDir)
		{
		case 0:
			if (randCol + 1 == width - 1) {
				break;
			}
			if (updateMaze(randCol, randLine, randCol +2 , randLine, randCol+ 1, randLine)) {
				iter++;
			}
			break;
		
		case 1:
			if (randLine - 1 == 0) {
				break;
			}
			if (updateMaze(randCol, randLine, randCol, randLine - 2, randCol, randLine - 1)) {
				iter++;
			}
			break;
		case 2:

			if (randCol - 1 == 0) {
				break;
			}
			if (updateMaze(randCol, randLine, randCol -2, randLine, randCol - 1, randLine)) {
				iter++;
			}
			break;
			
		case 3:
			if (randLine + 1 == height - 1)
				break;
			if (updateMaze(randCol, randLine, randCol, randLine+2, randCol, randLine + 1)) {
				iter++;
			}
			break;
		}
	}
	for (j = 0; j < height - 1; j++) {
		for (i = 0; i < width - 1; i++) {
			if (maze[i][j] > 0)
				maze[i][j] = 1;
			else
				maze[i][j] = 0;
		}
	}
	maze[0][1] = 1;
	maze[width - 1][height- 2] = 1;

	generated = true;

	return maze;

}
const std::vector<std::vector<int>>& Maze::getMaze()
{
	return maze;
}

sf::Vector2f Maze::getExitPos()
{
	return sf::Vector2f(width - 1, height - 2);
}

bool Maze::updateMaze(int x, int y, int overWallCellX, int overWallCellY,int wallX,int WallY) {

	int valueTmp;
	if (maze[x][y] == maze[overWallCellX][overWallCellY]) {
		return false;
	}
	valueTmp = maze[overWallCellX][overWallCellY];
	maze[wallX][WallY] = maze[x][y];
	for (int i = 0; i < width - 1; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (maze[i][j] == valueTmp)
				maze[i][j] = maze[x][y];
		}
	}
	return true;
	
}

void Maze::clearMaze() {
	this->generated = false;
	maze.clear();
}

void Maze::optimizeMazeForRendering() {
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			bool top = false, right = false, left = false, bot = false;

			if (maze[i][j] == 0) {
				if (i > 0) {
					if (maze[i - 1][j] <= 0) {
						left = true;
					}
				} if (j > 0) {
					if (maze[i][j - 1] <= 0) {
						top = true;
					}
				} if (j < height - 1) {
					if (maze[i][j + 1] <= 0) {
						bot = true;
					}
				} if (i < width - 1) {
					if (maze[i + 1][j] <= 0) {
						right = true;
					}
				}
			}
			if (top && left && bot && right) {
				maze[i][j] = ALL;
			}
			else if (top && left && bot) {
				maze[i][j] = TOP_LEFT_BOT;

			}
			else if (left && bot && right) {
				maze[i][j] = LEFT_BOT_RIGHT;
			}
			else if (bot && right && top) {
				maze[i][j] = BOT_RIGHT_TOP;
			}
			else if (right && top && left) {
				maze[i][j] = RIGHT_TOP_LEFT;
			}
			else if (top && left) {
				maze[i][j] = TOP_LEFT;
			}
			else if (left && bot) {
				maze[i][j] = LEFT_BOT;
			}
			else if (bot && right) {
				maze[i][j] = BOT_RIGHT;
			}
			else if (right && top) {
				maze[i][j] = RIGHT_TOP;
			}
			else if (top && bot) {
				maze[i][j] = TOP_BOT;
			}
			else if (right && left) {
				maze[i][j] = LEFT_RIGHT;
			}
			else if (top) {
				maze[i][j] = TOP;
			}
			else if (left) {
				maze[i][j] = LEFT;
			}
			else if (bot) {
				maze[i][j] = BOT;
			}
			else if (right) {
				maze[i][j] = RIGHT;
			}
		}
	}
}


int Maze::getWidth() const
{
	return width;
}

int Maze::getHeight() const
{
	return height;
}

Maze::~Maze()
{
}
