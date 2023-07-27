#pragma once

#ifndef GAME
#define GAME

#include <SDL.h>
#include <math.h>
#include <stdio.h>
#include <SDL_mixer.h>
#include<thread>

#include "structs.h"

#include "map.h"
#include "renderer.h"
#include "utils.h"
#include "texture_mapper.h"
#include "weapon.h"
#include "enemy.h"

#define THREADS 8

void rayCastingVectors(Player* p, Game* g, SDLVariables* sdl, TextureMap* tex, Map* m);

#endif