#include "Background.h"
#include <string>

float Background::getX()
{
	return mPosX;
}

Background::Background(SDL_Window* w, SDL_Renderer* r, std::string n, float s)
{
	mPosX = 0;
	mPosY = 0;
	X_VEL = s;

	gBGTexture = new LTexture();
	gBGTexture->Init(w, r);
	if (!gBGTexture->loadFromFile(n))
	{
		printf("Failed to load background texture!\n");
	}
	if (s != 0.5) gBGTexture->setColor(128, 128, 128);
	BG_WIDTH = gBGTexture->getWidth();
	BG_HEIGHT = gBGTexture->getHeight();
}

Background::Background(SDL_Window* w, SDL_Renderer* r, float s, float o)
{
	mPosX = o;
	mPosY = 0;
	X_VEL = s;

	gBGTexture = new LTexture();
	gBGTexture->Init(w, r);
	
	planet = true;
}

void Background::Load(std::string n)
{
	if (!gBGTexture->loadFromFile(n))
	{
		printf("Failed to load background texture!\n");
	}
	BG_WIDTH = gBGTexture->getWidth();
	BG_HEIGHT = gBGTexture->getHeight();
}

void Background::move()
{
	mPosX -= X_VEL;
	mPosY -= Y_VEL;

	if (mPosX < -BG_WIDTH)
	{
		mPosX = 0;
	}

	if (mPosX > BG_WIDTH)
	{
		mPosX = 0;
	}


}

void Background::render()
{
	gBGTexture->render(mPosX, 0);
	if (X_VEL > 0)
		gBGTexture->render(mPosX + BG_WIDTH, 0);
	else
		gBGTexture->render(mPosX - BG_WIDTH, 0);
}

Background::~Background()
{
	gBGTexture->free();
}
