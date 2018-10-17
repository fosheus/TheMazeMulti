#pragma once
#include <SFML/System.hpp>
class Timing
{
private:
	static Timing* _instance;
	Timing();
public:
	Timing(const Timing&) = delete;
	Timing& operator=(const Timing&) = delete;
	static Timing* getInstance();
	sf::Int32 getElapsedTime();
	void kill();
private:
	sf::Clock clock;

};

