#include "ServerPacketManager.h"




ServerPacketManager::~ServerPacketManager()
{
	socketManager.unbind();
}

CustomPacket ServerPacketManager::generatePacketHeaderFor(int clientId)
{
	return CustomPacket();
}

ServerPacketManager::ServerPacketManager(GameDataRef data, IServerCallback * server, unsigned short port) :_data(data)
{
	this->server = server;
	socketManager.bind(port);
}



void ServerPacketManager::receiveData()
{
	std::queue<CustomPacket> receivedPackets;
	socketManager.receivePackets();
	receivedPackets = socketManager.getReceivedPackets();
	int queueSize = receivedPackets.size();
	for (int i = 0; i < queueSize; i++) {
		handlePacket(receivedPackets.front());
		receivedPackets.pop();
	}

	updateClientsAcks();
}

void ServerPacketManager::sendWaitingData()
{
	socketManager.sendWaitingPackets();
}

void ServerPacketManager::sendBodyPacketToClient(sf::Packet bodyPacket, int clientId)
{
	CustomPacket packet;
	const void * body = bodyPacket.getData();
	int size = bodyPacket.getDataSize();

	packet = generatePacketHeaderFor(clientId);

	packet.packet.append(body, size);
	packet.address = clientsProxy[clientId].address;
	packet.port = clientsProxy[clientId].port;

	socketManager.addToSendList(packet);

	
}

void ServerPacketManager::broadcastBodyPacketToClients(sf::Packet bodyPacket)
{
	CustomPacket packet;
	const void * body = bodyPacket.getData();
	int size = bodyPacket.getDataSize();

	for (int i = 0; i < MAX_CONNECTION; i++) {

		packet = generatePacketHeaderFor(i);

		packet.packet.append(body, size);
		packet.address = clientsProxy[i].address;
		packet.port = clientsProxy[i].port;

		socketManager.addToSendList(packet);

	}
}



void ServerPacketManager::handlePacket(CustomPacket & packet)
{

	RemoteProxy * clientProxy = nullptr;
	Common::PacketHeader header;
	packet.packet >> header;
	//verify protocoleId
	if (header.protocolId != Common::PROTOCOL_ID) {
		//not a packet for this application, ignore it
		return;
	}

	switch (header.packetType) {
	case Common::CONNECTION_REQUEST:
		connectionRequestPacket(packet);
		break;
	case Common::GAME_DATA:
		gameDataPacket(packet);
		break;
	case Common::DISCONNECTION_REQUEST:
		disconnectionRequestPacket(packet);
		break;
	default:
		break;
	}

}
void ServerPacketManager::updateClientsAcks()
{
	for (int i = 0; i < MAX_CONNECTION; i++) {
		if (clientsConnected[i]) {
			clientsProxy[i].deliveryPacketManager.updateAcks();
		}
	}
}
void ServerPacketManager::connectionRequestPacket(CustomPacket& packet) {
	RemoteProxy* remote = isPacketFromNewRemoteHost(packet.address, packet.port);
	if (remote == nullptr) {
		handleNewClient(packet.address, packet.port);
	}
	else {
		sendConnectionAcceptedTo(remote->id);
	}
}

RemoteProxy* ServerPacketManager::isPacketFromNewRemoteHost(const sf::IpAddress & address, const unsigned short port)
{
	int i = 0;
	for (; i < MAX_CONNECTION; i++) {
		if (clientsConnected[i] && clientsProxy[i].address == address && clientsProxy[i].port == port) {
			return clientsProxy + i;
		}
	}
	return nullptr;
}

void ServerPacketManager::handleNewClient(const sf::IpAddress &address, const unsigned short port)
{
	RemoteProxy* client = nullptr;
	int i = -1;
	i = findFirstEmptySlot();
	if (i >= 0) {
		clientsProxy[i].resetProxy(address, port);
		clientsConnected[i] = true;
		nbConnectedClients++;
		clientsProxy[i].connectionSequenceNumber = nextConnectionSequenceNumber++;

		//send connection accepted packet 
	}
	//send connection accepted if socketProxy != nullptr
	//else send connection denied
	if (client != nullptr) {
		sendConnectionAcceptedTo(client->id);
	}
	else {
		sendConnectionDeniedTo(address, port);
	}

}

int ServerPacketManager::findFirstEmptySlot()
{
	//iterate throw clientsConenced and find first empty slot
	if (nbConnectedClients >= MAX_CONNECTION) {
		return -1;
	}
	for (int i = 0; i < MAX_CONNECTION; i++) {
		if (!clientsConnected[i]) {
			return i;
		}
	}
	return -1;
}

void ServerPacketManager::sendConnectionAcceptedTo(const sf::Uint8 clientId)
{
	CustomPacket packet;
	sf::Uint32 protocolId = Common::PROTOCOL_ID;
	sf::Uint8 packetType = Common::CONNECTION_ACCEPTED;
	sf::Uint16 connectionSequenceNumber = clientsProxy[clientId].connectionSequenceNumber;
	packet.packet << protocolId << packetType << connectionSequenceNumber << clientId;
	packet.address = clientsProxy[clientId].address;
	packet.port = clientsProxy[clientId].port;

	socketManager.addToSendList(packet);
	
}

void ServerPacketManager::sendConnectionDeniedTo(const sf::IpAddress& address, const unsigned short port)
{
	CustomPacket packet;
	sf::Uint32 protocolId = Common::PROTOCOL_ID;
	sf::Uint8 packetType = Common::CONNECTION_DENIED;
	packet.packet << protocolId << packetType;
	packet.address = address;
	packet.port = port;
	
	socketManager.addToSendList(packet);
}


void ServerPacketManager::disconnectionRequestPacket(CustomPacket& packet)
{
	Common::PacketDisconnection packetDisconnection;
	int id;
	packet.packet >> packetDisconnection;
	id = packetDisconnection.clientId;
	if (id >= 0 && id < MAX_CONNECTION) {
		if (!(clientsProxy[id].address == packet.address && clientsProxy[id].port == packet.port)) {
			return;
		}
		if (!clientsConnected[id]) {
			return;
		}
		if (packetDisconnection.connectionSequenceNumber == clientsProxy[id].connectionSequenceNumber) {
			broadcastClientDisconnected(id);
			clientsConnected[id] = false;
			nbConnectedClients--;
		}
	}
}

void ServerPacketManager::broadcastClientDisconnected(sf::Uint8 clientId)
{
	CustomPacket packet;
	sf::Uint32 protocolId = Common::PROTOCOL_ID;
	sf::Uint8 packetType = Common::DISCONNECTION_REQUEST;
	packet.packet << protocolId << packetType << clientId;

	for (int i = 0; i < MAX_CONNECTION; i++) {
		if (clientsConnected[i]) {
			packet.address = clientsProxy[i].address;
			packet.port = clientsProxy[i].port;
			socketManager.addToSendList(packet);
		}
	}
}

void ServerPacketManager::gameDataPacket(CustomPacket& packet) {
	Common::PacketReliablilityLayer reliability;
	int id;
	packet.packet >> reliability;
	id = reliability.clientId;
	if (id >= 0 && id < MAX_CONNECTION) {
		if (!(clientsProxy[id].address == packet.address && clientsProxy[id].port == packet.port)) {
			return;
		}
		if (!clientsConnected[id]) {
			return;
		}

		//process packet number
		clientsProxy[id].deliveryPacketManager.processIncomingPacketSeqNumber(reliability.packetSequenceNumber);
		clientsProxy[id].deliveryPacketManager.processIncomingAcks(reliability.lastAck, reliability.ackBits);
		//process acks

	}
}
