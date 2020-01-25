#pragma once
#include <SDL_mixer.h>
#include <string>

#include "LTexture.h"

class Boss
{
public:
	Boss(SDL_Window* w, SDL_Renderer* r, Mix_Chunk* explosions[]);
	~Boss();
	void move();
	void render();
	int getX();
	int getY();
	void setTargetted(bool v);
	bool getTargetted();
	void explode();
	bool hasExploded();
	bool isNotExploding();
	int getLife();
	void loadNew(std::string fn, std::string fn2, int l, int t);
private:
	bool notExploded;
	bool exploded = false;
	int Boss_WIDTH = 64;
	int Boss_HEIGHT = 64;
	int realY;
	int mPosX, mPosY;
	int mVelX, mVelY;
	LTexture* gBossTexture;
	LTexture* badBoss;
	LTexture* boom;
	int life;
	int halflife;
	Mix_Chunk** explosionlist;
	Mix_Chunk* clank;
	int boomcount = 1000;
	bool targetted;
	float angle;
	int type;

	int desty;
	int destx;
};