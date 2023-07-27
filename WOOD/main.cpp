#define SDL_MAIN_HANDLED

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <math.h>

#include "structs.h"
#include "sdl_event_handler.h"
#include "renderer.h"
#include "game.h"
#include "texture_mapper.h"
#include "user_interface.h"
#include "controls.h"
#include "weapon.h"
#include "enemy.h"


int main() 
{
	TTF_Init();

	Map map;
	SDLVariables sdl;
	Player player;
	Game game;
	TextureMap textureMap;
	UserInterface ui;

	loadAllEnemies(&game);
	loadMap("test", &player, &map);

	spawnEnemy(0, 9.5f, 13.5f, &game);
	spawnEnemy(0, 11.5f, 17.5f, &game);
	spawnEnemy(0, 11.5f, 19.5f, &game);

	player.planeY = player.fieldOfView / 90.f;
	player.sensitivity /= (0x9600);
	player.movementSpeed /= 100;
	player.verticalFieldOfView = 2.f * atanf(tanf(DEG2RAD * player.fieldOfView / 2.f) * HEIGHT/WIDTH) * RAD2DEG;

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return 1;

	userInterfaceInit(&ui);

	sdl.window = SDL_CreateWindow("WOOD 3D", 200, 100, WIDTH, HEIGHT, 0);
	sdl.renderer = SDL_CreateRenderer(sdl.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 4, 2048);

	game.ambientMusic = Mix_LoadMUS("assets/sound/ambient_music.wav");
	Mix_VolumeMusic(0xF);
	Mix_PlayMusic(game.ambientMusic, -1);

	if (player.useMouse)
	{
		if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0)
			return 1;
	}

	loadAllWeapons(&game, &textureMap);
	preloadAllTextures(&textureMap, sdl.renderer);

	Uint64 NOW = SDL_GetTicks();
	Uint64 LAST = 0;

	//inputHandler(&player, &game, &map);
	std::thread inputThread(input, &player, &game, &map);

	while (game.isRunning)
	{
		LAST = NOW;
		NOW = SDL_GetTicks();
		game.deltaTime = (float)(NOW - LAST) / 1000.f;

		SDL_PumpEvents();
		
		rayCastingVectors(&player, &game, &sdl, &textureMap, &map);
		
		loadWeapons(&player, &game, &sdl, &textureMap);
		renderUI(&sdl, &player, &game, &ui);
		renderTexture(&sdl, &textureMap, &ui);
		

		//printf("================ MS: %f ================ ", game.deltaTime * 1000.f);
		//printf("playerPos: %f %f viewAngle: %f %f FPS: %f\n", player.x, player.y, player.planeX, player.planeY, 1.f / game.deltaTime);
	}


	inputThread.join();

	SDL_DestroyRenderer(sdl.renderer);
	SDL_DestroyWindow(sdl.window);

	TTF_CloseFont(ui.uiFont24);
	TTF_CloseFont(ui.uiFont40);
	TTF_CloseFont(ui.uiFont60);

	SDL_FreeSurface(textureMap.fistsTexture);
	SDL_FreeSurface(textureMap.muzzelFlashTextures);
	SDL_FreeSurface(textureMap.texture);
	SDL_FreeSurface(textureMap.weaponTexture);

	IMG_Quit();
	SDL_Quit();

	return 0;
}