#pragma once
#include <SDL.h>
#include <string>
#include "LTexture.h"

class Planet
{
public:


	Planet(SDL_Window* w, SDL_Renderer* r, std::string n, float s);
	Planet(SDL_Window* w, SDL_Renderer* r, float s, float o);
	void Load(std::string n);

	~Planet();
	void move();
	void render();
	float getX();
	void setX(float x);

private:
	float mPosX, mPosY;
	int BG_WIDTH = 64;
	int BG_HEIGHT = 64;
	float X_VEL = 1;
	float Y_VEL = 0;
	LTexture* gBGTexture;
	bool planet = false;
	std::string fnames[3];
	int stage = 0;
};