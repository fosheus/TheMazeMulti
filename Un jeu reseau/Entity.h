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
	float getX();
	float getY();
	sf::String getName();
	sf::Uint16 getScore();
	sf::CircleShape& getSprite();
	void setX(float x);
	void setY(float y);
	void setName(sf::String name);
	void setScore(sf::Uint16 score);
	void move(float deltaX,float deltaY,float multiplier);
	void setLocalPosition(float x, float y);
	void setRadius(float size);
	void interpolation(float dt);

	void updateFromModel(const EntityModel& entityModel,float scale);
	EntityModel toModel(float scale);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	~Entity();
private :
	float x;
	float y;
	sf::Uint16 id;
	sf::CircleShape sprite;
	sf::String name;
	sf::Uint16 score;
	

};

