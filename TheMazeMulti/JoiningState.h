#pragma once
#include "State.h"
#include "Game.h"
#include "GameState.h"

class JoiningState :
	public State
{
public:
	JoiningState(GameDataRef data);
	~JoiningState();


	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

private:
	GameDataRef _data;

	sf::Clock _clock;

	sf::Sprite _background;

	sf::Sprite _hostButton;
	sf::Sprite _joinButton;
	sf::Sprite _cancelButton;
	sf::Sprite _addressTextArea;
	sf::Sprite _nameTextArea;

	bool textAddressHasFocus;
	bool textNameHasFocus;

	bool joinButtonClicking;
	bool hostButtonClicking;
	bool cancelButtonClicking;


	sf::String addressStr;
	sf::String nameStr;
	sf::Text address;
	sf::Text name;
	sf::Text lblName;
	sf::Text lblAddress;
	sf::Font basicFont;


	


};

