#pragma once

#ifndef RENDERER
#define RENDERER

#include <SDL.h>
#include <stdio.h>
#include <SDL_ttf.h>

#include "texture_mapper.h"
#include "structs.h"

struct sdlVariables
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* surface;
	SDL_Texture* texture;
	char* pixels = new char[WIDTH * HEIGHT * CHANNELS];
};
typedef struct sdlVariables SDLVariables;

void renderTexture(SDLVariables* sdl, TextureMap* textureMap, UserInterface* ui);
void setPixelColor(SDLVariables* sdl, int x, int y, Uint32 hex = NULL, int r = NULL, int g = NULL, int b = NULL, int a = NULL);

#endif