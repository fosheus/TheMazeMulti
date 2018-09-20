#include "GameState.h"
#include "DEFINITIONS.h"
#include "MainMenuState.h"
#include "Common.h"



GameState::GameState(GameDataRef data,sf::String address,sf::String pseudo,bool local) :_data(data),maze(data),mazeRender(data),moveList(0)
{
	
	if (!local) {
		this->address =  address.toAnsiString();
		this->_data->window.setTitle("CLIENT");
		std::cout << "CLIENT" << std::endl;
	}
	else {
		server = new GameServer(data, LISTEN_PORT);
		
		this->address = "127.0.0.1";
		this->_data->window.setTitle("SERVER");
		std::cout << "SERVER" << std::endl;
	}
	this->pseudo = pseudo;
	packetManager = new ClientPacketManager(_data,this,address,LISTEN_PORT);
	identified = false;
	sendTimeout = 0;
	pingDt = 0;
	identifier = 0;
	
	
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
	if (!packetManager->connect()) {
		std::cout << "No connection is possible" << std::endl;	
	}
	deltaX = 0;
	deltaY = 0;
	score.setFont(_data->assets.GetFont(BASIC_FONT));
	score.setFillColor(sf::Color::Black);
	pingTxt.setFont(_data->assets.GetFont(BASIC_FONT));
	pingTxt.setFillColor(sf::Color::Black);

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
		packetManager->startGame(identifier);
	}
}

void GameState::Update(float dt)
{
	std::map<sf::Uint16, Entity*>::iterator it;
	sendTimeout += dt;
	packetManager->receivePackets(identifier);
	if (identified) {
		if (updated) {
			moveList.addMove(deltaX*dt / (TEXTURE_SIZE*scale), deltaY*dt / (TEXTURE_SIZE*scale));
			Entity * e = mapIdEntities[identifier];
			if (e->getX() + deltaX * dt > 0 && e->getX() + deltaX * dt < _data->window.getSize().x) {
				mapIdEntities[identifier]->move(deltaX, 0, dt);
			}
			if (e->getY() + deltaY * dt > 0 && e->getY() + deltaY * dt < _data->window.getSize().y) {
				mapIdEntities[identifier]->move(0, deltaY, dt);
			}
			updated = false;
		}
		for (it = mapIdEntities.begin(); it != mapIdEntities.end(); it++) {
			it->second->interpolation(dt);
		}
		if (sendTimeout > 4.0f / 60.0f) {
			if (moveList.getMoveListSize() > 0) {
				packetManager->sendMoveList(moveList, identifier);
				packetManager->sendWaitingPackets();
			}
			sendTimeout = 0.0f;
		}
	}

}

void GameState::Draw(float dt)
{
	std::map<sf::Uint16, Entity*>::iterator it;
	//CLEAR AND SET WINDOW//

	this->_data->window.clear(sf::Color(0x33,0x66,0xff));

	
	//this->_data->window.draw(_background);
	if (maze.getGenerated()) {
		//maze.draw(_data->window,scale);
		_data->window.draw(mazeRender);
	}
	int i = 0;

	if (identified) {
		for (it = mapIdEntities.begin(); it != mapIdEntities.end(); it++) {
			sf::CircleShape oui;
			oui.setPosition(it->second->getX(), it->second->getY());
			oui.setRadius(30);
			oui.setOrigin(30, 30);
			oui.setFillColor(sf::Color::Transparent);
			oui.setOutlineThickness(1);
			oui.setOutlineColor(sf::Color::Black);
			//this->_data->window.draw(oui);
			this->_data->window.draw(*it->second);
			score.setPosition(_data->window.getSize().x / 5 * 4, _data->window.getSize().y / 5 + 50 * i++);
			score.setString(it->second->getName().toAnsiString() + " : " + std::to_string(it->second->getScore()));
			_data->window.draw(score);
		}
		
	}
	pingTxt.setString(std::to_string(pingDt));
	_data->window.draw(pingTxt);


	
	this->_data->window.display();
}


void GameState::closeAll()
{
	packetManager->disconnect();
	if (server != nullptr) {
		server->stopServer();
		delete server;
		server = nullptr;
	}
}


void GameState::updateEntityName(std::pair<sf::Uint16, sf::String> nameIdPair)
{
	mapIdEntities[nameIdPair.first]->setName(nameIdPair.second);
}

void GameState::ping()
{
	pingDt = pingClock.getElapsedTime().asMilliseconds();
}

void GameState::welcomed(sf::Uint16 identifier)
{
	this->identifier = identifier;
	identified = true;
	packetManager->sendName(identifier, this->pseudo);
	mapIdEntities[identifier] = new Entity(identifier, 0,0);
	mapIdEntities[identifier]->getSprite().setFillColor(sf::Color(54,188,107));
	moveList = MoveList(identifier);

}

void GameState::updateEntityModel(std::vector<EntityModel>& em)
{

	for (size_t i = 0; i < em.size(); i++) {
		if (mapIdEntities[em[i].getId()] == nullptr) {
			mapIdEntities[em[i].getId()] = new Entity(em[i].getId(), em[i].getX(), em[i].getY());
		}
		else {
			Entity* e = mapIdEntities[em[i].getId()];
			e->updateFromModel(em[i], scale*TEXTURE_SIZE);
			if (e->getId() == identifier && moveList.getMoveListSize() > 0) {
				Move m = moveList.getOldestMove();
				while (m.getMoveId() < em[i].getLastMoveId()) {
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
}

void GameState::lostConnection()
{
	closeAll();
	std::cout << "Connection lost" << std::endl;
	this->_data->machine.AddState(StateRef(new MainMenuState(this->_data)), true);
}

void GameState::removeEntity(sf::Uint16 id)
{
	delete mapIdEntities[id];
	mapIdEntities.erase(id);
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

void GameState::startGame()
{
}

void GameState::levelCompleted(sf::Uint16 id)
{
	std::map<sf::Uint16, Entity*>::iterator it;
	maze.clearMaze();
	for (it = mapIdEntities.begin(); it != mapIdEntities.end(); it++) {
		mapIdEntities[it->first]->setRadius(bazeRadius);
	}
}



