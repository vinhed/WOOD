#include "controls.h"


void walk(Player* p, float offset, Game* g, float movementSpeed, float* moveX, float* moveY, Map* m)
{
	float mX = cosf((*p).viewAngle + offset) * movementSpeed;
	float mY = sinf((*p).viewAngle + offset) * movementSpeed;

	float posX = mX + (*p).x;
	float posY = mY + (*p).y;

	float wallOffset = 0.0f;

	if (getWallTexture((int)(posX + wallOffset), (int)((*p).y), m) > 0 && getWallTexture((int)(posX + wallOffset), (int)((*p).y), m) < 0xFF) mX = 0;
	if (getWallTexture((int)(posX - wallOffset), (int)((*p).y), m) > 0 && getWallTexture((int)(posX - wallOffset), (int)((*p).y), m) < 0xFF) mX = 0;
	(*moveX) += mX;

	if (getWallTexture((int)((*p).x), (int)(posY + wallOffset), m) > 0 && getWallTexture((int)((*p).x), (int)(posY + wallOffset), m) < 0xFF) mY = 0;
	if (getWallTexture((int)((*p).x), (int)(posY - wallOffset), m) > 0 && getWallTexture((int)((*p).x), (int)(posY - wallOffset), m) < 0xFF) mY = 0;
	(*moveY) += mY;
}

void input(Player* p, Game* g, Map* m)
{
	Uint32 internalTimerLAST = 0;

	while ((*g).isRunning)
	{
		if (SDL_GetTicks() - internalTimerLAST < 10) continue;
		internalTimerLAST = SDL_GetTicks();

		if ((*p).useMouse)
		{
			int mX = 0, mY = 0;
			Uint32 buttons = SDL_GetRelativeMouseState(&mX, &mY);

			if (SDL_BUTTON(buttons) & 1 && !(*g).pressMouse1 && !(*g).holdMouse1)
			{
				(*g).pressMouse1 = true;
			}
			else if (SDL_BUTTON(buttons) & 1)
			{
				(*g).pressMouse1 = false;
			}

			if (SDL_BUTTON(buttons) & 1)
			{
				(*g).holdMouse1 = true;
			}
			else
			{
				(*g).holdMouse1 = false;
			}

			float rotationSpeed = -(*p).sensitivity * mX;

			float oldDirX = (*p).dirX;
			(*p).dirX = (*p).dirX * cosf(rotationSpeed) - (*p).dirY * sinf(rotationSpeed);
			(*p).dirY = oldDirX * sinf(rotationSpeed) + (*p).dirY * cosf(rotationSpeed);
			float oldPlaneX = (*p).planeX;
			(*p).planeX = (*p).planeX * cosf(rotationSpeed) - (*p).planeY * sinf(rotationSpeed);
			(*p).planeY = oldPlaneX * sinf(rotationSpeed) + (*p).planeY * cosf(rotationSpeed);

			(*p).viewAngle = atan2f((*p).dirY, (*p).dirX) + M_PI;
		}

		const Uint8* keystate = SDL_GetKeyboardState(NULL);

		float movementSpeed = (*p).movementSpeed * (keystate[SDL_SCANCODE_LSHIFT] ? 1.5f : 1);
		float moveX = 0, moveY = 0;

		if (keystate[SDL_SCANCODE_ESCAPE])
		{
			(*g).isRunning = false;
		}

		if (keystate[SDL_SCANCODE_1])
		{
			(*p).equippedWeapon = 0;
		}
		if (keystate[SDL_SCANCODE_2])
		{
			(*p).equippedWeapon = 1;
		}
		if (keystate[SDL_SCANCODE_3])
		{
			(*p).equippedWeapon = 2;
		}
		if (keystate[SDL_SCANCODE_RIGHT])
		{
			(*p).viewAngle += (*p).sensitivity;
		}
		if (keystate[SDL_SCANCODE_LEFT])
		{
			(*p).viewAngle -= (*p).sensitivity;
		}
		if (keystate[SDL_SCANCODE_W])
		{
			walk(p, -PI, g, movementSpeed, &moveX, &moveY, m);
		}
		if (keystate[SDL_SCANCODE_A])
		{
			walk(p, -HALF_PI, g, movementSpeed, &moveX, &moveY, m);
		}
		if (keystate[SDL_SCANCODE_S])
		{
			walk(p, 0, g, movementSpeed, &moveX, &moveY, m);
		}
		if (keystate[SDL_SCANCODE_D])
		{
			walk(p, THREEHALFS_PI - PI, g, movementSpeed, &moveX, &moveY, m);
		}

		if (fabsf(moveX) > 0 || fabsf(moveY) > 0)
		{
			(*p).bobbingPct = 1;
		}
		else
		{
			(*p).bobbingPct -= 2.5f * (*g).deltaTime;
		}
		(*p).bobbingPct = clampf(0, 1.f, (*p).bobbingPct);

		float bobbing = (*p).bobbingPct * (*g).weapons[(*p).equippedWeapon].weaponBobbingAmount;
		(*p).bobbingY = sinf(SDL_GetTicks64() / ((*g).weapons[(*p).equippedWeapon].weaponBobbingCycleSpeed)) * bobbing * (keystate[SDL_SCANCODE_LSHIFT] ? 1.5f : 1);

		float moveSqr = moveX * moveX + moveY * moveY;
		if (moveSqr > movementSpeed * movementSpeed)
		{
			float magnitude = sqrtf(moveSqr);
			float mul = movementSpeed / magnitude;
			moveX *= mul;
			moveY *= mul;
		}

		(*p).x += moveX;
		(*p).y += moveY;
	}
}