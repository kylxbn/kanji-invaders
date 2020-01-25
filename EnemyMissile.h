#pragma once
#include <SDL.h>

#include "LTexture.h"

class EnemyMissile
{
public:
	EnemyMissile(SDL_Window * w, SDL_Renderer * r, int x, int y, int tx, int ty);
	~EnemyMissile();
	void move();
	void render();
	int getX();
	int getY();

	bool deleteMe = false;

private:
	bool straight = true;
	float mPosX, mPosY;
	float mVelX, mVelY;
	float speed;
	float angle;
	LTexture* gMissileTexture;
	int tarx, tary;
};