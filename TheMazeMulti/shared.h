/*
	Shared Code for Tests and Examples.

	Copyright © 2016 - 2017, The Network Protocol Company, Inc.

	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

		1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

		2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
		   in the documentation and/or other materials provided with the distribution.

		3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
		   from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
	INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
	USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once



#include <yojimbo.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include "Level.h"
#include "IServerConnection.h"
#include <SFML/Graphics/Color.hpp>



const uint64_t ProtocolId = 0x11223344556677ULL;

const int ClientPort = 30000;
const int ServerPort = 40000;

static const uint8_t DEFAULT_PRIVATE_KEY[yojimbo::KeyBytes] = { 0 };
static const int MAX_PLAYERS = 5;
static const sf::Color PLAYERS_COLORS[MAX_PLAYERS] = {sf::Color(128,255,128),sf::Color(255,128,128),sf::Color(255,128,64),sf::Color(0,128,128),sf::Color(255,128,255)};

enum class GameMessageType
{
	CONNECTION_MESSAGE,
	MOVE_MESSAGE,
	LEVEL_STATE_MESSAGE,
	PLAYER_NAME_MESSAGE,
	PLAYER_WON_MESSAGE,
	EVENT_CD_PLAYER_MESSAGE,
	GENERATE_MAZE_MESSAGE,
	GAME_EVENT_MESSAGE,
	NUM_MESSAGE_TYPES
};

enum class GameChannel {
	RELIABLE,
	UNRELIABLE,
	COUNT
};

enum class GameEventType {
	NEW_GAME,
	NUM_GAME_EVENT_TYPE
};

struct GameConnectionConfig : ClientServerConfig {
	GameConnectionConfig() {
		numChannels = 2;
		timeout = -1;
		channel[(int)GameChannel::RELIABLE].type = yojimbo::CHANNEL_TYPE_RELIABLE_ORDERED;
		channel[(int)GameChannel::UNRELIABLE].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
	}
	~GameConnectionConfig() {}
};

struct ConnectionMessage : public Message {
	std::string names[MAX_PLAYERS];
	ConnectionMessage() {

	}
	template<typename Stream> bool Serialize(Stream &stream) {
		
		if(Stream::IsWriting){
			for (int i = 0; i < MAX_PLAYERS; i++) {
				serialize_string(stream,(char*)&names[i][0], names[i].size() + 8);
			}
		}
		if (Stream::IsReading) {
			char s[255];
			int bufferSize=255;
			for (int i = 0; i < MAX_PLAYERS; i++) {
				serialize_string(stream, s, bufferSize);
				names[i] = s;
			}
		}
		return true;
	}

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};

struct MoveMessage : public Message {
	float deltaX;
	float deltaY;
	int moveId;

	MoveMessage() {
		deltaX = 0.0f;
		deltaY = 0.0f;
		moveId = 0;
	}

	template<typename Stream> bool Serialize(Stream &stream) {
		serialize_float(stream, deltaX);
		serialize_float(stream, deltaY);
		serialize_uint32(stream, moveId);
		return true;
	}

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();
};

struct LevelStateMessage : public yojimbo::Message {
	Level level;

	LevelStateMessage()
	{

	}
	template<typename Stream> bool Serialize(Stream &stream) {
		serialize_object(stream, level);
		return true;
	}

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

};

struct PlayerWonMessage : public Message {
	int playerIndex;

	PlayerWonMessage()
	{
		playerIndex = 0;
	}
	template<typename Stream> bool Serialize(Stream &stream) {
		serialize_int(stream, playerIndex, 0, MAX_PLAYERS);
		return true;
	}

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

};

struct EventCDPlayerMessage : public Message {

	int action; //1 create, 0 destroy
	int clientIndex;
	float x;
	float y;

	EventCDPlayerMessage() {
		action = -1;
		clientIndex = -1;
		x = 0;
		y = 0;
	}
	template<typename Stream> bool Serialize(Stream &stream) {
		serialize_int(stream, action,0,1);
		serialize_int(stream, clientIndex,0,MAX_PLAYERS-1);
		serialize_float(stream, x);
		serialize_float(stream, y);
		return true;
	}

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

};

struct GenerateMazeMessage : public Message {
	uint32_t seed;
	int width;
	int height;

	GenerateMazeMessage() {
		seed = 0;
		width = 0;
		height = 0;
	}
	template<typename Stream> bool Serialize(Stream &stream) {
		serialize_uint32(stream, seed);
		serialize_int(stream, width, 11, 1000);
		serialize_int(stream, height, 11, 1000);
		return true;
	}
	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

};

struct GameEventMessage : public Message {
	int gameEventType;
	int sender;
	GameEventMessage() {

	}
	template<typename Stream> bool Serialize(Stream &stream) {
		serialize_uint32(stream, gameEventType);
		serialize_uint32(stream, sender);
		return true;
	}
	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

};



struct PlayerNameMessage : public Message {
	int clientIndex;
	std::string name;

	PlayerNameMessage() {

	}

	template <typename Stream> bool Serialize(Stream & stream)
	{
		char bufferSize;
		char c;
		if (Stream::IsReading) {
			serialize_int(stream, clientIndex, 0, MAX_PLAYERS);
			serialize_int(stream, bufferSize, 0, 255);
			for (int i = 0; i < bufferSize; i++) {
				serialize_bits(stream, c, 8);
				name += c;
			}
			return true;
		}
		if (Stream::IsWriting) {
			bufferSize = name.size();
			serialize_int(stream, clientIndex, 0, MAX_PLAYERS);
			serialize_int(stream, bufferSize, 0, 255);
			for (int i = 0; i < bufferSize; i++) {
				serialize_bits(stream, name[i], 8);
			}

			return true;
		}
	}
	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS();

};


YOJIMBO_MESSAGE_FACTORY_START(GameMessageFactory,(int)GameMessageType::NUM_MESSAGE_TYPES);
YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::CONNECTION_MESSAGE, ConnectionMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::MOVE_MESSAGE, MoveMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::LEVEL_STATE_MESSAGE, LevelStateMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::PLAYER_WON_MESSAGE, PlayerWonMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::EVENT_CD_PLAYER_MESSAGE, EventCDPlayerMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::GENERATE_MAZE_MESSAGE, GenerateMazeMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::GAME_EVENT_MESSAGE, GameEventMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::PLAYER_NAME_MESSAGE, PlayerNameMessage);
YOJIMBO_MESSAGE_FACTORY_FINISH();



class GameAdapter : public Adapter
{
public:

	explicit GameAdapter(IServerConnection* server = NULL) : m_server(server) {}

	MessageFactory * CreateMessageFactory(Allocator & allocator)
	{
		return YOJIMBO_NEW(allocator, GameMessageFactory, allocator);
	}
	
	void ClientSendLoopbackPacket(int clientIndex, const uint8_t * packetData, int packetBytes, uint64_t packetSequence)
	{
		std::cout << "loopback packet sent from client" << std::endl;
	}

	void ServerSendLoopbackPacket(int clientIndex, const uint8_t * packetData, int packetBytes, uint64_t packetSequence)
	{
		std::cout << "loopback packet sent from server" << std::endl;

	}
	

	void OnServerClientConnected(int clientIndex) override {
		if (m_server != NULL) {
			m_server->clientConnection(clientIndex);
		}
	}

	void OnServerClientDisconnected(int clientIndex) override {
		if (m_server != NULL) {
			m_server->clientDisconnection(clientIndex);
		}
	}
private:

	IServerConnection * m_server;
	
};




