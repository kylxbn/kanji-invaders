#pragma once

#include "Enemy.h"
#include "LTexture.h"
#include "Boss.h"

class HomingMissile
{
public:


	HomingMissile(SDL_Window * w, SDL_Renderer * r, int x, int y, Enemy* e[], int* es, Boss* b);
	~HomingMissile();
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
	Enemy* target;
	Enemy** enemyList;
	int* enemyCount;
	Boss* boss;
};