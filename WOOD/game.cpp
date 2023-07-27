#include "game.h"


void rayCastWalls(Player* p, Game* g, SDLVariables* sdl, TextureMap* tex, Map* m, Uint32* surfaceEnemies)
{
	for (int x = 0; x < WIDTH; x++)
	{
		float cameraX = 2 * x / float(WIDTH) - 1;
		float rayDirX = (*p).dirX + (*p).planeX * cameraX;
		float rayDirY = (*p).dirY + (*p).planeY * cameraX;

		int mapX = (int)(*p).x;
		int mapY = (int)(*p).y;

		float deltaDistX = fabsf(1.f / rayDirX);
		float deltaDistY = fabsf(1.f / rayDirY);
		float perpWallDist;

		int stepX;
		int stepY;

		int hit = 0;
		int side;

		float sideDistX, sideDistY;

		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = ((*p).x - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0f - (*p).x) * deltaDistX;
		}

		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = ((*p).y - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0f - (*p).y) * deltaDistY;
		}

		while (hit == 0)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}

			if (getWallTexture(mapX, mapY, m) > 0) hit = 1;
		}

		if (side == 0)
		{
			perpWallDist = (sideDistX - deltaDistX);
		}
		else
		{
			perpWallDist = (sideDistY - deltaDistY);
		}

		int lineHeight = (int)(HEIGHT / perpWallDist);

		int drawStart = (-lineHeight / 2 + HEIGHT / 2);
		if (drawStart < 0)
			drawStart = 0;

		int drawEnd = (lineHeight / 2 + HEIGHT / 2);
		if (drawEnd >= HEIGHT)
			drawEnd = HEIGHT - 1;

		int texNum = getWallTexture(mapX, mapY, m) - 1;

		float wallX;
		if (side == 0)
		{
			wallX = (*p).y + perpWallDist * rayDirY;
		}
		else
		{
			wallX = (*p).x + perpWallDist * rayDirX;
		}
		wallX -= floorf(wallX);

		int texX = (int)(wallX * TEXTURESIZE);

		float step = 1.0 * TEXTURESIZE / lineHeight;
		float texPos = ((drawStart - HEIGHT / 2 + lineHeight / 2) * step);

		for (int y = 0; y < HEIGHT; y++)
		{
			if (y >= drawStart && y <= drawEnd)
			{
				int texY = (int)texPos & (TEXTURESIZE - 1);
				texPos += step;
				Uint32 color = colorFromTexture(tex, texNum, texX, texY);
				color = color & 0xFFFFFF00 + 0xFF;
				//printf("%x %d %d %d\n", (*tex).textureMap[0 * TEXTURESIZE * TEXTURESIZE + 0 + 0 * TEXTURESIZE], 0, 0, 0);

				setPixelColor(sdl, x, y, color);
			}

			Uint32 enemyColor = surfaceEnemies[y * WIDTH + x];
			int index = enemyColor & 0xFF;

			if (index >= 128) continue;

			if ((enemyColor & 0xFFFFFF00) != 0 && (*g).enemyDistanceFromPlayer[index] < perpWallDist)
			{
				enemyColor = (enemyColor & 0xFFFFFF00) + 0xFF;
				setPixelColor(sdl, x, y, enemyColor);
			}
		}
	}
}

void rayCastFloor(Player* p, Game* g, SDLVariables* sdl, TextureMap* tex, Map* m, Uint32* surfaceEnemies)
{
	for (int y = 0; y < HEIGHT; y++)
	{
		float rayDirX0 = (*p).dirX - (*p).planeX;
		float rayDirY0 = (*p).dirY - (*p).planeY;
		float rayDirX1 = (*p).dirX + (*p).planeX;
		float rayDirY1 = (*p).dirY + (*p).planeY;

		float rowDistance = (0.5f * HEIGHT) / (y - (HEIGHT / 2.f));

		float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / WIDTH;
		float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / WIDTH;

		float floorX = (*p).x + rowDistance * rayDirX0;
		float floorY = (*p).y + rowDistance * rayDirY0;

		for (int x = 0; x < WIDTH; x++)
		{
			int cellX = (int)floorX;
			int cellY = (int)floorY;

			int tx = (int)(TEXTURESIZE * (floorX - cellX)) & (TEXTURESIZE - 1);
			int ty = (int)(TEXTURESIZE * (floorY - cellY)) & (TEXTURESIZE - 1);

			floorX += floorStepX;
			floorY += floorStepY;

			int floorTextureId = getFloorTexture(cellX, cellY, m);
			int ceilingTextureId = getCeilingTexture(cellX, cellY, m);

			//printf("%d\n", floorTextureId);

			Uint32 color = colorFromTexture(tex, floorTextureId, tx, ty);
			color = color & 0xFFFFFF00 + 0xFF;
			setPixelColor(sdl, x, y, color);

			color = colorFromTexture(tex, ceilingTextureId, tx, ty);
			color = color & 0xFFFFFF00 + 0xFF;
			setPixelColor(sdl, x, HEIGHT - y - 1, color);
		}
	}
}

void rayCastingVectors(Player* p, Game* g, SDLVariables* sdl, TextureMap* tex, Map* m)
{
	(*sdl).surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, CHANNELS * 8, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	
	Uint32* surfaceEnemies = updateEnemies(g, p, sdl);
	rayCastFloor(p, g, sdl, tex, m, surfaceEnemies);
	rayCastWalls(p, g, sdl, tex, m, surfaceEnemies);

	SDL_FreeSurface((*sdl).surface);
	
	(*sdl).surface = SDL_CreateRGBSurfaceFrom((void*)(*sdl).pixels, WIDTH, HEIGHT, CHANNELS * 8, WIDTH * CHANNELS, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
}

void rayCastingAngle(Player* p, Game* g, SDLVariables* sdl, TextureMap* tex, Map* m)
{
	float verticalFieldOfView = ((*p).verticalFieldOfView * DEG2RAD);
	float halfVerticalFieldOfView = verticalFieldOfView / 2.f;

	(*sdl).surface = SDL_CreateRGBSurfaceFrom((void*)(*sdl).pixels, WIDTH, HEIGHT, CHANNELS * 8, WIDTH * CHANNELS, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

	float fieldOfView = ((*p).fieldOfView * DEG2RAD);
	float halfFieldOfView = fieldOfView / 2.f;



	for (int x = 0; x < WIDTH; x++)
	{
		float rayAngle = (fieldOfView * x / WIDTH) - halfFieldOfView;
		float xVector = cosf((*p).viewAngle + rayAngle);
		float yVector = sinf((*p).viewAngle + rayAngle);

		float xPos = (*p).x;
		float yPos = (*p).y;
		float dist;
		const char* textureID = "";
		int blockade = -1;
		for (dist = 0; dist < (*g).maxSteps; dist++)
		{
			xPos = clampf(0, (*m).MAP_WIDTH, xPos + xVector * (*g).stepSize);
			yPos = clampf(0, (*m).MAP_HEIGHT, yPos + yVector * (*g).stepSize);

			blockade = getWallTexture((int)xPos, (int)yPos, m);
			if (blockade > 0 && blockade < 0x81)
			{
				break;
			}
		}

		if (blockade == -1) continue;

		float pct = nearestToInt(xPos, yPos);
		//printf("%f %f %d\n", xPos, yPos, (int)roundf(pct * 64.f));

		float wallHeight = 0x40 * (*g).maxSteps / (dist * cosf(rayAngle));

		SDL_Rect a; a.x = x; a.y = (HEIGHT - wallHeight) / 2.f; a.w = 1; a.h = wallHeight;
		if (SDL_BlitScaled((*tex).texture, getTextureRectWall(pct, NULL, blockade), (*sdl).surface, &a) < 0)
		{
			printf("%s\n", SDL_GetError());
		}

		for (int y = 0; y < HEIGHT; y++)
		{
			if (y >= a.y + a.h)
				setPixelColor(sdl, x, y, 163, 156, 119, 255);
			else if (y <= a.y)
				setPixelColor(sdl, x, y, 135, 206, 250, 255);
		}

		/*for (float i = 0; i < HEIGHT; i++)
		{
			float a = i / HEIGHT;
			float distancePct = dist / (*g).maxSteps;

			if ((1 - distancePct) > a)
			{
				setPixelColor(sdl, x, i, 255, 255, 255, 200 * (1 - distancePct));
			}
			else
			{
				setPixelColor(sdl, x, i, 255, 0, 0, 200);
			}

			//SDL_BlitSurface((*tex).texture, getTextureRect("brick_wall_0"), (*sdl).surface, getTextureRect("brick_wall_0"));

		}*/

	}
}

