#include "Game.h"
#include "DEFINITIONS.h"
#include "Timing.h"
#include <yojimbo.h>
int main(){
	if (!InitializeYojimbo())
	{
		printf("error: failed to initialize Yojimbo!\n");
		return EXIT_FAILURE;
	}
	srand(time(NULL));

	yojimbo_log_level(YOJIMBO_LOG_LEVEL_INFO);
	Game(SCREEN_WIDTH, SCREEN_HEIGHT, "The Maze Multi");
	ShutdownYojimbo();
	return EXIT_SUCCESS;
}	
