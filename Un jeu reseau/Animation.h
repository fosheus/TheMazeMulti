#pragma once
#include <SFML/Graphics.hpp>

class Animation
{
public:
	Animation() {};
	Animation(const sf::Texture* texture, sf::Vector2u imageCount, float switchCount);
	void update(int row, float multiplier);
	~Animation();

public :
	sf::IntRect uvRect;

private :
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;

	float totalTime;
	float switchTime;





};

