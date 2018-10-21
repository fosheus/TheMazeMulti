#pragma once
#include <SFML\Graphics.hpp>
#include "EntityModel.h"

class Entity : public sf::Drawable
{
public:
	Entity(sf::Uint16 id,float x, float y);
	Entity(sf::Uint16 id, float x, float y,sf::String name);
	void setId(sf::Uint16 id);
	sf::Uint16 getId();
	sf::Vector2f getRemotePosition();
	sf::Vector2f getLocalPosition();
	sf::String getName();
	sf::Uint16 getScore();
	sf::CircleShape& getSprite();
	void setName(sf::String name);
	void moveLocal(float deltaX,float deltaY,float multiplier);
	void setRadius(float size);
	void interpolation(float dt);
	void updateRemoteFromModel(const EntityModel &em,float scale);
	EntityModel toModel(float scale);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	~Entity();
private :
	const float threshold = 2.0f;
	const float interpolation_constant = 0.2f;

	sf::Vector2f remotePosition;
	sf::Uint16 id;
	sf::CircleShape sprite;
	sf::String name;
	sf::Uint16 score;
	

};

