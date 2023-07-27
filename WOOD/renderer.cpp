#include "renderer.h"


void renderTexture(SDLVariables* sdl, TextureMap* textureMap, UserInterface* ui)
{
	SDL_RenderClear((*sdl).renderer);
    (*sdl).texture = SDL_CreateTextureFromSurface((*sdl).renderer, (*sdl).surface);
    SDL_FreeSurface((*sdl).surface);
    SDL_RenderCopy((*sdl).renderer, (*sdl).texture, NULL, NULL);
    SDL_RenderCopy((*sdl).renderer, (*ui).healthTexture, NULL, &(*ui).healthRect);
    SDL_RenderCopy((*sdl).renderer, (*ui).ammoTexture, NULL, &(*ui).ammoRect);
    
    SDL_DestroyTexture((*sdl).texture);
    SDL_DestroyTexture((*ui).healthTexture);
    SDL_DestroyTexture((*ui).ammoTexture);
    
	SDL_RenderPresent((*sdl).renderer);
}

void setPixelColor(SDLVariables* sdl, int x, int y, Uint32 hex, int r, int g, int b, int a)
{
    if (hex != NULL)
    {
        r = (hex & 0xFF000000) >> 24;
        g = (hex & 0x00FF0000) >> 16;
        b = (hex & 0x0000FF00) >> 8;
        a = (hex & 0x000000FF);
    }

    unsigned int i = (y * WIDTH + x) * 4;
    (*sdl).pixels[i] = r;
    (*sdl).pixels[i + 1] = g;
    (*sdl).pixels[i + 2] = b;
    (*sdl).pixels[i + 3] = a;
}
