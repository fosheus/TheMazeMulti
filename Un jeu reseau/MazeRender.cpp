#include "MazeRender.h"



MazeRender::MazeRender(GameDataRef data) : _data(data)
{
}

void MazeRender::load(Maze & maze, sf::Vector2u tileSize,float scale)
{
	m_vertices.clear();
	m_texture = _data->assets.GetImage("images/tileset.png");
	m_vertices.setPrimitiveType(sf::Quads);
	m_vertices.resize(maze.getWidth() * maze.getHeight() * 4);


	for (size_t i = 0; i < maze.getWidth(); i++)
	{
		for (size_t j = 0; j < maze.getHeight(); j++)
		{
			if (maze.getMaze()[i][j] <= 0) {

				int tileNumber = maze.getMaze()[i][j];
				int tu = abs(tileNumber) % (m_texture.getSize().x / tileSize.x);
				int tv = abs(tileNumber) / (m_texture.getSize().x / tileSize.x);

				sf::Vertex* quad = &m_vertices[(i + j * maze.getWidth()) * 4];

				quad[0].position = sf::Vector2f(i * tileSize.x*scale, j * tileSize.y*scale);
				quad[1].position = sf::Vector2f((i + 1) * tileSize.x *scale, j * tileSize.y *scale);
				quad[2].position = sf::Vector2f((i + 1) * tileSize.x*scale, (j + 1) * tileSize.y*scale);
				quad[3].position = sf::Vector2f(i * tileSize.x*scale, (j + 1) * tileSize.y*scale);

				// define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
			}
		}
	}
}


void MazeRender::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	
	states.texture = &m_texture;
	target.draw(m_vertices, states);

}


MazeRender::~MazeRender()
{
}
