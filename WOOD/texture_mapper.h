#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>

#define WIDTH 1280
#define HEIGHT 720
#define CHANNELS 4

#define TEXTUREWIDTH 114
#define TEXTURESIZE 64

struct textureMapper
{
	SDL_Surface* fistsTexture = NULL;
	SDL_Surface* weaponTexture = NULL;
	SDL_Surface* muzzelFlashTextures = NULL;
	SDL_Surface* texture = NULL;
	Uint32* textureMap = new Uint32[TEXTURESIZE * TEXTURESIZE * TEXTUREWIDTH];
};
typedef struct textureMapper TextureMap;

void preloadAllTextures(TextureMap* tex, SDL_Renderer* renderer);
SDL_Rect* getTextureRectWall(float pct, const char* name, int textureID);
Uint32 colorFromTexture(TextureMap* tex, int i, int x, int y);
