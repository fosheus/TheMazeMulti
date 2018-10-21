#include "GameState.h"

using namespace yojimbo;

GameState::GameState(GameDataRef data,sf::String address,sf::String pseudo,bool local) :_data(data),maze(),mazeRender(data),connectionConfig(),
	client(yojimbo::GetDefaultAllocator(),yojimbo::Address("0.0.0.0"),connectionConfig,adapter,0.0f)
{
	
	if (!local) {
		this->endpoint = Address(address.toAnsiString().c_str(), ServerPort);
		this->_data->window.setTitle("The Maze Multi CLIENT");
		std::cout << "CLIENT" << std::endl;
	}
	else {
		this->endpoint = Address(sf::IpAddress::getLocalAddress().toString().c_str(), ServerPort);
		this->_data->window.setTitle("The Maze Multi SERVER");
		std::cout << "SERVER" << std::endl;
		_data->server.startServer();
	}


	hasFocus = true;
	this->pseudo = pseudo;
	sendClock = 0;
	currentState = State::DISCONNECTED;
	yojimbo::random_bytes((uint8_t*)&clientId, 8);
	for (int i = 0; i < MAX_PLAYERS; i++) {
		players[i] = NULL;
	}
}

GameState::~GameState()
{
	
}



void GameState::quit()
{

	client.Disconnect();
	_data->server.stopServer();
	this->_data->machine.AddState(StateRef(new MainMenuState(this->_data)), true);

}

void GameState::Init()
{

	this->_background.setTexture(this->_data->assets.GetImage(GAME_STATE_BACKGROUND_FILEPATH));
	this->_background.setPosition(this->_data->window.getSize().x / 2 - this->_background.getGlobalBounds().width / 2, this->_data->window.getSize().y / 2 - this->_background.getGlobalBounds().height / 2);

	arrow.setTexture(this->_data->assets.GetImage(ARROW_POSITION));
	arrow.setOrigin(arrow.getTexture()->getSize().x / 2, arrow.getTexture()->getSize().y / 2);
	client.InsecureConnect(DEFAULT_PRIVATE_KEY, clientId, endpoint);


	char addressString[256];
	client.GetAddress().ToString(addressString, sizeof(addressString));
	printf("client address is %s\n", addressString);

	score.setFont(_data->assets.GetFont(BASIC_FONT));
	score.setFillColor(sf::Color::Black);
	rtt.setFont(_data->assets.GetFont(BASIC_FONT));
	rtt.setFillColor(sf::Color::Black);

	clientTime = 0.0f;

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
			if (client.CanSendMessage((int)GameChannel::RELIABLE)) {
				GameEventMessage* message = (GameEventMessage*)client.CreateMessage((int)GameMessageType::GAME_EVENT_MESSAGE);
				message->gameEventType = (int)GameEventType::NEW_GAME;
				message->sender = client.GetClientIndex();
				client.SendMessage((int)GameChannel::RELIABLE, message);
			}
		}
		if ((up || down) && (left || right)) {
			deltaX /= std::sqrt(2.0f);
			deltaY /= std::sqrt(2.0f);
		}
		displayPath = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
			displayPath = true;
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

	client.AdvanceTime(clientTime);
	client.ReceivePackets();

	//detects first connection
	if (currentState == DISCONNECTED && client.IsConnected()) {
		currentState = CONNECTED;
		clientIndex = client.GetClientIndex();
		std::cout << "Successful connection"<<std::endl;
	}
	//detects disconnection
	if (currentState == CONNECTED && !client.IsConnected()) {
		players[clientIndex] = NULL;
		for (int i = 0; i < MAX_PLAYERS; i++) {
			delete players[i];
		}
		currentState = DISCONNECTED;
		std::cout << "Disconnection" << std::endl;
		quit();
	}

	if (currentState == CONNECTED) {
		processMessages();
		e = players[clientIndex];
	}

	/*
	update local state 
	*/
	if (e != NULL) {
		if (updated) {
			MoveMessage* message = (MoveMessage*)client.CreateMessage((int)GameMessageType::MOVE_MESSAGE);
			deltaX = deltaX * dt / (TEXTURE_SIZE*scale);
			deltaY = deltaY * dt / (TEXTURE_SIZE*scale);
			
			e->moveLocal(deltaX, deltaY, 1);
			message->deltaX = deltaX;
			message->deltaY = deltaY;
			
			if (client.CanSendMessage((int)GameChannel::UNRELIABLE)) {
				message->moveId = moveId++;
				client.SendMessage((int)GameChannel::UNRELIABLE, message);
			}
			else {
				delete message;
			}
			updated = false;
		}
		for (int i = 0; i < MAX_PLAYERS; i++) {
			if (players[i] != nullptr) {
				players[i]->interpolation(dt);
			}
		}

		arrow.setPosition(players[clientIndex]->getLocalPosition());
		arrow.setRotation(0.0f);
		bool left = false;
		bool right = false;
		//manage arrow position
		if (e->getLocalPosition().x < 0) {
			arrow.rotate(180);
			arrow.setPosition(10 + arrow.getGlobalBounds().width / 2, arrow.getPosition().y);
			left = true;
		}
		if (e->getLocalPosition().x > _data->window.getSize().x) {
			arrow.setPosition(_data->window.getSize().x -10 - arrow.getGlobalBounds().width / 2, arrow.getPosition().y);
			right = true;
		}
		if (e->getLocalPosition().y < 0) {
			if (left) {
				arrow.rotate(45.0f);
				arrow.setPosition(10 + arrow.getGlobalBounds().width / 2, 10 + arrow.getGlobalBounds().height / 2);
			}
			else if (right) {
				arrow.rotate(-45.0f);
				arrow.setPosition(_data->window.getSize().x - 10 - arrow.getGlobalBounds().width / 2, 10 + arrow.getGlobalBounds().height / 2);
			}
			else {
				arrow.rotate(-90.0f);
				arrow.setPosition(arrow.getPosition().x, 10 + arrow.getGlobalBounds().height / 2);
			}
		}
		if (e->getLocalPosition().y > _data->window.getSize().y) {
			if (left) {
				arrow.rotate(-45.0f);
				arrow.setPosition(10 + arrow.getGlobalBounds().width / 2, _data->window.getSize().y - 10 - arrow.getGlobalBounds().height / 2);
			}
			else if (right) {
				arrow.rotate(45.0f);
				arrow.setPosition(_data->window.getSize().x - 10 - arrow.getGlobalBounds().width / 2, _data->window.getSize().y - 10 - arrow.getGlobalBounds().height / 2);
			}
			else {
				arrow.rotate(90.0f);
				arrow.setPosition(arrow.getPosition().x, _data->window.getSize().y - 10 - arrow.getGlobalBounds().height / 2);
			}
			
		}
	}



	if (sendClock >= (4.0f / 60.0f)) {
		client.SendPackets();
		sendClock = 0;
	}


	

	
}

void GameState::Draw(float dt)
{

	this->_data->window.clear(sf::Color(0x33,0x66,0xff));
	
	if (maze.isGenerated() && mazeStatus) {
		_data->window.draw(mazeRender);
		if (displayPath) {
			renderPath();
		}
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
				this->_data->window.draw(*players[i]);
				this->_data->window.draw(oui);
				score.setPosition(_data->window.getSize().x / 5 * 4, _data->window.getSize().y / 5 + 50 * i);
				score.setString(players[i]->getName().toAnsiString() + " : " + std::to_string(players[i]->getScore()));
				NetworkInfo info;
				client.GetNetworkInfo(info);
				rtt.setString("ping : "+ std::to_string(info.RTT/2.0f));
				_data->window.draw(rtt);
				_data->window.draw(score);
			}
		}
		if (players[clientIndex] != NULL) {
			if (players[clientIndex]->getLocalPosition().x < 0 ||
				players[clientIndex]->getLocalPosition().y < 0 ||
				players[clientIndex]->getLocalPosition().x > _data->window.getSize().x ||
				players[clientIndex]->getLocalPosition().y > _data->window.getSize().y) {
				_data->window.draw(arrow);
			}
		}
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
	mazeStatus = message->level.getMazeStatus();
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
	mazeStatus = false;
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
				if (client.CanSendMessage((int)GameChannel::RELIABLE)) {
					PlayerNameMessage* message = (PlayerNameMessage*)client.CreateMessage((int)GameMessageType::PLAYER_NAME_MESSAGE);
					message->name = this->pseudo.toAnsiString();
					message->clientIndex = 0;
					client.SendMessage((int)GameChannel::RELIABLE, message);
				}
			}
		}
		else if (message->action == 0) { //destroy a player
			delete players[message->clientIndex];
		}
	}
}

void GameState::processGenerateMazeMessage(GenerateMazeMessage * message)
{
	mazeStatus = true;
	maze.generateMaze(message->seed, message->width, message->height);
	scale = baseMazeSize / (float)message->width;
	for (int i = 0; i < MAX_PLAYERS;i++) {
		if (players[i] != NULL) {
			players[i]->setRadius(bazeRadius*scale);
		}
	}
	maze.optimizeMazeForRendering();
	mazeRender.load(maze, sf::Vector2u(50, 50), scale);
	astar.setStartPoint(Point(0, 1));
	astar.setDestPoint(Point(maze.getExitPos().x, maze.getExitPos().y));
	astar.find(maze.getMaze(), maze.getWidth(), maze.getHeight());
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

void GameState::renderPath()
{
	sf::CircleShape circle;
	circle.setRadius(5);
	circle.setOrigin(5, 5);
	circle.setFillColor(sf::Color::Red);
	if (maze.isGenerated() && astar.isPathFound()) {
		std::vector<Point> path = astar.getPath();
		for (int i = 0; i < path.size(); i++) {
			circle.setPosition(path[i].x*TEXTURE_SIZE*scale+ TEXTURE_SIZE * scale/2, path[i].y*TEXTURE_SIZE*scale + TEXTURE_SIZE * scale / 2);
			_data->window.draw(circle);
		}
	}
}



