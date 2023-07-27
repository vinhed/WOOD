#pragma once

#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <thread>

#define MAX_ENEMIES 128

struct map
{
	int MAP_WIDTH = 32;
	int MAP_HEIGHT = 32;

	// BYTE 1 = Wall | BYTE 2 = Floor | BYTE 3 = Ceiling | BYTE 4 = Interactable
	Uint32* mapArr = new Uint32[1048576]{
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,23,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,23,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,23,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,23,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,23,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,23,0,0,0,0,0,1,1,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,23,0,0,0,0,0,1,1,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,23,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,23,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,0,0,0,0,23,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,18,0,0,0,0,23,23,0,0,0,0,0,0,1,1,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,18,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,18,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,11,11,0,0,0,0,0,0,0,0,0,0,18,18,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,18,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,18,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,18,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,18,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,22,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,22,22,0,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
	};
};
typedef struct map Map;

struct userInterface
{
	TTF_Font* uiFont24;
	TTF_Font* uiFont40;
	TTF_Font* uiFont60;

	SDL_Color colorGreen;

	SDL_Rect healthRect;
	SDL_Rect ammoRect;

	SDL_Texture* healthTexture;
	SDL_Texture* ammoTexture;
};
typedef struct userInterface UserInterface;

struct weapon
{
	const char* name;
	int ammo;
	bool automatic;
	SDL_Rect fireAnimation;
	int animationDelay;
	SDL_Rect src;
	SDL_Rect dst;
	SDL_Rect muzzelFlashSrc;
	SDL_Rect muzzelFlashDst;
	Mix_Chunk* fireSound;
	Uint64 cooldown;
	int damage;
	int textureId;
	float volume;
	float range;
	float weaponBobbingAmount;
	float weaponBobbingCycleSpeed;
};
typedef struct weapon Weapon;

struct enemy
{
	const char* name;
	int startingHealth;
	int damage;
	float width;
	float height;
	SDL_Surface* sprite;
};
typedef struct enemy Enemy;

struct spawnedEnemy
{
	int enemyId;
	int health;
	float x;
	float y;
	SDL_Rect screenPos;
};
typedef spawnedEnemy SpawnedEnemy;

struct game
{
	bool isRunning = true;
	float stepSize = 0.001f;
	int maxSteps = 10000;
	unsigned int lastUpdateTime = -1;
	float deltaTime = 1;
	Weapon weapons[3];
	Enemy enemies[1];
	SpawnedEnemy spawnedEnemies[MAX_ENEMIES];
	float enemyDistanceFromPlayer[MAX_ENEMIES];
	int nrOfSpawnedEnemies = 0;
	bool pressMouse1 = false;
	bool holdMouse1 = false;

	Mix_Music* ambientMusic = NULL;
};
typedef struct game Game;

struct player
{
	float verticalFieldOfView = 70;
	float x = 2;
	float y = 2;
	float viewAngle = 0.54;
	float dirX = -1;
	float dirY = 0;
	float planeX = 0.f;
	float planeY = 0.9f;
	float height = 0.5f;

	float sensitivity = 30.f;
	float fieldOfView = 90.f;
	float movementSpeed = 2.f;
	bool useMouse = true;

	int equippedWeapon = 0;
	int health = 100;
	Uint64 lastTimeFiredGun = 0;

	Uint32 crosshairColor = 0xFF0000FF;
	int crosshairSize = 20;
	int crosshairWidth = 2;

	float bobbingPct = 0;
	float bobbingY = 0;
};
typedef struct player Player;