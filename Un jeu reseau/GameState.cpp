#include "GameState.h"

using namespace yojimbo;

GameState::GameState(GameDataRef data,sf::String address,sf::String pseudo,bool local) :_data(data),maze(data),mazeRender(data),moveList(0),connectionConfig(),
	client(yojimbo::GetDefaultAllocator(),yojimbo::Address("0.0.0.0"),connectionConfig,adapter,0.0f)
{
	
	uint64_t clientId;
	yojimbo::random_bytes((uint8_t*)&clientId, 8);
	
	if (!local) {
		this->_data->window.setTitle("CLIENT");
		std::cout << "CLIENT" << std::endl;
	}
	else {
		server = new GameServer(data);
		this->_data->window.setTitle("SERVER");
		std::cout << "SERVER" << std::endl;
	}
	this->pseudo = pseudo;
	identified = false;
	sendTimeout = 0;
	pingDt = 0;
	identifier = 0;
	this->endpoint = Address(address.toAnsiString().c_str(),ServerPort);
	client.InsecureConnect(DEFAULT_PRIVATE_KEY, clientId,endpoint);
	currentState = State::DISCONNECTED;
	
	
}

GameState::~GameState()
{
	closeAll();
}

void GameState::Init()
{
	this->_background.setTexture(this->_data->assets.GetImage(GAME_STATE_BACKGROUND_FILEPATH));
	this->_background.setPosition(this->_data->window.getSize().x / 2 - this->_background.getGlobalBounds().width / 2, this->_data->window.getSize().y / 2 - this->_background.getGlobalBounds().height / 2);
	if (server != nullptr) {
		server->startServer();
	}

	deltaX = 0;
	deltaY = 0;
	score.setFont(_data->assets.GetFont(BASIC_FONT));
	score.setFillColor(sf::Color::Black);
	pingTxt.setFont(_data->assets.GetFont(BASIC_FONT));
	pingTxt.setFillColor(sf::Color::Black);

	offlinePlayer = new Entity(0, 0, 0, this->pseudo);
	offlinePlayer->getSprite().setFillColor(sf::Color::Green);
	moveId = 0;

}

void GameState::HandleInput()
{
	sf::Event event;
	
	while (this->_data->window.pollEvent(event))
	{

		if (event.type == sf::Event::Closed) {
			closeAll();
			this->_data->machine.AddState(StateRef(new MainMenuState(this->_data)), true);

		}

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		closeAll();
		this->_data->machine.AddState(StateRef(new MainMenuState(this->_data)), true);
	}
	deltaX = 0;
	deltaY = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		updated = true;
		deltaX -= 200;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		updated = true;
		deltaY += 200;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		updated = true;
		deltaX += 200;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		updated = true;
		deltaY -= 200;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {

	}
}

void GameState::Update(float dt)
{
	Entity * e = NULL;
	sendTimeout += dt;

	client.AdvanceTime(dt);
	client.ReceivePackets();
	if (!client.IsConnected()) {
		e = offlinePlayer;
		if (currentState == State::CONNECTED) {
			//we just get disconnected so we swap the current player entity to the offline entity
			players[clientIndex] = NULL;
			for (int i = 0; i < MAX_PLAYERS; i++) {
				delete players[i];
			}
			currentState = DISCONNECTED;
		}
	}
	else {
		if (currentState == DISCONNECTED) {
			clientIndex = client.GetClientIndex();
			//we just connected, we swap 
			players[clientIndex] = offlinePlayer;
			currentState = CONNECTED;
			
		}
		processMessages();
		e = players[clientIndex];
	}

	if (updated) {
		moveList.addMove(deltaX*dt / (TEXTURE_SIZE*scale), deltaY*dt / (TEXTURE_SIZE*scale));
		if (e->getX() + deltaX * dt > 0 && e->getX() + deltaX * dt < _data->window.getSize().x) {
			e->move(deltaX, 0, dt);
		}
		if (e->getY() + deltaY * dt > 0 && e->getY() + deltaY * dt < _data->window.getSize().y) {
			e->move(0, deltaY, dt);
		}
		MoveMessage* message = (MoveMessage*)client.CreateMessage((int)GameMessageType::MOVE_MESSAGE);
		message->deltaX = deltaX * dt;
		message->deltaY = deltaY * dt;
		message->moveId = moveId++;
		client.SendMessage((int)GameChannel::UNRELIABLE,message);
		updated = false;
	}
	for (int i = 0; i < MAX_PLAYERS;i++) {
		if (players[i] != nullptr) {
			players[i]->interpolation(dt);
		}
	}

	if (sendTimeout > 2.0f / 60.0f) {
		client.SendPackets();
		sendTimeout = 0.0f;
	}
}

void GameState::Draw(float dt)
{
	//CLEAR AND SET WINDOW//

	this->_data->window.clear(sf::Color(0x33,0x66,0xff));

	
	//this->_data->window.draw(_background);
	if (maze.getGenerated()) {
		//maze.draw(_data->window,scale);
		_data->window.draw(mazeRender);
	}

	if (currentState == State::CONNECTED) {
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (players[i] != NULL) {
				sf::CircleShape oui;
				oui.setPosition(players[i]->getX(), players[i]->getY());
				oui.setRadius(30);
				oui.setOrigin(30, 30);
				oui.setFillColor(sf::Color::Transparent);
				oui.setOutlineThickness(1);
				oui.setOutlineColor(sf::Color::Black);
				//this->_data->window.draw(oui);
				this->_data->window.draw(*players[i]);
				score.setPosition(_data->window.getSize().x / 5 * 4, _data->window.getSize().y / 5 + 50 * i++);
				score.setString(players[i]->getName().toAnsiString() + " : " + std::to_string(players[i]->getScore()));
				_data->window.draw(score);
			}
		}
	}
	else {
		this->_data->window.draw(*offlinePlayer);
		score.setPosition(_data->window.getSize().x / 5 * 4, _data->window.getSize().y / 5);
		score.setString(offlinePlayer->getName().toAnsiString() + " : " + std::to_string(offlinePlayer->getScore()));
		_data->window.draw(score);
	}
		
	pingTxt.setString(std::to_string(pingDt));
	_data->window.draw(pingTxt);


	
	this->_data->window.display();
}


void GameState::processMessages()
{
	for (int i = 0; i < connectionConfig.numChannels; i++) {
		Message* message = client.ReceiveMessage(i);
		while (message != NULL) {
			processMessage(message);
			client.ReleaseMessage(message);
			message = client.ReceiveMessage(i);
		}
	}
}

void GameState::processMessage(yojimbo::Message * message)
{
	switch (message->GetType())
	{
	case (int)GameMessageType::LEVEL_STATE_MESSAGE: 
		processLevelStateMessage((LevelStateMessage*)message);
		break;
	case (int)GameMessageType::PLAYER_WON_MESSAGE:
		break;
	default:
		break;
	}
}

void GameState::processLevelStateMessage(LevelStateMessage * message)
{
	std::vector<EntityModel> playersModel = message->level.getPlayers();

	for (size_t i = 0; i < playersModel.size(); i++) {
		
		EntityModel currentPlayer = playersModel[i];
		int index = currentPlayer.getId();
		if (players[index] == nullptr) {
			players[index] = new Entity(currentPlayer.getId(), currentPlayer.getX(), currentPlayer.getY());
		}
		else {
			Entity* e =players[index];
			e->updateFromModel(currentPlayer, scale*TEXTURE_SIZE);
			if (e->getId() == identifier && moveList.getMoveListSize() > 0) {
				Move m = moveList.getOldestMove();
				while (m.getMoveId() < currentPlayer.getLastMoveId()) {
					moveList.removeOldestMove();
					if (moveList.getMoveListSize() > 0) {
						m = moveList.getOldestMove();
					}
					else {
						break;
					}
				}
			}
		}
	}

	//remove old players;
}

void GameState::closeAll()
{
	client.Disconnect();
	if (server != nullptr) {
		server->stopServer();
		delete server;
		server = nullptr;
	}
}


/*
void GameState::removeEntity(sf::Uint16 id)
{
	delete players[id];
}

void GameState::generateLevel(MazeConfig config)
{
	
	std::map<sf::Uint16, Entity*>::iterator it;
	maze.generateMaze(config.getSeed(), config.getWidth(), config.getHeight());
	scale = baseMazeSize / (float)config.getWidth();
	for (it = mapIdEntities.begin(); it != mapIdEntities.end(); it++) {
		mapIdEntities[it->first]->setRadius(bazeRadius*scale);
	}
	maze.optimizeMazeForRendering();
	mazeRender.load(maze, sf::Vector2u(50, 50),scale);
	
}

void GameState::levelCompleted(sf::Uint16 id)
{
	/*
	std::map<sf::Uint16, Entity*>::iterator it;
	maze.clearMaze();
	for (it = mapIdEntities.begin(); it != mapIdEntities.end(); it++) {
		mapIdEntities[it->first]->setRadius(bazeRadius);
	}
}
*/


