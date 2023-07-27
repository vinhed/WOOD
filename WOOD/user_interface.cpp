#include "user_interface.h"

void userInterfaceInit(UserInterface* ui)
{
	(*ui).uiFont24 = TTF_OpenFont("assets/fonts/SpaceMarine.ttf", 24);
	(*ui).uiFont40 = TTF_OpenFont("assets/fonts/SpaceMarine.ttf", 40);
	(*ui).uiFont60 = TTF_OpenFont("assets/fonts/SpaceMarine.ttf", 60);
	(*ui).colorGreen = { 28, 112, 34, 200 };
}

void renderCrosshair(SDLVariables* sdl, Player* p)
{
	int halfWidth = WIDTH / 2;
	int halfHeight = HEIGHT / 2;
	for (int x = -(*p).crosshairSize / 2; x < (*p).crosshairSize / 2; x++)
	{
		for (int y = -(*p).crosshairWidth / 2; y < (*p).crosshairWidth / 2; y++)
		{
			Uint32 clr = (*p).crosshairColor;
			setPixelColor(sdl, x + halfWidth, y + halfHeight, clr);
			setPixelColor(sdl, y + halfWidth, x + halfHeight, clr);
		}
	}
}

void renderHealth(SDLVariables* sdl, Player* p, UserInterface* ui)
{
	SDL_Surface* healthSurface = TTF_RenderText_Solid((*ui).uiFont60, std::to_string((*p).health).c_str(), (*ui).colorGreen);
	(*ui).healthTexture = SDL_CreateTextureFromSurface((*sdl).renderer, healthSurface);
	int w, h;
	SDL_QueryTexture((*ui).healthTexture, NULL, NULL, &w, &h);
	(*ui).healthRect.x = 30; (*ui).healthRect.y = HEIGHT - h - 30; (*ui).healthRect.w = w; (*ui).healthRect.h = h;
	SDL_FreeSurface(healthSurface);	
}

void renderAmmo(SDLVariables* sdl, Player* p, UserInterface* ui, Game* g)
{
	SDL_Surface* ammoSurface = TTF_RenderText_Solid((*ui).uiFont60, (*p).equippedWeapon == 0 ? "" : std::to_string((*g).weapons[(*p).equippedWeapon].ammo).c_str(), (*ui).colorGreen);
	(*ui).ammoTexture = SDL_CreateTextureFromSurface((*sdl).renderer, ammoSurface);
	int w, h;
	SDL_QueryTexture((*ui).ammoTexture, NULL, NULL, &w, &h);
	(*ui).ammoRect.x = WIDTH - 30 - w; (*ui).ammoRect.y = HEIGHT - h - 30; (*ui).ammoRect.w = w; (*ui).ammoRect.h = h;
	SDL_FreeSurface(ammoSurface);
}

void renderUI(SDLVariables* sdl, Player* p, Game* g, UserInterface* ui)
{
	renderCrosshair(sdl, p);
	renderHealth(sdl, p, ui);
	renderAmmo(sdl, p, ui, g);
}