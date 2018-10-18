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


const uint64_t ProtocolId = 0x11223364556677ULL;

const int ClientPort = 30000;
const int ServerPort = 40000;

static const uint8_t DEFAULT_PRIVATE_KEY[yojimbo::KeyBytes] = { 0 };
static const int MAX_PLAYERS = 5;

enum class GameMessageType
{
	MOVE_MESSAGE,
	LEVEL_STATE_MESSAGE,
	PLAYER_WON_MESSAGE,
	NUM_MESSAGE_TYPES
};

enum class GameChannel {
	RELIABLE,
	UNRELIABLE,
	COUNT
};

struct GameConnectionConfig : ClientServerConfig {
	GameConnectionConfig() {
		numChannels = 2;
		channel[(int)GameChannel::RELIABLE].type = yojimbo::CHANNEL_TYPE_RELIABLE_ORDERED;
		channel[(int)GameChannel::UNRELIABLE].type = yojimbo::CHANNEL_TYPE_UNRELIABLE_UNORDERED;
	}
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


YOJIMBO_MESSAGE_FACTORY_START(GameMessageFactory,(int)GameMessageType::NUM_MESSAGE_TYPES);
YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::MOVE_MESSAGE, MoveMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::LEVEL_STATE_MESSAGE, LevelStateMessage);
YOJIMBO_DECLARE_MESSAGE_TYPE((int)GameMessageType::PLAYER_WON_MESSAGE, PlayerWonMessage);
YOJIMBO_MESSAGE_FACTORY_FINISH();

class GameAdapter : public Adapter
{
public:

	MessageFactory * CreateMessageFactory(Allocator & allocator)
	{
		return YOJIMBO_NEW(allocator, GameMessageFactory, allocator);
	}
	explicit GameAdapter(IServerConnection* server = NULL) : m_server(server) {}
	
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




