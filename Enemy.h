#pragma once
#include <SDL_mixer.h>

#include "LTexture.h"

class Enemy
{
public:
	Enemy(SDL_Window* w, SDL_Renderer* r, Mix_Chunk* explosions[]);
	~Enemy();
	void move();
	void render();
	int getX();
	int getY();
	void setTargetted(bool v);
	bool getTargetted();
	void explode();
	bool hasExploded();
	bool isNotExploding();
private:
	bool notExploded;
	bool exploded;
	int ENEMY_WIDTH = 64;
	int ENEMY_HEIGHT = 64;
	int LIFE = 100;
	int realY;
	int mPosX, mPosY;
	int mVelX, mVelY;
	LTexture* gEnemyTexture;
	LTexture* boom;
	Mix_Chunk** explosionlist;
	int boomcount = 20;
	bool targetted;
	float angle;
};