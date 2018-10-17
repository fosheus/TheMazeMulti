#pragma once
class IServerConnection
{
public:
	virtual void clientConnection(int clientIndex) = 0;
	virtual void clientDisconnection(int clientIndex) = 0;
};

