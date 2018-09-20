#include "MainMenuState.h"
#include "JoiningState.h"
#include <iostream>

MainMenuState::MainMenuState(GameDataRef data) : _data(data)
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::Init()
{
	_background.setTexture(_data->assets.GetImage(MAIN_MENU_BACKGROUND_FILEPATH));
	_playButton.setTexture(_data->assets.GetImage(MAIN_MENU_PLAY_BUTTON));
	_exitButton.setTexture(_data->assets.GetImage(MAIN_MENU_EXIT_BUTTON));

	_playButton.setPosition(_data->window.getSize().x / 2 - _playButton.getGlobalBounds().width / 2, _data->window.getSize().y / 3 * 1 - _playButton.getGlobalBounds().height / 2);
	_exitButton.setPosition(_data->window.getSize().x / 2 - _exitButton.getGlobalBounds().width / 2, _data->window.getSize().y / 3 * 2 - _exitButton.getGlobalBounds().height / 2);

	this->_background.setPosition(this->_data->window.getSize().x / 2 - this->_background.getGlobalBounds().width / 2, this->_data->window.getSize().y / 2 - this->_background.getGlobalBounds().height / 2);
}

void MainMenuState::HandleInput()
{
	sf::Event event;

	while (this->_data->window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			this->_data->window.close();
		}

		if (this->_data->inputs.IsSpriteClicked(_playButton, sf::Mouse::Left, _data->window)) {
			this->_data->machine.AddState(StateRef(new JoiningState(this->_data)), true);
		}
		if (this->_data->inputs.IsSpriteClicked(_exitButton, sf::Mouse::Left, _data->window)) {
			this->_data->window.close();
		}
	}
}

void MainMenuState::Update(float dt)
{
}

void MainMenuState::Draw(float dt)
{
	_data->window.clear();
	_data->window.draw(_background);
	_data->window.draw(_playButton);
	_data->window.draw(_exitButton);

	_data->window.display();

}
