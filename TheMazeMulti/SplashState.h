#pragma once
#include <SFML\Graphics.hpp>
#include "Game.h"
#include "State.h"

class SplashState : public State
{
public:
	SplashState(GameDataRef data);
	~SplashState();

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

private :
	GameDataRef _data;

	sf::Clock _clock;

	sf::Sprite _background;
};

