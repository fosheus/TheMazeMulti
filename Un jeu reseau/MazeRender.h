#pragma once
#include "Maze.h"
#include "Game.h"
#include <SFML\Graphics.hpp>
class MazeRender : public sf::Drawable
{
public:
	MazeRender(GameDataRef data);
	void load(Maze& maze, sf::Vector2u tileSize,float scale);
	~MazeRender();

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


private :
	GameDataRef _data;
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::VertexArray m_vertices;

};

