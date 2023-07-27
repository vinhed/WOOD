#include "sdl_event_handler.h"


int sdlEventHandler(SDL_Event e)
{
	//			0x0 = Stop Game


	if (e.type == SDL_QUIT)
	{
		return 0x0;
	}

	else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
	{
		return 0x0;
	}

	return -1;
}