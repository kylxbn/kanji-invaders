#include <SDL.h>
#include <string>
#include <cstdlib>

#include "Ship.h"
#include "EnemyMissile.h"
#include "Boss.h"

Ship::Ship(SDL_Window* w, SDL_Renderer* r, Enemy* el[], int* ec, EnemyMissile* em[], int* EMSize, Boss* b)
{
	enemyList = el;
	enemyCount = ec;

	missileList = em;
	missileCount = EMSize;

	boss = b;

	currentFrame = 0;

	//Initialize the offsets
	mPosX = 32;
	mPosY = 600 / 2 + 8;

	//Initialize the velocity
	mVelX = 0;
	mVelY = 0;

	std::string fnames[] = { "images/ship1.png", "images/ship2.png", "images/ship3.png",
		"images/ship4.png", "images/ship5.png", "images/ship6.png",
		"images/ship7.png", "images/ship8.png", "images/ship9.png",
		"images/ship10.png" };

	for (int i = 0; i < 10; i++)
	{
		gShipTexture[i] = new LTexture();
		gShipTexture[i]->Init(w, r);
		gShipTexture[i]->loadFromFile(fnames[i]);
	}
}

void Ship::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY = -SHIP_VEL; break;
		case SDLK_DOWN: mVelY = SHIP_VEL; break;
		case SDLK_LEFT: mVelX = -SHIP_VEL; break;
		case SDLK_RIGHT: mVelX = SHIP_VEL; break;
		}
	}
	//If a key was released
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: mVelY = 0; break;
		case SDLK_DOWN: mVelY = 0; break;
		case SDLK_LEFT: mVelX = 0; break;
		case SDLK_RIGHT: mVelX = 0; break;
		}
	}

}

void Ship::move()
{

	mPosX += mVelX;
	mPosY += mVelY;

	if (mPosX < 0) mPosX = 0;
	if (mPosX > 1024) mPosX = 1024;
	if (mPosY < -12) mPosY = -12;
	if (mPosY > 560) mPosY = 560;

	if (invincible > 0)
	{
		invincible--;
		return;
	}

	int x, y;

	int sX = mPosX + 111;
	int sY = mPosY + 32;

	for (int i = 0; i < *enemyCount; i++)
	{
		x = enemyList[i]->getX();
		y = enemyList[i]->getY();
		if ((sX >= x && sX < x + 32 && sY >= y && sY < y + 32) && enemyList[i]->isNotExploding())
		{
			enemyList[i]->explode();
			hit = true;
		}
	}

	for (int i = 0; i < *missileCount; i++)
	{
		x = missileList[i]->getX();
		y = missileList[i]->getY();
		if (sX >= x && sX < x + 32 && sY >= y && sY < y + 32)
		{
			hit = true;
		}
	}

	if (boss->getLife()>0)
		if (sX >= boss->getX() && sX < boss->getX() + 128 && sY >= boss->getY() && sY < boss->getY() + 128)
		{
			hit = true;
		}
}

void Ship::render()
{
	if (invincible == 0)
	{
		gShipTexture[currentFrame >> 1]->render(mPosX, mPosY);
	}
	else
	{
		if (draw)
			gShipTexture[currentFrame >> 1]->render(mPosX, mPosY);
		draw = !draw;
	}
	currentFrame++;
	if (currentFrame == 20) currentFrame = 0;
}

Ship::~Ship()
{
	for (int i = 0; i < 10; i++)
	{
		gShipTexture[i]->free();
	}
}

int Ship::getX() { return mPosX; }

int Ship::getY() { return mPosY; }