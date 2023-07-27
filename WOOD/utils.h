#pragma once

#include <math.h>
#include <stdio.h>
#include <SDL.h>

#define PI 3.14159265358979323846264338327950288
#define TWO_PI 6.283185307179586
#define DEG2RAD 0.017453292519943295
#define RAD2DEG 57.29577951308232
#define HALF_PI 1.5707963267948966
#define THREEHALFS_PI 4.71238898038469
#define THREEFOURTHS_PI 2.356194490192345
#define ONEFOURTH_PI 0.7853981633974483

float clampf(float min, float max, float v);
float nearestToInt(float xPos, float yPos);
bool arrContains(int* arr, int b, int arrSize);
float naiveLerpf(float a, float b, float t);
float minZero(float a);
float dot2D(float aX, float aY, float bX, float bY);
float vec2DMagnitude(float aX, float aY);
float isOnRight(float aX, float aY, float bX, float bY);
float angToSquare(float ang);
Uint32 BGRA2RGBA(Uint32 c);