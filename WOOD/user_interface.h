#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "game.h"
#include "renderer.h"
#include "structs.h"


void renderUI(SDLVariables* sdl, Player* p, Game* g, UserInterface* ui);
void userInterfaceInit(UserInterface* ui);