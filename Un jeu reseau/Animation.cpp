#include "Animation.h"


Animation::Animation(const sf::Texture * texture, sf::Vector2u imageCount, float switchCount)
{
	this->imageCount = imageCount;
	this->switchTime = switchCount;
	this->totalTime = 0.0f;
	this->currentImage.x = 0;

	uvRect.width = texture->getSize().x / float(imageCount.x);
	uvRect.height = texture->getSize().y / float(imageCount.y);

}


void Animation::update(int row, float multiplier) {
	currentImage.y = row;
	totalTime += multiplier;
	if (totalTime >= switchTime) {
		totalTime -= switchTime;
		currentImage.x++;
		if (currentImage.x >= imageCount.x) {
			currentImage.x = 0;
		}
	}

	uvRect.left = currentImage.x * uvRect.width;
	uvRect.top = currentImage.y * uvRect.height;
}

Animation::~Animation()
{
}
