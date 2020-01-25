#include "Planet.h"
#include <string>

float Planet::getX()
{
	return mPosX;
}

Planet::Planet(SDL_Window* w, SDL_Renderer* r, float s, float o)
{
	mPosX = o;
	mPosY = 0;
	X_VEL = s;

	gBGTexture = new LTexture();
	gBGTexture->Init(w, r);

	fnames[0] = std::string("images/planet1.png");
	fnames[1] = std::string("images/planet2.png");
	fnames[2] = std::string("images/planet3.png");

	planet = true;
}

void Planet::Load(std::string n)
{
	if (!gBGTexture->loadFromFile(n))
	{
		printf("Failed to load background texture!\n");
	}
	BG_WIDTH = gBGTexture->getWidth();
	BG_HEIGHT = gBGTexture->getHeight();
}

void Planet::move()
{
	mPosX -= X_VEL;
	mPosY -= Y_VEL;

	if (mPosX <= -1024)
	{
		Load(fnames[stage]);
		stage++;
		mPosX = 1024;
	}


}

void Planet::render()
{
	gBGTexture->render(mPosX, 0);
}

Planet::~Planet()
{
	gBGTexture->free();
}

void Planet::setX(float x)
{
	mPosX = x;
}