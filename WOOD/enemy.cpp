#include "enemy.h"

void loadAllEnemies(Game* g)
{
	SDL_Rect full = {0, 0, 1280, 1280};

	(*g).enemies[0].damage = 7;
	(*g).enemies[0].startingHealth = 100;
	(*g).enemies[0].name = "Cacodemon";
	SDL_Surface* tmpImage = SDL_LoadBMP("assets/enemies/cacodemon.bmp"); 
	(*g).enemies[0].sprite = SDL_ConvertSurface(tmpImage, tmpImage->format, 0);
	SDL_FreeSurface(tmpImage);
	(*g).enemies[0].width = 0.75f;
	(*g).enemies[0].height = 0.75f;
}

void spawnEnemy(int i, float x, int y, Game* g)
{
	(*g).spawnedEnemies[(*g).nrOfSpawnedEnemies].enemyId = i;
	(*g).spawnedEnemies[(*g).nrOfSpawnedEnemies].health = (*g).enemies[i].startingHealth;
	(*g).spawnedEnemies[(*g).nrOfSpawnedEnemies].x = x;
	(*g).spawnedEnemies[(*g).nrOfSpawnedEnemies].y = y;
	(*g).nrOfSpawnedEnemies++;
}

Uint32* updateEnemies(Game* g, Player* p, SDLVariables* sdl)
{
	SDL_Surface* tmpSurface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 8 * CHANNELS, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	Uint8* pixelDistance = new Uint8[WIDTH * HEIGHT];
	memset(pixelDistance, 0, WIDTH * HEIGHT);
	Uint32* surfacePixels = new Uint32[WIDTH * HEIGHT];

	std::map<float, int> distanceIndexes;
	
	for (int i = 0; i < (*g).nrOfSpawnedEnemies; i++)
	{
		int enemyId = (*g).spawnedEnemies[i].enemyId;

		float vectorX = (*p).x - (*g).spawnedEnemies[i].x;
		float vectorY = (*p).y - (*g).spawnedEnemies[i].y;

		float s = dot2D(vectorX, vectorY, (*p).dirX, (*p).dirY);
		float f = s / (vec2DMagnitude(vectorX, vectorY) * vec2DMagnitude((*p).dirX, (*p).dirY));
		float d = acos(f);

		if (d < 2)
		{
			SDL_Rect r = { 0 };
			(*g).spawnedEnemies[i].screenPos = r;
			continue;
		}

		float v = tan(d);
			
		float offset = isOnRight(vectorX, vectorY, (*p).dirX, (*p).dirY);
		if (offset < 0) v *= offset;
		v *= 0.5f;

		float distance = sqrtf(vectorX * vectorX + vectorY * vectorY) / angToSquare(d);
		float adjustedDistance = distance;

		(*g).enemyDistanceFromPlayer[i] = distance;
		distanceIndexes.insert(std::pair<float, int>(distance, i));

		float width = (*g).enemies[enemyId].height * (HEIGHT / adjustedDistance);
		float height = (*g).enemies[enemyId].height * (HEIGHT / adjustedDistance);

		int xPos = (WIDTH - width) / 2 - v * WIDTH;
		int yPos = (HEIGHT - height) / 2;

		SDL_Rect r = { xPos, yPos, width, height };
		(*g).spawnedEnemies[i].screenPos = r;
	}
	
	
	//printf("----\n");
	for (auto iter = distanceIndexes.rbegin(); iter != distanceIndexes.rend(); ++iter)
	{
		//printf("%d %f\n", iter->second, iter->first);
		SDL_Rect r = (*g).spawnedEnemies[iter->second].screenPos;
		SDL_Rect rZero = { 0, 0, r.w, r.h };

		SDL_BlitScaled((*g).enemies[(*g).spawnedEnemies[iter->second].enemyId].sprite, NULL, (*sdl).surface, &r);
		SDL_BlitSurface((*sdl).surface, &r, tmpSurface, &rZero);
		
		for (int y = r.y; y < r.y + r.h; y++)
		{
			memset(&pixelDistance[y * WIDTH + r.x], iter->second, r.w);
		}
		
	}
	
	memcpy(surfacePixels, (*sdl).surface->pixels, (*sdl).surface->pitch * (*sdl).surface->h);

	for (int i = 0; i < WIDTH * HEIGHT; i++)
	{
		surfacePixels[i] = (surfacePixels[i] & 0xFFFFFF00) + pixelDistance[i];
	}

	SDL_FreeSurface(tmpSurface);
	delete[] pixelDistance;

	return surfacePixels;
}