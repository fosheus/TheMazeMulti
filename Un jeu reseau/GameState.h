#pragma once
#include <SFML\Graphics.hpp>
#include <SFML/Network/IpAddress.hpp>
#include <yojimbo.h>

#include "shared.h"
#include "Game.h"
#include "State.h"
#include "DEFINITIONS.h"
#include "GameServer.h"
#include "Entity.h"
#include "Maze.h"
#include "MazeRender.h"
#include "MoveList.h"
#include "Timing.h"
#include "MainMenuState.h"
#include <string>
#include <iostream>
#include <string>
#include <queue>
#include <string.h>  
#include <errno.h>  



using namespace yojimbo;

class GameState : public State
{

public:
	GameState(GameDataRef data,sf::String address,sf::String pseudo,bool local);
	~GameState();

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);


private :
	void processMessages();
	void processMessage(yojimbo::Message* message);
	void processLevelStateMessage(LevelStateMessage* message);
	void processPlayerNameMessage(PlayerNameMessage* message);
	void processPlayerWonMessage(PlayerWonMessage* message);
	void processEventCDPlayerMessage(EventCDPlayerMessage* message);
	void processGenerateMazeMessage(GenerateMazeMessage* message);
	void processGameEventMessage(GameEventMessage* message);

	bool connectionSucceed();

	void quit();
private :
	enum State {
		DISCONNECTED,
		CONNECTED,
	};

private:
	GameDataRef _data;


	GameConnectionConfig connectionConfig;
	GameAdapter adapter;
	yojimbo::Client client;
	yojimbo::Address endpoint;
	uint64_t clientId;
	bool hasFocus;
	float sendClock;
	float clientTime;


	GameState::State currentState;
	
	int clientIndex;
	Maze maze;
	MazeRender mazeRender;
	Entity* offlinePlayer;
	Entity* players[MAX_PLAYERS];
	bool slostUsed[MAX_PLAYERS];

	sf::Clock _clock;

	sf::Sprite _background;

	float deltaX;
	float deltaY;

	bool updated;
	bool identified;

	sf::String pseudo;
	sf::Uint16 identifier;

	sf::Text score;
	sf::Font basicFont;
	sf::Text pingTxt;

	const float TEXTURE_SIZE = 50;
	float scale = 1.0f;
	const float baseMazeSize = 13;
	const float bazeRadius = 30;

	uint32_t moveId;

};
