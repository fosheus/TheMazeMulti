#include "Entity.h"



Entity::Entity(sf::Uint16 id,float x, float y)
{
	this->id = id;
	remotePosition = sf::Vector2f(0, 0);
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

sf::Vector2f Entity::getRemotePosition() {
	return remotePosition;
}

sf::Vector2f Entity::getLocalPosition() {
	return sprite.getPosition();
}

sf::CircleShape & Entity::getSprite()
{
	return sprite;
}


void Entity::setName(sf::String name)
{
	this->name = name;
}

sf::String Entity::getName()
{
	return this->name;
}

sf::Uint16 Entity::getScore()
{
	return score;
}

void Entity::setScore(int score)
{
	this->score = score;
}

void Entity::moveLocal(float deltaX, float deltaY,float multiplier)
{
	sprite.setPosition(sprite.getPosition().x+deltaX*multiplier, sprite.getPosition().y + deltaY* multiplier);
}

void Entity::setRadius(float size)
{
	sprite.setRadius(size);
	sprite.setOrigin(size, size);
}

void Entity::interpolation(float dt)
{
	
	float differenceX = getRemotePosition().x -	getLocalPosition().x;
	float differenceY = getRemotePosition().y - getLocalPosition().y;
	float localX = sprite.getPosition().x;
	float localY = sprite.getPosition().y;
	if (std::abs(differenceX) < threshold) {
		localX = getRemotePosition().x;
	}
	else {
		localX += differenceX * interpolation_constant;
	}
	if (std::abs(differenceY) < threshold) {
		localY = getRemotePosition().y;
	} else {
		localY += differenceY * interpolation_constant;

	}
	sprite.setPosition(localX, localY);
}

void Entity::updateRemoteFromModel(const EntityModel &em, float scale)
{
	if (this->id == em.getId()) {
		remotePosition = sf::Vector2f(em.getX()*scale, em.getY()*scale);
		this->score = em.getScore();
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
