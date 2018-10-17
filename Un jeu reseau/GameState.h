#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <yojimbo.h>

#include "shared.h"
#include "Game.h"
#include "State.h"
#include "DEFINITIONS.h"
#include "GameServer.h"
#include "Common.h"
#include "Entity.h"
#include "IClientCallback.h"
#include "ClientPacketManager.h"
#include "Maze.h"
#include "MazeRender.h"
#include "MoveList.h"

#include <string>
#include <iostream>
#include <string>
#include <queue>


class GameState : public State, public IClientCallback
{

public:
	GameState(GameDataRef data,sf::String address,sf::String pseudo,bool local);
	~GameState();

	void Init();
	void HandleInput();
	void Update(float dt);
	void Draw(float dt);


	// Hérité via IClientCallback
	virtual void welcomed(sf::Uint16 identifier) override;

	virtual void updateEntityModel(std::vector<EntityModel>& em) override;

	virtual void lostConnection() override;

	virtual void removeEntity(sf::Uint16 id) override;

	virtual void generateLevel(MazeConfig config) override;

	virtual void startGame() override;

	virtual void levelCompleted(sf::Uint16 id) override;

	virtual void updateEntityName(std::pair<sf::Uint16, sf::String> nameIdPair) override;

	virtual void ping() override;



private :
	void processMessages();
	void processMessage(yojimbo::Message* message);
	void processLevelStateMessage(LevelStateMessage* message);

	void closeAll();
private :
	enum State {
		DISCONNECTED,
		CONNECTED,
	};

private:
	GameDataRef _data;

	GameServer* server;

	yojimbo::Client client;
	yojimbo::Address endpoint;
	GameConnectionConfig connectionConfig;
	GameAdapter adapter;

	MoveList moveList;

	GameState::State currentState;
	int clientIndex;
	Maze maze;
	MazeRender mazeRender;
	Entity* offlinePlayer;
	Entity* players[MAX_PLAYERS];
	bool slostUsed[MAX_PLAYERS];

	sf::Clock _clock;
	float sendTimeout;

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
	float scale = 1;
	const float sideSize = 650;
	const float baseMazeSize = 13;
	const float bazeRadius = 30;

	float pingDt;
	std::string pingStr;
	sf::Clock pingClock;
	sf::Uint32 moveId;




};
