#include <SDL.h>
#include <SDL_mixer.h>
#include <cstdlib>
#include <cmath>

#include "Enemy.h"

Enemy::Enemy(SDL_Window* w, SDL_Renderer* r, Mix_Chunk* explosions[])
{
	explosionlist = explosions;
	notExploded = true;
	exploded = false;

	std::string fnames[] = { "images/kanji1.png", "images/kanji2.png", "images/kanji3.png",
		"images/kanji4.png", "images/kanji5.png", "images/kanji6.png",
		"images/kanji7.png", "images/kanji8.png", "images/kanji9.png",
		"images/kanji10.png" };

	gEnemyTexture = new LTexture();
	gEnemyTexture->Init(w, r);
	if (!gEnemyTexture->loadFromFile(fnames[rand() % 10]))
	{
		printf("Failed to load enemy texture!\n");
	}

	boom = new LTexture();
	boom->Init(w, r);
	boom->loadFromFile("images/boom.png");

	//Initialize the offsets
	mPosX = 1050;
	realY = rand() % 700 - 50;

	//Initialize the velocity
	mVelX = 4;
	mVelY = 0;

}

void Enemy::move()
{
	if (!notExploded) return;
	mPosY = static_cast<int>(realY + sin(angle) * 64);
	mPosX -= mVelX;
	angle += 0.1f;
}

void Enemy::render()
{
	if (notExploded) gEnemyTexture->render(mPosX, mPosY);
	else
	{
		for (int i = 0; i < boomcount; i++)
		{
			boom->render(rand() % 128 - 64 + mPosX, rand() % 128 - 64 + mPosY);
		}
		boomcount--;
		if (boomcount == 0) exploded = true;
	}
}

Enemy::~Enemy()
{
	gEnemyTexture->free();
	boom->free();

	explosionlist = NULL;
}

int Enemy::getX() { return mPosX; }

int Enemy::getY() { return mPosY; }

void Enemy::setTargetted(bool v)
{
	targetted = v;
}

bool Enemy::getTargetted() { return targetted; }

void Enemy::explode()
{
	notExploded = false;
	exploded = false;
	targetted = false;

	Mix_PlayChannel(-1, explosionlist[rand()%14], 0);
}

bool Enemy::hasExploded()
{
	return exploded;
}

bool Enemy::isNotExploding()
{
	return notExploded;
}