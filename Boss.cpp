#include <SDL.h>
#include <SDL_mixer.h>
#include <cstdlib>
#include <cmath>
#include <string>

#include "Boss.h"

Boss::Boss(SDL_Window* w, SDL_Renderer* r, Mix_Chunk* explosions[])
{
	explosionlist = explosions;
	clank = Mix_LoadWAV("audio/clank.wav");

	notExploded = false;
	exploded = true;

	gBossTexture = new LTexture();
	gBossTexture->Init(w, r);
	badBoss = new LTexture();
	badBoss->Init(w, r);

	type = 0;
	life = 10;

	boom = new LTexture();
	boom->Init(w, r);
	boom->loadFromFile("images/boom.png");

	//Initialize the offsets
	mPosX = 1050;

	//Initialize the velocity
	mVelX = 4;
	mVelY = 0;

}

void Boss::move()
{
	if (!notExploded) return;
	if (mPosX > destx) {
		mPosX-=2;
		return;
	}
	if (mPosY < desty)
	{
		mPosY+=1;
		return;
	}
	if (mPosY > desty)
	{
		mPosY-=1;
		return;
	}
	desty = rand() % 344;
}

void Boss::render()
{
	if (life > halflife)
		gBossTexture->render(mPosX, mPosY);
	else
		badBoss->render(mPosX, mPosY);
	if (notExploded) return;
	for (int i = 0; i < boomcount; i++)
	{
		boom->render(rand() % 512 - 256 + mPosX, rand() % 512 - 256 + mPosY);
	}
	boomcount -= 5;
	if (boomcount % 100 == 0)
		Mix_PlayChannel(-1, explosionlist[rand() % 14], 0);
	if (boomcount == 0)
	{
		exploded = true;
		notExploded = false;
	}
}

Boss::~Boss()
{
	gBossTexture->free();
	badBoss->free();
	boom->free();
	Mix_FreeChunk(clank);

	explosionlist = NULL;
}

int Boss::getX() { return mPosX; }

int Boss::getY() { return mPosY; }

void Boss::setTargetted(bool v)
{
	targetted = v;
}

bool Boss::getTargetted() { return targetted; }

void Boss::explode()
{
	if (exploded) return;
	life--;
	Mix_PlayChannel(-1, clank, 0);
	if (life == 0)
	{
		notExploded = false;
		exploded = false;
		targetted = false;
	}
}

bool Boss::hasExploded()
{
	return exploded;
}

bool Boss::isNotExploding()
{
	return notExploded;
}

int Boss::getLife()
{
	return life;
}

void Boss::loadNew(std::string fn, std::string fn2, int l, int t)
{
	life = l;
	halflife = l / 2;
	if (!gBossTexture->loadFromFile(fn))
	{
		printf("Failed to load Boss texture!\n");
	}
	if (!badBoss->loadFromFile(fn2))
	{
		printf("Failed to load Boss texture!\n");
	}
	type = t;
	mPosX = 1050;
	mPosY = 300 - 128;
	notExploded = true;
	exploded = false;
	boomcount = 1000;

	destx = 760;
	desty = rand() % 344;
}