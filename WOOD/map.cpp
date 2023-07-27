#include "map.h"

int getWallTexture(int x, int y, Map* m)
{
	if (x < 0 || x >= (*m).MAP_WIDTH || y < 0 || y >= (*m).MAP_HEIGHT) return 0;
	int v = ((*m).mapArr[x + y * (*m).MAP_WIDTH] & 0x000000FF);
	return v;
}

int getFloorTexture(int x, int y, Map* m)
{
	if (x < 0 || x >= (*m).MAP_WIDTH || y < 0 || y >= (*m).MAP_HEIGHT) return 72;
	int v = (((*m).mapArr[x + y * (*m).MAP_WIDTH] & 0x0000FF00) >> 8) - 1;
	return (v <= 0 ? 47 : v);
}

int getCeilingTexture(int x, int y, Map* m)
{
	if (x < 0 || x >= (*m).MAP_WIDTH || y < 0 || y >= (*m).MAP_HEIGHT) return 0;
	int v = (((*m).mapArr[x + y * (*m).MAP_WIDTH] & 0x00FF0000) >> 16) - 1;
	return (v <= 0 ? 71 : v);
}

int getInteraction(int x, int y, Map* m)
{
	if (x < 0 || x >= (*m).MAP_WIDTH || y < 0 || y >= (*m).MAP_HEIGHT) return 0;
	int v = (((*m).mapArr[x + y * (*m).MAP_WIDTH] & 0xFF000000) >> 24) - 1;
	return (v < 0 ? 0 : v);
}

void loadMap(const char* mapName, Player* p, Map* m)
{
	Uint32 x;

	std::ifstream mapFile;
	mapFile.open(std::string("assets/maps/") + std::string(mapName) + std::string(".duw"));
	if (!mapFile) 
	{
		printf("Unable to open file\n");
	}

	int c = -1;
	while (mapFile >> x)
	{
		c++;
		if (c == 0) { (*p).y			= x	+ 0.5f;		continue; }
		if (c == 1) { (*p).x			= x	+ 0.5f;		continue; }
		if (c == 2) { (*m).MAP_WIDTH	= x;			continue; }
		if (c == 3) { (*m).MAP_HEIGHT	= x;			continue; }

		int i = c - 4;
		(*m).mapArr[i] = x;
	}
}