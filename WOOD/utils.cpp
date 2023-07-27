#include "utils.h"

float clampf(float min, float max, float v)
{
	if (v >= min && v <= max)
	{
		return v;
	}
	else if (v < min)
	{
		return min;
	}
	return max;
}

float nearestToInt(float xPos, float yPos)
{
	if (fabsf(xPos - roundf(xPos)) < fabsf(yPos - roundf(yPos))) return yPos - (int)yPos;
	return xPos - (int)xPos;
}

bool arrContains(int* arr, int b, int arrSize)
{
	for (int i = 0; i < arrSize; i++)
	{
		if (arr[i] == b) return true;
	}
	return false;
}

float naiveLerpf(float a, float b, float t)
{
	return a + t * (b - a);
}

float minZero(float a)
{
	return a > 0 ? a : 0;
}

float dot2D(float aX, float aY, float bX, float bY)
{
	return aX * bX + aY * bY;
}

float isOnRight(float aX, float aY, float bX, float bY)
{
	float dot = aX * -bY + aY * bX;
	if (dot > 0) return 0;
	return -1;
}

float angToSquare(float ang)
{
	float s = sin(ang);
	float c = cos(ang);
	if (fabsf(c) > fabsf(s)) { s = s / c; c = 1; }
	else { c = c / s; s = 1; }
	return sqrtf(s * s + c * c);
}

float cross2D(float aX, float aY, float bX, float bY)
{
	return aX * bY - aY * bY;
}

float vec2DMagnitude(float aX, float aY)
{
	return sqrtf(aX * aX + aY * aY);
}


Uint32 BGRA2RGBA(Uint32 c)
{
	return ((c & 0x0000FF00) << 16) + ((c & 0xFF000000) >> 16) + ((c & 0x00FF0000)) + (c & 0x000000FF);
}