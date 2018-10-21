#include "Game.h"
#include "DEFINITIONS.h"
#include "Timing.h"
#include <yojimbo.h>
#include <Windows.h>
int main(){
	if (!InitializeYojimbo())
	{
		printf("error: failed to initialize Yojimbo!\n");
		return EXIT_FAILURE;
	}

	yojimbo_log_level(YOJIMBO_LOG_LEVEL_INFO);
	Game(SCREEN_WIDTH, SCREEN_HEIGHT, "The Maze Multi");
	ShutdownYojimbo();
	return EXIT_SUCCESS;
}	
