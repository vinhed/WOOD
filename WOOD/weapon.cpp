#include "weapon.h"

void loadAllWeapons(Game* g, TextureMap* tex)
{
	(*tex).fistsTexture = SDL_LoadBMP("assets/weapons/fists.bmp");
	(*tex).weaponTexture = SDL_LoadBMP("assets/weapons/weapons.bmp");
	(*tex).muzzelFlashTextures = SDL_LoadBMP("assets/weapons/muzzel_flash.bmp");


	// Fists
	(*g).weapons[0].damage = 20;
	(*g).weapons[0].range = 0.25f;
	(*g).weapons[0].name = "Fists of Doom";
	(*g).weapons[0].ammo = 0;
	(*g).weapons[0].automatic = false;
	(*g).weapons[0].muzzelFlashSrc = { 0 , 0 , 0 , 0 };
	(*g).weapons[0].muzzelFlashDst = { 0 , 0 , 0 , 0 };
	(*g).weapons[0].src = { 165 , 0 , 152 , 86 };
	(*g).weapons[0].dst = { WIDTH / 2 + 100 , HEIGHT - 165 , 454 , 300 };
	(*g).weapons[0].fireAnimation = { WIDTH / 2 - 100, HEIGHT - 400 , 600 , 400 };
	(*g).weapons[0].animationDelay = 150;
	(*g).weapons[0].fireSound = Mix_LoadWAV("assets/sound/punch.wav");
	(*g).weapons[0].cooldown = 300;
	(*g).weapons[0].textureId = 0;
	(*g).weapons[0].volume = 0.25f;
	(*g).weapons[0].weaponBobbingAmount = 10.f;
	(*g).weapons[0].weaponBobbingCycleSpeed = 75.f;


	// Pistol
	(*g).weapons[1].damage = 10;
	(*g).weapons[1].range = 4096;
	(*g).weapons[1].name = "Glock-17";
	(*g).weapons[1].ammo = 35;
	(*g).weapons[1].automatic = false;
	(*g).weapons[1].muzzelFlashSrc = { 296 , 14 , 81 , 47 };
	(*g).weapons[1].muzzelFlashDst = { WIDTH / 2 + 50 , HEIGHT - 310 , 122 , 70 };
	(*g).weapons[1].src = { 300 , 277 , 100 , 100 };
	(*g).weapons[1].dst = { WIDTH / 2 + 100 , HEIGHT - 275 , 300 , 300 };
	(*g).weapons[1].fireAnimation = { WIDTH / 2 + 100 , HEIGHT - 325 , 350 , 350 };
	(*g).weapons[1].animationDelay = 100;
	(*g).weapons[1].fireSound = Mix_LoadWAV("assets/sound/pistol.wav");
	(*g).weapons[1].cooldown = 250;
	(*g).weapons[1].textureId = 1;
	(*g).weapons[1].volume = 1.f;
	(*g).weapons[1].weaponBobbingAmount = 10.f;
	(*g).weapons[1].weaponBobbingCycleSpeed = 100.f;


	// AK-47
	(*g).weapons[2].damage = 18;
	(*g).weapons[2].range = 8192;
	(*g).weapons[2].name = "AK-47";
	(*g).weapons[2].ammo = 350;
	(*g).weapons[2].automatic = true;
	(*g).weapons[2].muzzelFlashSrc = { 296 , 14 , 81 , 47 };
	(*g).weapons[2].muzzelFlashDst = { WIDTH / 2 + 100 , HEIGHT - 255 , 122 , 70 };
	(*g).weapons[2].src = { 530 , 400 , 212 , 148 };
	(*g).weapons[2].dst = { WIDTH / 2 + 50 , HEIGHT - 275 , 537, 375 };
	(*g).weapons[2].fireAnimation = { WIDTH / 2 + 50 , HEIGHT - 300 , 573 , 400 };
	(*g).weapons[2].animationDelay = 75;
	(*g).weapons[2].fireSound = Mix_LoadWAV("assets/sound/ak47.wav");
	(*g).weapons[2].cooldown = 125;
	(*g).weapons[2].textureId = 1;
	(*g).weapons[2].volume = 0.35f;
	(*g).weapons[2].weaponBobbingAmount = 10.f;
	(*g).weapons[2].weaponBobbingCycleSpeed = 125.f;
}

void fireGun(Player* p, Game* g)
{
	if ((*p).lastTimeFiredGun + (*g).weapons[(*p).equippedWeapon].cooldown > SDL_GetTicks64()) return;
	if ((*p).equippedWeapon != 0)
	{
		if ((*g).weapons[(*p).equippedWeapon].ammo == 0) return;
		(*g).weapons[(*p).equippedWeapon].ammo--;
	}

	(*p).lastTimeFiredGun = SDL_GetTicks64();
	Mix_Volume(1, (*g).weapons[(*p).equippedWeapon].volume * MIX_MAX_VOLUME);
	Mix_PlayChannel(1, (*g).weapons[(*p).equippedWeapon].fireSound, 0);
}

void loadWeapons(Player* p, Game* g, SDLVariables* sdl, TextureMap* tex)
{
	if ((*g).weapons[(*p).equippedWeapon].automatic && (*g).holdMouse1)
	{
		fireGun(p, g);
	}
	else if (!(*g).weapons[(*p).equippedWeapon].automatic && (*g).pressMouse1)
	{
		fireGun(p, g);
	}

	float lerpedValue = ((*p).lastTimeFiredGun + (*g).weapons[(*p).equippedWeapon].animationDelay - SDL_GetTicks64()) / (float)(*g).weapons[(*p).equippedWeapon].animationDelay;
	lerpedValue = lerpedValue <= 1 ? lerpedValue : 0;
	SDL_Rect r = (*g).weapons[(*p).equippedWeapon].dst;
	if (lerpedValue > 0)
	{
		r.x = naiveLerpf((*g).weapons[(*p).equippedWeapon].fireAnimation.x, (*g).weapons[(*p).equippedWeapon].dst.x, 1.f - lerpedValue);
		r.y = naiveLerpf((*g).weapons[(*p).equippedWeapon].fireAnimation.y, (*g).weapons[(*p).equippedWeapon].dst.y, 1.f - lerpedValue);
		r.w = naiveLerpf((*g).weapons[(*p).equippedWeapon].fireAnimation.w, (*g).weapons[(*p).equippedWeapon].dst.w, 1.f - lerpedValue);
		r.h = naiveLerpf((*g).weapons[(*p).equippedWeapon].fireAnimation.h, (*g).weapons[(*p).equippedWeapon].dst.h, 1.f - lerpedValue);

		SDL_BlitScaled((*tex).muzzelFlashTextures, &(*g).weapons[(*p).equippedWeapon].muzzelFlashSrc, (*sdl).surface, &(*g).weapons[(*p).equippedWeapon].muzzelFlashDst);
	}
	else
	{
		r.y += (*p).bobbingY;
	}

	SDL_BlitScaled((((*g).weapons[(*p).equippedWeapon].textureId == 0) ? (*tex).fistsTexture : (*tex).weaponTexture), &(*g).weapons[(*p).equippedWeapon].src, (*sdl).surface, &r);
}