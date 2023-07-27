#pragma once

#include <SDL.h>

#include "structs.h"
#include "game.h"


void loadAllWeapons(Game* g, TextureMap* tex);
void loadWeapons(Player* p, Game* g, SDLVariables* sdl, TextureMap* tex);