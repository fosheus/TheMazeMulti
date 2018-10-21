#pragma once
#include <vector>
#include "Point.h"
#include <SFML\System.hpp>

class AStar
{

public :
	struct Node {
		Point coord;
		int hCost;
		int gCost;
		Node* parent;
		int getfCost() {
			return hCost + gCost;
		}
	};
private :
	
	Node* root;
	Point start;
	Point end;
	sf::Thread t;
	std::vector<Node> openList;
	std::vector<Node> closeList;
	std::vector<std::vector<int>> map;
	//int **map=NULL;
	int width;
	int height;
	std::vector<Point> path;
	bool pathFound;


	int getDistance(Point pointA, Point pointB);
	std::vector<AStar::Node> getNeighbours(Node node, int width, int height);
	std::vector<Point> retracePath(Node node);
	void pathFinding();
	

public :
	AStar();
	AStar(Point start, Point dest);
	void setStartPoint(Point start);
	Point getStartPoint();
	void setDestPoint(Point end);
	Point getDestPoint();
	std::vector<AStar::Node>* getOpenList() { return &openList; }
	std::vector<AStar::Node>* getCloseList() { return &closeList; }
	std::vector<Point> getPath() { return path; }
	bool isPathFound() { return pathFound; }

	//void find(int **map, int width, int height);
	void find(const std::vector<std::vector<int>> & map, int width, int height);
	
	~AStar();

	
};

