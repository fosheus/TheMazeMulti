#include "ServerPacketManager.h"




ServerPacketManager::~ServerPacketManager()
{
	socketManager.stopServer();
}

ServerPacketManager::ServerPacketManager(GameDataRef data, IServerCallback * server, unsigned short port) :_data(data),socketManager(data,this,port)
{
	this->server = server;
}

void ServerPacketManager::packetReceived(CustomPacket & packet)
{
	sf::Uint8 code;
	packet >> code;
	switch (code)
	{

	case Common::ENTITYPOS:
		server->entityModel(entityPosReceived(packet)[0]);
		break;
	case Common::NEWGAME :
		server->newGame(newGameReceived(packet));
		break;
	
	case Common::LEVELGENERATED:
		server->levelGenerated(levelGeneratedReceived(packet));
		break;
	case Common::NAME : 
		server->changeClientName(nameReceived(packet));
		break;
	case Common::PING :
		pingReceived(packet);
		break;
	case Common::MOVELIST:
		server->updateFromMoveList(moveListReceived(packet));
		break;
	case Common::WELCOME:
	case Common::EVENT:
	case Common::GENERATELEVEL:
	case Common::LEVELCOMPLETED:
	case Common::DISCONNECT:
		std::cout << "unimplemented packet received " + std::to_string(code) << std::endl;
		break;
	default:
		std::cout << "undefined packet received " + std::to_string(code) << std::endl;
		break;
	}
}

int ServerPacketManager::handleNewClient()
{
	EntityModel* em;
	em = this->server->newClientConnected();

	CustomPacket packet(em->getId(), false);
	sf::Uint8 code = Common::WELCOME;
	packet << code << em->getId();
	socketManager.sendPacket(packet);

	packet.clear();
	std::map<sf::Uint16, EntityModel*> entities = server->getAllEntities();
	std::map<sf::Uint16, EntityModel*>::iterator it;
	code = Common::ENTITYPOS;
	packet << code;
	packet << (sf::Uint16)entities.size();
	for (it = entities.begin(); it != entities.end(); it++) {
		packet << *it->second;
	}
	packet.setId(0);
	socketManager.sendPacket(packet);
	return em->getId();
}

sf::Uint16 ServerPacketManager::welcomeReceived(CustomPacket & packet)
{
	return sf::Uint16();
}

std::vector<EntityModel> ServerPacketManager::entityPosReceived(CustomPacket & packet)
{
	std::vector<EntityModel> ems;
	EntityModel em;
	while (!packet.endOfPacket()) {
		packet >> em;
		ems.push_back(em);
	}
	return ems;
}

int ServerPacketManager::newGameReceived(CustomPacket & packet)
{
	sf::Uint16 id;
	packet >> id;
	return id;
}

void ServerPacketManager::startGameReceived(CustomPacket & packet)
{
}

int ServerPacketManager::eventReceived(CustomPacket & packet)
{
	return 0;
}

int ServerPacketManager::levelGeneratedReceived(CustomPacket & packet)
{
	return 0;
}

int ServerPacketManager::disconnectReceived(CustomPacket & packet)
{
	return 0;
}

MazeConfig ServerPacketManager::generateLevelReceived(CustomPacket & packet)
{
	return MazeConfig();
}

sf::Uint16 ServerPacketManager::levelCompletedReceived(CustomPacket & packet)
{
	return sf::Uint16();
}

sf::Uint16 ServerPacketManager::getNextClientId()
{
	return server->getNextEntityId();
}

void ServerPacketManager::lostConnection()
{

}

std::pair<sf::Uint16, sf::String> ServerPacketManager::nameReceived(CustomPacket & packet)
{
	std::pair<sf::Uint16, sf::String> ret;
	sf::String name;
	sf::Uint16 id;

	packet >> id >> name;

	ret.first = id;
	ret.second = name;
	return ret;
}

sf::Uint16 ServerPacketManager::pingReceived(CustomPacket & packet)
{
	sf::Uint16 id;
	packet >> id;
	packet.clear();
	packet.setId(id);
	packet.setSetAsDatagram(false);
	packet << (sf::Uint8)Common::PING << 0;
	socketManager.sendPacket(packet);
	return id;
}

MoveList ServerPacketManager::moveListReceived(CustomPacket & packet)
{
	sf::Uint16 id;
	MoveList ml(0);
	packet >> ml;
	return ml;
}

void ServerPacketManager::clientLostConnection(sf::Uint16 id)
{
	server->removeClient(id);
	CustomPacket packet(0,false);
	sf::Uint8 code = Common::DISCONNECT;
	packet << code << id;
	socketManager.sendPacket(packet);

}

void ServerPacketManager::startServer()
{
	socketManager.startServer();
}

void ServerPacketManager::stopServer()
{
	socketManager.stopServer();
}

void ServerPacketManager::handleClients()
{
	socketManager.receivePackets();
	socketManager.receiveReliablePackets();
	socketManager.sendWaitingPackets();
}

void ServerPacketManager::receiveData()
{
	socketManager.receivePackets();
	socketManager.receiveReliablePackets();
}

void ServerPacketManager::sendWaitingData()
{
	socketManager.sendWaitingPackets();
}

void ServerPacketManager::broadcastEntityModel(EntityModel & em)
{
	CustomPacket packet(0, true);
	sf::Uint8 code = Common::ENTITYPOS;
	packet << code << (sf::Uint16)1 <<em;
	socketManager.sendPacket(packet);
}

void ServerPacketManager::broadcastLevelState(std::map<sf::Uint16,EntityModel*> & entities)
{
	std::map<sf::Uint16, EntityModel*>::iterator it;
	CustomPacket packet(0, true);
	sf::Uint8 code = Common::ENTITYPOS;
	sf::Uint16 size = entities.size();
	packet << code << size;
	for (it = entities.begin(); it != entities.end(); it++) {
		packet << *it->second;
	}
	socketManager.sendPacket(packet);
}

void ServerPacketManager::broadcastLevelCompleted(sf::Uint16 id)
{
	CustomPacket packet(0,false);
	sf::Uint8 code = Common::LEVELCOMPLETED;
	packet << code << id;
	socketManager.sendPacket(packet);
}

void ServerPacketManager::sendSeedDimension(MazeConfig config)
{
	CustomPacket packet(0,false);
	sf::Uint8 code = Common::GENERATELEVEL;
	packet << code << config;
	socketManager.sendPacket(packet);
}

void ServerPacketManager::broadcastName(sf::Uint16 id, sf::String name)
{
	CustomPacket packet(0,false);
	sf::Uint8 code = Common::NAME;
	packet << code << id << name;
	socketManager.sendPacket(packet);
}



