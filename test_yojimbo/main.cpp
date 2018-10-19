#include <SFML/Graphics.hpp>
#include "yojimbo.h"
#include "shared.h"
#include <iostream>

using namespace yojimbo;

int main()
{
	if (!InitializeYojimbo()) {
		std::cout << "Erreur yojimbo" << std::endl;
	}
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	double time = 100.0;

	GameConnectionConfig config;

	uint8_t privateKey[KeyBytes];
	memset(privateKey, 0, KeyBytes);
	Address serverAddress("127.0.0.1", ServerPort);

	printf("starting server on port %d\n", ServerPort);

	Server server(GetDefaultAllocator(), privateKey, serverAddress, config, adapter, time);

	server.Start(MaxClients);

	if (!server.IsRunning())
		return 1;

	printf("started server\n");

	uint64_t clientId = 0;
	random_bytes((uint8_t*)&clientId, 8);
	printf("client id is %.16" PRIx64 "\n", clientId);


	Client client(GetDefaultAllocator(),serverAddress, config, adapter, time);

	client.InsecureConnect(privateKey, clientId, serverAddress);

	const double deltaTime = 0.1;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		server.SendPackets();
		client.SendPackets();

		server.ReceivePackets();
		client.ReceivePackets();

		time += deltaTime;

		client.AdvanceTime(time);

		if (client.IsDisconnected())
			break;

		time += deltaTime;

		server.AdvanceTime(time);

		yojimbo_sleep(deltaTime);

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}