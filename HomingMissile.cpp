#include <SDL.h>
#include <cmath>
#include <vector>

#include "HomingMissile.h"
#include "Enemy.h"

HomingMissile::HomingMissile(SDL_Window* w, SDL_Renderer* r, int x, int y, Enemy* e[], int* es, Boss* b)
{
	angle = 0;
	enemyList = e;
	enemyCount = es;
	boss = b;
	//Initialize the offsets
	mPosX = static_cast<float>(x+126);
	mPosY = static_cast<float>(y+32);

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
	speed = 5;

	gMissileTexture = new LTexture();
	gMissileTexture->Init(w, r);
	if (!gMissileTexture->loadFromFile("images/bullet.png", 0, 0, 0))
	{
		printf("Failed to load missile texture!\n");
	}

	if (es > 0)
	{
		for (int i = 0; i < *es; i++)
		{
			if (!(e[i]->getTargetted()))
			{
				target = (e[i]);
				i = *es;
				target->setTargetted(true);
			}
		}
	}
	else
	{
		target = NULL;
	}
}


void HomingMissile::move()
{
	if (target == NULL || straight)
	{
		mVelX = 5;
		mVelY = 0;
	}
	else
	{
		float realangle = atan2f(target->getY() - mPosY + 16, target->getX() - mPosX + 16);
		/*if (angle - realangle > 0.1f) angle += 0.1f;
		else if (angle - realangle < -0.1f) angle -= 0.1f;
		else angle = realangle;*/
		angle = realangle;
		mVelX = cos(angle)*speed;
		mVelY = sin(angle)*speed;
	}
	mPosX += mVelX;
	mPosY += mVelY;

	int x, y;

	for (int i = 0; i < *enemyCount; i++)
	{
		x = enemyList[i]->getX();
		y = enemyList[i]->getY();
		if ((mPosX >= x && mPosX < x + 32 && mPosY >= y && mPosY < y + 32) && enemyList[i]->isNotExploding())
		{
			enemyList[i]->explode();
			deleteMe = true;
		}
	}

	x = boss->getX();
	y = boss->getY();
	if ((mPosX >= x && mPosX < x + 256 && mPosY >= y && mPosY < y + 256) && boss->isNotExploding())
	{
		boss->explode();
		deleteMe = true;
	}
}

int HomingMissile::getX()
{
	return static_cast<int>(mPosX);
}

int HomingMissile::getY()
{
	return mPosY;
}

void HomingMissile::render()
{
	gMissileTexture->render(mPosX, mPosY);
}

HomingMissile::~HomingMissile()
{
	target = NULL;
	gMissileTexture->free();
}
