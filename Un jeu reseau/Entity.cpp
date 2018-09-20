#include "Entity.h"



Entity::Entity(sf::Uint16 id,float x, float y)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->score = 0;
	sprite.setPosition(x, y);
	sprite.setRadius(30);
	sprite.setOrigin(30,30);
	sprite.setFillColor(sf::Color::White);
}

Entity::Entity(sf::Uint16 id, float x, float y, sf::String name) : Entity(id,x,y)
{
	this->name = name;
}

void Entity::setId(sf::Uint16 id)
{
	this->id = id;
}

sf::Uint16 Entity::getId()
{
	return this->id;
}

float Entity::getX()
{
	return x;
}

float Entity::getY()
{
	return y;
}

sf::CircleShape & Entity::getSprite()
{
	return sprite;
}

void Entity::setX(float x)
{
	this->x = x;
}

void Entity::setY(float y)
{
	this->y = y;
}

void Entity::setName(sf::String name)
{
	this->name = name;
}

void Entity::setScore(sf::Uint16 score)
{
	this->score = score;
}

sf::String Entity::getName()
{
	return this->name;
}

sf::Uint16 Entity::getScore()
{
	return score;
}

void Entity::move(float deltaX, float deltaY,float multiplier)
{
	sprite.setPosition(sprite.getPosition().x+deltaX*multiplier, sprite.getPosition().y + deltaY* multiplier);
}

void Entity::setLocalPosition(float x, float y)
{
	this->x = x;
	this->y = y;
	this->sprite.setPosition(x, y);
}



void Entity::setRadius(float size)
{
	sprite.setRadius(size);
	sprite.setOrigin(size, size);
}

void Entity::interpolation(float dt)
{
	const float threshold = 3.0f;
	const float interpolation_constant = 0.1f;
	float differenceX = x - sprite.getPosition().x;
	float differenceY = y - sprite.getPosition().y;
	float remoteX = sprite.getPosition().x;
	float remoteY = sprite.getPosition().y;
	if (std::abs(differenceX) < threshold) {
		remoteX = x;
	}
	else {
		remoteX += differenceX * interpolation_constant;
	}
	if (std::abs(differenceY) < threshold) {
		remoteY = y;
	} else {
		remoteY += differenceY * interpolation_constant;

	}
	sprite.setPosition(remoteX, remoteY);
}




void Entity::updateFromModel(const EntityModel & entityModel, float scale)
{
	if (this->id == entityModel.getId()) {
		this->x=entityModel.getX()*scale;
		this->y=entityModel.getY()*scale;
		this->score = entityModel.getScore();
	}
}

EntityModel Entity::toModel(float scale)
{
	return EntityModel(this->id, this->sprite.getPosition().x/scale, this->sprite.getPosition().y/scale,this->score,0);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}



Entity::~Entity()
{
}
