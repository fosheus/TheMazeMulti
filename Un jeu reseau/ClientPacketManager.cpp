#include "ClientPacketManager.h"

ClientPacketManager::ClientPacketManager(GameDataRef data, IClientCallback * client,std::string address, unsigned short port) : _data(data),socketManager(data,this,address,port)
{
	this->client = client;
}

ClientPacketManager::~ClientPacketManager()

{
}

bool ClientPacketManager::connect()
{
	return socketManager.connect();
}


void ClientPacketManager::disconnect()
{
	socketManager.close();
}

void ClientPacketManager::packetReceived(CustomPacket & packet)
{
	sf::Uint8 code;
	packet >> code;
	switch (code)
	{
	case Common::WELCOME:
		client->welcomed(welcomeReceived(packet));
		break;
	case Common::ENTITYPOS:
		client->updateEntityModel(entityPosReceived(packet));
		break;
	case Common::STARTGAME:
		client->startGame();
		break;
	case Common::GENERATELEVEL:
		client->generateLevel(generateLevelReceived(packet));
		break;
	case Common::LEVELCOMPLETED:
		client->levelCompleted(levelCompletedReceived(packet));
		break;
	case Common::DISCONNECT:
		client->removeEntity(disconnectReceived(packet));
		break;
	case Common::NAME:
		client->updateEntityName(nameReceived(packet));
		break;
	case Common::PING:
		client->ping();
		break;
	case Common::NEWGAME:
	case Common::EVENT:
	case Common::LEVELGENERATED :
		std::cout << "unimplemented packet received " + std::to_string(code) << std::endl;
		break;
	default:
		std::cout << "undefined packet received " + std::to_string(code) << std::endl;
		break;
	}
}

void ClientPacketManager::sendEntityModel(EntityModel & em)
{
	CustomPacket packet(em.getId(),true);
	packet << (sf::Uint8)Common::ENTITYPOS;
	packet << em;
	socketManager.sendPacket(packet);
}

void ClientPacketManager::sendMoveList(MoveList & moveList, sf::Uint16 id)
{
	CustomPacket packet(id, true);
	packet << (sf::Uint8)Common::MOVELIST;
	packet << moveList;
	socketManager.sendPacket(packet);
}

void ClientPacketManager::receivePackets(sf::Uint16 id)
{
	if (id != 0) {
		socketManager.receivePacket();
	}
	socketManager.receiveReliablePacket();
}

void ClientPacketManager::sendWaitingPackets()
{
	socketManager.sendWaitingPackets();
}

void ClientPacketManager::startGame(sf::Uint16 id)
{
	CustomPacket packet(id,false);
	sf::Uint8 code = Common::NEWGAME;
	packet << code << id;
	socketManager.sendPacket(packet);

}

void ClientPacketManager::sendName(sf::Uint16 id, sf::String name)
{
	CustomPacket packet(id,false);
	sf::Uint8 code = Common::NAME;
	packet << code << id << name;
	socketManager.sendPacket(packet);
}

void ClientPacketManager::sendPing(sf::Uint16 id)
{
	CustomPacket packet(0,false);
	packet << (sf::Uint8)Common::PING << id;
	socketManager.sendPacket(packet);
}

sf::Uint16 ClientPacketManager::welcomeReceived(CustomPacket & packet)
{
	sf::Uint16 id;
	packet >> id;
	return id;
}

std::vector<EntityModel> ClientPacketManager::entityPosReceived(CustomPacket & packet)
{
	std::vector<EntityModel> entities;
	EntityModel em;
	sf::Uint16 size;
	packet >> size;
	for (size_t i = 0; i < size; i++)
	{
		packet >> em;
		entities.push_back(em);
	}
	return entities;
}

int ClientPacketManager::newGameReceived(CustomPacket & packet)
{
	return 0;
}

void ClientPacketManager::startGameReceived(CustomPacket & packet)
{

}

int ClientPacketManager::eventReceived(CustomPacket & packet)
{
	return 0;
}

MazeConfig ClientPacketManager::generateLevelReceived(CustomPacket & packet)
{
	MazeConfig config;
	packet >> config;
	return config;
}


int ClientPacketManager::disconnectReceived(CustomPacket & packet)
{
	sf::Uint16 id;
	packet >> id;
	return id;
}

void ClientPacketManager::lostConnection()
{
	client->lostConnection();
}

std::pair<sf::Uint16, sf::String> ClientPacketManager::nameReceived(CustomPacket & packet)
{
	sf::Uint16 id;
	sf::String name;
	packet >> id >> name;
	return std::pair<sf::Uint16, sf::String>(id, name);
}

sf::Uint16 ClientPacketManager::pingReceived(CustomPacket & packet)
{
	sf::Uint16 id;
	packet >> id;
	return id;
}

sf::Uint16 ClientPacketManager::levelCompletedReceived(CustomPacket & packet)
{
	sf::Uint16 id;
	packet >> id;
	return id;
}

int ClientPacketManager::levelGeneratedReceived(CustomPacket & packet)
{
	return 0;
}

