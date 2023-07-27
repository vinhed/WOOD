#pragma once

#include <fstream>
#include <string>

#include "game.h"
#include "structs.h"


int getWallTexture(int x, int y, Map* m);
int getFloorTexture(int x, int y, Map* m);
int getCeilingTexture(int x, int y, Map* m);
int getInteractionTexture(int x, int y, Map* m);

void loadMap(const char* mapName, Player* p, Map* m);
