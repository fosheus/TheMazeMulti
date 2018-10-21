#include "GameState.h"

using namespace yojimbo;

GameState::GameState(GameDataRef data,sf::String address,sf::String pseudo,bool local) :_data(data),maze(),mazeRender(data),connectionConfig(),
	client(yojimbo::GetDefaultAllocator(),yojimbo::Address("0.0.0.0"),connectionConfig,adapter,0.0f)
{
	
	if (!local) {
		this->endpoint = Address(address.toAnsiString().c_str(), ServerPort);
		this->_data->window.setTitle("CLIENT");
		std::cout << "CLIENT" << std::endl;
	}
	else {
		this->endpoint = Address(sf::IpAddress::getLocalAddress().toString().c_str(), ServerPort);
		this->_data->window.setTitle("SERVER");
		std::cout << "SERVER" << std::endl;
		_data->server.startServer();
	}


	hasFocus = true;
	this->pseudo = pseudo;
	identified = false;
	sendClock = 0;
	identifier = 0;
	currentState = State::DISCONNECTED;
	yojimbo::random_bytes((uint8_t*)&clientId, 8);
	
}

GameState::~GameState()
{
	
}

void GameState::quit()
{

	client.Disconnect();
	_data->server.stopServer();

}

void GameState::Init()
{

	this->_background.setTexture(this->_data->assets.GetImage(GAME_STATE_BACKGROUND_FILEPATH));
	this->_background.setPosition(this->_data->window.getSize().x / 2 - this->_background.getGlobalBounds().width / 2, this->_data->window.getSize().y / 2 - this->_background.getGlobalBounds().height / 2);


	client.InsecureConnect(DEFAULT_PRIVATE_KEY, clientId, endpoint);


	char addressString[256];
	client.GetAddress().ToString(addressString, sizeof(addressString));
	printf("client address is %s\n", addressString);

	deltaX = 0;
	deltaY = 0;
	score.setFont(_data->assets.GetFont(BASIC_FONT));
	score.setFillColor(sf::Color::Black);
	pingTxt.setFont(_data->assets.GetFont(BASIC_FONT));
	pingTxt.setFillColor(sf::Color::Black);

	offlinePlayer = new Entity(0, 0, 0, this->pseudo);
	offlinePlayer->getSprite().setFillColor(sf::Color::Green);

}

void GameState::HandleInput()
{
	
	sf::Event event;
	
	while (this->_data->window.pollEvent(event))
	{

		if (event.type == sf::Event::Closed) {
			quit();
			this->_data->machine.AddState(StateRef(new MainMenuState(this->_data)), true);

		}
		if (event.type == sf::Event::GainedFocus) {
			hasFocus = true;
		}
		if (event.type == sf::Event::LostFocus) {
			hasFocus = false;
		}

	}
	
	deltaX = 0;
	deltaY = 0;
	if (hasFocus) {
		bool up = false, down = false, left = false, right = false;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			quit();
			this->_data->machine.AddState(StateRef(new MainMenuState(this->_data)), true);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
			updated = true;
			deltaX -= 200;
			left = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			updated = true;
			deltaY += 200 ;
			down = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			updated = true;
			deltaX += 200 ;
			right = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
			updated = true;
			deltaY -= 200 ;
			up = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
			GameEventMessage* message = (GameEventMessage*)client.CreateMessage((int)GameMessageType::GAME_EVENT_MESSAGE);
			message->gameEventType = (int)GameEventType::NEW_GAME;
			message->sender = client.GetClientIndex();
			client.SendMessage((int)GameChannel::RELIABLE, message);
		}
		if ((up || down) && (left || right)) {
			deltaX /= std::sqrt(2.0f);
			deltaY /= std::sqrt(2.0f);
		}
	}
	
}

void GameState::Update(float dt)
{
	Entity * e = NULL;
	/*
	Process everything corresponding to network
	*/
	sendClock += dt;

	clientTime += dt;

	client.ReceivePackets();

	client.AdvanceTime(clientTime);

	connectionSucceed();

	if (client.IsConnected()) {
		processMessages();
		if (players[client.GetClientIndex()] != NULL) {
			e = players[client.GetClientIndex()];
		}
		else {
			e = offlinePlayer;
		}
	}
	else {
		e = offlinePlayer;
	}

	

	if (client.ConnectionFailed() || client.IsDisconnected()) {
		players[clientIndex] = NULL;
		for (int i = 0; i < MAX_PLAYERS; i++) {
			delete players[i];
		}
		currentState = DISCONNECTED;
		std::cout << "Disconnection" << std::endl;
		quit();
		this->_data->machine.AddState(StateRef(new MainMenuState(this->_data)), true);
	}

	/*
	update local state 
	*/

	if (updated) {
		MoveMessage* message = (MoveMessage*)client.CreateMessage((int)GameMessageType::MOVE_MESSAGE);
		deltaX = deltaX * dt / (TEXTURE_SIZE*scale);
		deltaY = deltaY * dt / (TEXTURE_SIZE*scale);
		if (e->getRemotePosition().x + deltaX > 0 && e->getRemotePosition().x + deltaX < _data->window.getSize().x) {
			e->moveLocal(deltaX, 0,1);
			message->deltaX = deltaX;
			
		}
		if (e->getRemotePosition().y + deltaY > 0 && e->getRemotePosition().y + deltaY < _data->window.getSize().y) {
			e->moveLocal(0, deltaY, 1);
			message->deltaY = deltaY ;
			
		}
		if (client.IsConnected()) {
			message->moveId = moveId++;
			client.SendMessage((int)GameChannel::UNRELIABLE, message);
		}
		else {
			delete message;
		}
		updated = false;
	}
	for (int i = 0; i < MAX_PLAYERS;i++) {
		if (players[i] != nullptr) {
			players[i]->interpolation(dt);
		}
	}

	if (sendClock >= (4.0f / 60.0f)) {
		client.SendPackets();
		sendClock = 0;
	}
	

	
}

void GameState::Draw(float dt)
{
	//CLEAR AND SET WINDOW//

	this->_data->window.clear(sf::Color(0x33,0x66,0xff));
	
	if (maze.isGenerated()) {
		_data->window.draw(mazeRender);
	}

	if (currentState == State::CONNECTED) {
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (players[i] != NULL) {
				sf::CircleShape oui;
				oui.setPosition(players[i]->getRemotePosition().x, players[i]->getRemotePosition().y);
				oui.setRadius(30);
				oui.setOrigin(30, 30);
				oui.setFillColor(sf::Color::Transparent);
				oui.setOutlineThickness(1);
				oui.setOutlineColor(sf::Color::Black);
				//this->_data->window.draw(oui);
				this->_data->window.draw(*players[i]);
				score.setPosition(_data->window.getSize().x / 5 * 4, _data->window.getSize().y / 5 + 50 * i);
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

bool GameState::connectionSucceed()
{
	if (client.IsConnected()) {
		if (currentState == State::DISCONNECTED) {
			currentState = CONNECTED;
			std::cout << "Successful connection" << std::endl;
			return true;
		}
	}
	return false;
}

void GameState::processMessage(yojimbo::Message * message)
{
	switch (message->GetType())
	{
	case (int)GameMessageType::LEVEL_STATE_MESSAGE: 
		processLevelStateMessage((LevelStateMessage*)message);
		break;
	case (int)GameMessageType::PLAYER_WON_MESSAGE:
		processPlayerWonMessage((PlayerWonMessage*)message);
		break;
	case (int)GameMessageType::EVENT_CD_PLAYER_MESSAGE:
		processEventCDPlayerMessage((EventCDPlayerMessage*)message);
		break;
	case (int)GameMessageType::GENERATE_MAZE_MESSAGE:
		processGenerateMazeMessage((GenerateMazeMessage*)message);
		break;
	case (int)GameMessageType::GAME_EVENT_MESSAGE:
		processGameEventMessage((GameEventMessage*)message);
		break;
	case (int)GameMessageType::PLAYER_NAME_MESSAGE:
		processPlayerNameMessage((PlayerNameMessage*)message);
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
		if (players[index] != nullptr) {
			Entity* e = players[index];
			e->updateRemoteFromModel(currentPlayer, scale*TEXTURE_SIZE);
		}
	}
	
}

void GameState::processPlayerWonMessage(PlayerWonMessage * message)
{
	maze.clearMaze();
	for (int i = 0; i < MAX_PLAYERS;i++) {
		if (players[i] != NULL) {
			players[i]->setRadius(bazeRadius);
		}
	}
	//scale = 1.0f;
}

void GameState::processEventCDPlayerMessage(EventCDPlayerMessage * message)
{
	if (message->clientIndex >= 0 && message->clientIndex < MAX_PLAYERS) {
		if (message->action == 1) { //create a player 
			players[message->clientIndex] = new Entity(message->clientIndex, message->x*scale*TEXTURE_SIZE, message->y*scale*TEXTURE_SIZE);
			std::cout << "Création du joueur " + std::to_string(message->clientIndex) << std::endl;
			if (message->clientIndex == client.GetClientIndex()) {

				players[message->clientIndex]->getSprite().setFillColor(sf::Color::Green);
				
				PlayerNameMessage* message = (PlayerNameMessage*)client.CreateMessage((int)GameMessageType::PLAYER_NAME_MESSAGE);
				message->name = this->pseudo.toAnsiString();
				message->clientIndex = 0;
				client.SendMessage((int)GameChannel::RELIABLE, message);
			}
		}
		else if (message->action == 0) { //destroy a player
			delete players[message->clientIndex];
		}
	}
}

void GameState::processGenerateMazeMessage(GenerateMazeMessage * message)
{
	
	maze.generateMaze(message->seed, message->width, message->height);
	scale = baseMazeSize / (float)message->width;
	for (int i = 0; i < MAX_PLAYERS;i++) {
		if (players[i] != NULL) {
			players[i]->setRadius(bazeRadius*scale);
		}
	}
	maze.optimizeMazeForRendering();
	mazeRender.load(maze, sf::Vector2u(50, 50), scale);
}

void GameState::processGameEventMessage(GameEventMessage * message)
{
	std::cout << "Not Implemented" << std::endl;
}

void GameState::processPlayerNameMessage(PlayerNameMessage* message) {

	if (message->clientIndex >= 0 && message->clientIndex < MAX_PLAYERS) {
		if (players[message->clientIndex] != NULL) {
			players[clientIndex]->setName(message->name);
		}
	}
}




