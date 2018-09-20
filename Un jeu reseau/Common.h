#pragma once

class Common 
{
public:
	enum ServerMessages {
		WELCOME = 0x01, //client receives his id
		ENTITYPOS = 0x02, //client or server receives 1 or more entityModel
		NEWGAME = 0x03,  //host choses to start a new game, must be in lobby (ask server to generate map and send seed to clients)
		STARTGAME = 0x04, //client starts the game, must be in lobby
		EVENT = 0x05, //not implemeted
		LEVELCOMPLETED = 0x06, //server informs clients that player x won the round,clients go to lobby
		GENERATELEVEL = 0x07, //server sends seed, clients go to lobby
		LEVELGENERATED =0x08, //client informs server that it has finished generating map
		NAME = 0x09,
		PING = 0x0A,
		MOVELIST = 0x0B,
		DISCONNECT = 0xFF, //server informs clients that one has disconnected
	};

};

