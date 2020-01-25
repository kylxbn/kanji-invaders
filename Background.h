#pragma once
#include <SDL.h>
#include <string>
#include "LTexture.h"

class Background
{
public:


	Background(SDL_Window* w, SDL_Renderer* r, std::string n, float s);
	Background(SDL_Window* w, SDL_Renderer* r, float s, float o);
	void Background::Load(std::string n);

	~Background();
	void move();
	void render();
	float getX();

private:
	float mPosX, mPosY;
	int BG_WIDTH = 64;
	int BG_HEIGHT = 64;
	float X_VEL = 1;
	float Y_VEL = 0;
	LTexture* gBGTexture;
	bool planet = false;
};