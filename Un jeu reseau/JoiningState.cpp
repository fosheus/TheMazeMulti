#include "JoiningState.h"
#include "DEFINITIONS.h"
#include <iostream>

JoiningState::JoiningState(GameDataRef data) 
{
	this->_data = data;
}

JoiningState::~JoiningState()
{
}

void JoiningState::Init()
{
	basicFont.loadFromFile(BASIC_FONT);

	_hostButton.setTexture(_data->assets.GetImage(JOIN_MENU_HOST_BUTTON));
	_joinButton.setTexture(_data->assets.GetImage(JOIN_MENU_JOIN_BUTTON));
	_addressTextArea.setTexture(_data->assets.GetImage(JOIN_MENU_TEXT_AREA));
	_nameTextArea.setTexture(_data->assets.GetImage(JOIN_MENU_TEXT_AREA));
	_background.setTexture(_data->assets.GetImage(JOIN_SCREEN_BACKGROUND_FILEPATH));

	lblAddress.setFont(basicFont);
	lblName.setFont(basicFont);
	lblName.setString("Pseudo");
	lblName.setFillColor(sf::Color::Red);

	lblAddress.setFont(basicFont);
	lblAddress.setString("Address");
	lblAddress.setFillColor(sf::Color::Red);


	lblName.setPosition(_data->window.getSize().x / 3 - 40, _data->window.getSize().y / 8 * 3 +10);
	_nameTextArea.setPosition(_data->window.getSize().x / 3 - _nameTextArea.getGlobalBounds().width / 2, _data->window.getSize().y / 2 - _nameTextArea.getGlobalBounds().height / 2);

	_hostButton.setPosition(_data->window.getSize().x / 3*2 - _hostButton.getGlobalBounds().width / 2, _data->window.getSize().y / 7*2 - _hostButton.getGlobalBounds().height / 2+50);
	lblAddress.setPosition(_data->window.getSize().x / 3*2-40, _data->window.getSize().y / 14 * 9 -20);
	_addressTextArea.setPosition(_data->window.getSize().x / 3*2 - _addressTextArea.getGlobalBounds().width / 2, _data->window.getSize().y / 7* 5 - _addressTextArea.getGlobalBounds().height / 2);
	_joinButton.setPosition(_data->window.getSize().x / 3*2 - _joinButton.getGlobalBounds().width / 2, _data->window.getSize().y / 7 * 6  - _joinButton.getGlobalBounds().height / 2);

	address.setFont(basicFont);
	address.setPosition(_addressTextArea.getPosition().x + 5, _addressTextArea.getPosition().y + 5);
	name.setFont(basicFont);
	name.setPosition(_nameTextArea.getPosition().x + 5, _nameTextArea.getPosition().y + 5);
	
	
}

void JoiningState::HandleInput()
{
	sf::Event event;

	while (this->_data->window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			this->_data->window.close();
		}
		if (textAddressHasFocus) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace) {
				if (addressStr.getSize() > 0) {
					addressStr.erase(addressStr.getSize() - 1, 1);
					
				}
			}else if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode < 58 && event.text.unicode > 47 || event.text.unicode == 46) {
					addressStr += event.text.unicode;
				}
			}

			address.setString(addressStr);
			
		}
		if (textNameHasFocus) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace) {
				if (nameStr.getSize() > 0) {
					nameStr.erase(nameStr.getSize() - 1, 1);

				}
			}
			else if (event.type == sf::Event::TextEntered) {
				if (event.text.unicode < 128 && event.text.unicode != 8) {
					nameStr += event.text.unicode;
				}
			}
			name.setString(nameStr);
		}


		if (this->_data->inputs.HasSpriteLostFocus(_addressTextArea, _data->window)) {
			textAddressHasFocus = false;
		}
		if (this->_data->inputs.IsSpriteClicked(_addressTextArea,sf::Mouse::Left, _data->window)) {
			textAddressHasFocus = true;
		}
		if (this->_data->inputs.HasSpriteLostFocus(_nameTextArea, _data->window)) {
			textNameHasFocus = false;
		}
		if (this->_data->inputs.IsSpriteClicked(_nameTextArea, sf::Mouse::Left, _data->window)) {
			textNameHasFocus = true;
		}
		if (this->_data->inputs.IsSpriteClicked(_joinButton, sf::Mouse::Left, _data->window)) {
			std::cout << address.getString().toAnsiString() << std::endl;
			if (addressStr.getSize() > 0 && nameStr.getSize() > 2) {
				_data->machine.AddState(StateRef(new GameState(_data, address.getString(), name.getString(), false)), true);
			}
		}
		if (this->_data->inputs.IsSpriteClicked(_hostButton, sf::Mouse::Left, _data->window)) {
			std::cout << address.getString().toAnsiString() << std::endl;
			if (nameStr.getSize() > 2) {
				_data->machine.AddState(StateRef(new GameState(_data, "", name.getString(), true)), true);
			}
		}
	}
	
}

void JoiningState::Update(float dt)
{
}

void JoiningState::Draw(float dt)
{
	_data->window.clear();
	this->_data->window.draw(_background);
	this->_data->window.draw(_hostButton);
	this->_data->window.draw(_joinButton);
	this->_data->window.draw(_addressTextArea);
	this->_data->window.draw(_nameTextArea);
	this->_data->window.draw(address);
	this->_data->window.draw(name);
	this->_data->window.draw(lblName);
	this->_data->window.draw(lblAddress);

	_data->window.display();
}
