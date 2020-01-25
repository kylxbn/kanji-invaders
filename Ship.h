#pragma once

#include "LTexture.h"
#include "Enemy.h"
#include "EnemyMissile.h"
#include "Boss.h"

class Ship
{
public:
	static const int SHIP_WIDTH = 32;
	static const int SHIP_HEIGHT = 32;

	Ship(SDL_Window* w, SDL_Renderer* r, Enemy* el[], int* ec, EnemyMissile* em[], int* EMSize, Boss* boss);
	~Ship();
	void handleEvent(SDL_Event& e);
	void move();
	void render();
	int getX();
	int getY();
	bool hit = false;
private:
	int mPosX, mPosY;
	int mVelX, mVelY;
	int SHIP_VEL = 4;

	Enemy** enemyList;
	int* enemyCount;
	EnemyMissile** missileList;
	int* missileCount;
	LTexture* gShipTexture[10];
	int currentFrame;
	int invincible = 100;
	bool draw = false;
	Boss* boss;
};