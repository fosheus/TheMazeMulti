#include "SplashState.h"
#include <iostream>
#include <sstream>
#include "DEFINITIONS.h"
#include "MainMenuState.h"

SplashState::SplashState(GameDataRef data) :_data(data)
{
}

SplashState::~SplashState()
{
}

void SplashState::Init()
{
	this->_background.setTexture(this->_data->assets.GetImage(SPASH_SCREEN_BACKGROUND_FILEPATH));
	this->_background.setPosition(this->_data->window.getSize().x / 2 - this->_background.getGlobalBounds().width / 2, this->_data->window.getSize().y / 2 - this->_background.getGlobalBounds().height / 2);
}

void SplashState::HandleInput()
{
	sf::Event event;

	while (this->_data->window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			this->_data->window.close();
		}
	}
}

void SplashState::Update(float dt)
{
	if (this->_clock.getElapsedTime().asSeconds() > SPLASH_SCREEN_TIME_SHOW) {
		this->_data->machine.AddState(StateRef(new MainMenuState(this->_data)), true);
	}
}

void SplashState::Draw(float dt)
{
	this->_data->window.clear();

	this->_data->window.draw(_background);

	this->_data->window.display();

}
