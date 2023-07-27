#pragma once

#include <SDL_image.h>
#include <map>

#include "structs.h"
#include "renderer.h"
#include "utils.h"

void loadAllEnemies(Game* g);
void spawnEnemy(int i, float x, int y, Game* g);
Uint32* updateEnemies(Game* g, Player* p, SDLVariables* sdl);