#pragma once
#include <SFML\Graphics.hpp>
#include "Game.h"
#include "State.h"
#include "DEFINITIONS.h"

class MainMenuState : public State
{
public:
	MainMenuState(GameDataRef data);
	~MainMenuState();

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

private:
	GameDataRef _data;

	sf::Clock _clock;

	sf::Sprite _background;

	sf::Sprite _playButton;
	sf::Sprite _exitButton;
};

