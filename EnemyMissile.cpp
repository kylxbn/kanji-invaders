#include <SDL.h>
#include <cmath>
#include <vector>

#include "EnemyMissile.h"

EnemyMissile::EnemyMissile(SDL_Window* w, SDL_Renderer* r, int x, int y, int tx, int ty)
{
	angle = 0;
	tarx = tx;
	tary = ty;
	//Initialize the offsets
	mPosX = static_cast<float>(x + 126);
	mPosY = static_cast<float>(y + 32);

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;
	speed = 5;

	gMissileTexture = new LTexture();
	gMissileTexture->Init(w, r);
	if (!gMissileTexture->loadFromFile("images/enemybullet.png", 0, 0, 0))
	{
		printf("Failed to load missile texture!\n");
	}

	float realangle = atan2f(tary - mPosY + 32, tarx - mPosX + 126);
	/*if (angle - realangle > 0.1f) angle += 0.1f;
	else if (angle - realangle < -0.1f) angle -= 0.1f;
	else angle = realangle;*/
	angle = realangle;
	mVelX = cos(angle)*speed;
	mVelY = sin(angle)*speed;
}


void EnemyMissile::move()
{
	mPosX += mVelX;
	mPosY += mVelY;
}

int EnemyMissile::getX()
{
	return static_cast<int>(mPosX);
}

int EnemyMissile::getY()
{
	return mPosY;
}

void EnemyMissile::render()
{
	gMissileTexture->render(mPosX, mPosY);
}

EnemyMissile::~EnemyMissile()
{
	gMissileTexture->free();
}
