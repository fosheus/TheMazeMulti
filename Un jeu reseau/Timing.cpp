#include "Timing.h"
#include <time.h>

Timing::Timing()
{
	clock.restart();
}

sf::Int32 Timing::getElapsedTime()
{
	return clock.getElapsedTime().asMilliseconds();
}

void Timing::kill()
{
	delete _instance;
}

Timing * Timing::getInstance()
{
	if (!_instance) {
		_instance = new Timing();
	}
	return _instance;
}


