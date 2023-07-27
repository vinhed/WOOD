#include "texture_mapper.h"

#define NROFTEXTURES 12

const char* textureNames[NROFTEXTURES] = {
		"brick_wall_0", "brick_wall_1", "brick_wall_2", "brick_wall_3", "nazi_flag_0", "nazi_flag_1",
		"hitler_painting_0", "hitler_painting_1", "brick_gate_blue_0", "brick_gate_blue_1", "eagle_statue_0", "eagle_statue_1"
};

Uint32 colorFromTexture(TextureMap* tex, int i, int x, int y)
{
	return (*tex).textureMap[i * TEXTURESIZE * TEXTURESIZE + x + y * TEXTURESIZE];
}

Uint32 pixelColor(char* pixels, int index)
{
	return (pixels[index * 3] << 24) + (pixels[index * 3 + 1] << 16) + (pixels[index * 3 + 2] << 8) + 0xFF;
}

Uint32 getpixel(SDL_Surface* surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp)
	{
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16*)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32*)p;
		break;

	default:
		return 0;
	}
}

void preloadAllTextures(TextureMap* tex, SDL_Renderer* renderer)
{
	int brightness = 0xC4;

	(*tex).texture = SDL_LoadBMP("assets/map_textures.bmp");

	for (int i = 0; i < TEXTUREWIDTH; i++)
	{
		for (int y = 0; y < TEXTURESIZE; y++)
		{
			for (int x = 0; x < TEXTURESIZE; x++)
			{
				int index = i * TEXTURESIZE + x + y * TEXTUREWIDTH * TEXTURESIZE;
				
				Uint32 color = getpixel((*tex).texture, x + i * TEXTURESIZE, y);
				Uint8 r, g, b;
				SDL_GetRGB(color, (*tex).texture->format, &r, &g, &b);
				color = (r << 24) + (g << 16) + (b << 8) + brightness;
				
				(*tex).textureMap[i * TEXTURESIZE * TEXTURESIZE + x + y * TEXTURESIZE] = color;
			}
		}
	}

	/*for (int i = 0; i < TEXTURESIZE * TEXTURESIZE * TEXTUREWIDTH; i++)
	{
		//printf("%d\n", (*tex).textureMap[i]);
	}*/
}

int findTexture(const char* name)
{
	for (int i = 0; i < NROFTEXTURES; i++)
	{
		if (name == textureNames[i])
		{
			return i;
		}
	}
	return -1;
}

SDL_Rect* getTextureRectWall(float pct, const char* name, int textureID)
{
	int index = textureID;
	if(textureID == -1) index = findTexture(name);

	SDL_Rect rect;
	rect.x = 64; rect.y = 64; rect.w = 128; rect.h = 128;
	rect.x = (int)roundf(pct * TEXTURESIZE + (index % 6) * TEXTURESIZE); rect.y = (int)(index / 6) * TEXTURESIZE; rect.w = 1; rect.h = TEXTURESIZE;
	return &rect;
}
