#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <cmath>
#include <sstream>

#include "LTexture.h"
#include "Enemy.h"
#include "Background.h"
#include "HomingMissile.h"
#include "Ship.h"
#include "EnemyMissile.h"
#include "Boss.h"
#include "Planet.h"

bool init();
bool loadMedia();
void close();

void showIntro();
void showTitle();
void showAbout();
void playGame();
void showHelp();

void cleanEnemies(Enemy* a[], int* s);
void cleanMissiles(HomingMissile* a[], int* s);
void cleanEM(EnemyMissile* a[], int* s);

SDL_Renderer* gRenderer = NULL;
SDL_Window* gWindow = NULL;
LTexture* gBGTexture = NULL;

LTexture* gEnemyTexture[10];


float speedd = 2;

int highScore = 0;
int score = 0;

bool bonus = false;
int bonusactivator = 0;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Kanji Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 600, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
				TTF_Init();
			}
		}
	}

	return success;
}

bool loadMedia()
{
	return true;
}


void close()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
}


int main(int argc, char* args[])
{
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			showIntro();
			showTitle();
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

void showIntro()
{
	SDL_Event e;

	LTexture p;
	p.Init(gWindow, gRenderer);
	p.loadFromFile("images/presents.png");
	p.setBlendMode(SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	for (int i = 0; i < 256; i+=4)
	{
		SDL_RenderClear(gRenderer);
		p.setAlpha(i);
		p.render(0,0);
		SDL_RenderPresent(gRenderer);

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				i = 256;
			}
		}
	}
	SDL_Delay(3000);

	SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0x0, 0xFF);

	for (int i = 255; i >= 0; i-=4)
	{
		SDL_RenderClear(gRenderer);
		p.setAlpha(i);
		p.render(0, 0);
		SDL_RenderPresent(gRenderer);

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				i = -1;
			}
		}

	}

	SDL_Delay(1000);
}

void showTitle()
{
	Mix_Music *gMusic = NULL;
	gMusic = Mix_LoadMUS("audio/title.ogg");
	if (gMusic == NULL)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	LTexture title, bg;
	title.Init(gWindow, gRenderer);
	bg.Init(gWindow, gRenderer);
	title.loadFromFile("images/title_bg.png");
	bg.loadFromFile("images/ship_bg.png");
	bg.render(0, 0);
	title.render(0, 0);
	
	int x = 0, y = 0;
	bool cont = true;
	SDL_Event e;
	Mix_PlayMusic(gMusic, -1);

	while (cont)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_GetMouseState(&x, &y);
				if (y > 315 && y < 376)
				{
					Mix_HaltMusic();
					playGame();
					Mix_PlayMusic(gMusic, -1);
				}
				else if (y > 400 && y < 462)
				{
					showAbout();
				}
				else if (y > 483 && y < 540)
				{
					cont = false;
				}
			}
			else if (e.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					if (bonusactivator == 0 || bonusactivator == 1) bonusactivator++; else bonusactivator = 0;
					break;

				case SDLK_DOWN:
					if (bonusactivator == 2 || bonusactivator == 3) bonusactivator++; else bonusactivator = 0;
					break;

				case SDLK_LEFT:
					if (bonusactivator == 4 || bonusactivator == 6) bonusactivator++; else bonusactivator = 0;					break;

				case SDLK_RIGHT:
					if (bonusactivator == 5 || bonusactivator == 7) bonusactivator++; else bonusactivator = 0;					break;
				default:
					bonusactivator = 0;
					break;
				}
			}
			else if (e.type == SDL_QUIT)
			{
				cont = false;
			}

		}
		bg.render(0, 0);
		title.render(0, 0);

		SDL_RenderPresent(gRenderer);

	}
	Mix_HaltMusic();
	Mix_FreeMusic(gMusic);
}

void playGame()
{

	bool cont = true;
	bool inBoss = false;

	int enemyCounter = 0;
	int enemyLoop = 120;
	int missileCounter = 0;
	int missileLoop = 20;
	int EMCounter = 0, EMLoop = 160;

	bool bossBehind = true;

	SDL_Event e;
	Enemy* anEnemy;
	HomingMissile* aMissile;
	EnemyMissile* anEM;
	Enemy* enemies[1000];
	EnemyMissile* em[1000];
	TTF_Font* font;
	font = TTF_OpenFont("C:/Windows/Fonts/meiryo.ttc", 28);
	LTexture* scoreText;
	scoreText = new LTexture();
	scoreText->Init(gWindow, gRenderer);
	SDL_Color textColor = { 255,255,0 };
	int eSize = 0;
	HomingMissile* missiles[1000];
	int mSize = 0;
	int EMSize = 0;
	Background* bgFar = new Background(gWindow, gRenderer, "images/BackgroundFar.png", -0.25*speedd);
	Planet* planet = new Planet(gWindow, gRenderer, 0.5*speedd, -1);
	Background* bgMiddle = new Background(gWindow, gRenderer, "images/BackgroundMiddle.png", 1*speedd);
	Background* bgNear = new Background(gWindow, gRenderer, "images/BackgroundNear.png", 1.5*speedd);

	Mix_Music *ingame = Mix_LoadMUS("audio/ingame.ogg");
	Mix_Music *bossmusic = Mix_LoadMUS("audio/boss.ogg");

	char* bonusbossfname[] = { "images/bonus_boss_ok_1.png", "images/bonus_boss_bad_1.png",
							 "images/bonus_boss_ok_2.png", "images/bonus_boss_bad_2.png",
							 "images/bonus_boss_ok_3.png", "images/bonus_boss_bad_3.png" };

	char* bossfname[] = { "images/boss_1.png", "images/boss_2.png",	"images/boss_3.png"};

	Mix_Chunk *boomsound[14];

	int bossindex = 4;

	char* fnames[] = { "audio/crash1.ogg", "audio/crash2.ogg", "audio/crash3.ogg", "audio/crash4.ogg",
							"audio/crash5.ogg", "audio/crash6.ogg", "audio/crash7.ogg", "audio/crash8.ogg",
							"audio/crash9.ogg", "audio/crash10.ogg", "audio/crash11.ogg", "audio/crash12.ogg",
							"audio/crash13.ogg", "audio/crash14.ogg"};

	char* planets[] = { "images/planet1.png", "images/planet2.png", "images/planet3.png" };
	int stage = 0;

	for (int i = 0; i < 14; i++)
	{
		boomsound[i] = Mix_LoadWAV(fnames[i]);
	}

	Boss* boss = new Boss(gWindow, gRenderer, boomsound);
	Ship* ship = new Ship(gWindow, gRenderer, enemies, &eSize, em, &EMSize, boss);

	Mix_PlayMusic(ingame, -1);

	std::string drawScore;

	while (cont)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				cont = false;
			}

			ship->handleEvent(e);
		}

		if (enemyCounter >= enemyLoop)
		{
			anEnemy = new Enemy(gWindow, gRenderer, boomsound);
			enemies[eSize] = anEnemy;
			eSize++;
			enemyCounter = 0;
		}

		if (missileCounter >= missileLoop)
		{
			aMissile = new HomingMissile(gWindow, gRenderer, ship->getX(), ship->getY(), enemies, &eSize, boss);
			missiles[mSize] = aMissile;
			mSize++;
			missileCounter = 0;
		}

		if (EMCounter >= EMLoop)
		{
			if (eSize > 0)
			{
				anEnemy = enemies[rand() % eSize];
				anEM = new EnemyMissile(gWindow, gRenderer, anEnemy->getX(), anEnemy->getY(), ship->getX(), ship->getY());
				em[EMSize] = anEM;
				EMSize++;
				EMCounter = 0;
			}
		}

		SDL_SetRenderDrawColor(gRenderer, 0x0, 0x0, 0x0, 0xFF);
		SDL_RenderClear(gRenderer);

		if (!inBoss)
		{
			if (planet->getX() == 0)
			{
				Mix_HaltMusic();
				Mix_PlayMusic(bossmusic, -1);
				inBoss = true;
				if (bonusactivator == 8)
				{
					boss->loadNew(bonusbossfname[bossindex], bonusbossfname[bossindex + 1], 40+(bossindex*20), 1);
					bossindex += 2;
				}
				else
				{
					boss->loadNew(bossfname[bossindex], bossfname[bossindex], 40 + (bossindex * 40), 1);
					bossindex += 1;
				}
			}
		}

		if (!inBoss)
		{
			bgFar->move();
			planet->move();
			bgMiddle->move();
			bgNear->move();
			ship->move();

			bgFar->render();
			planet->render();
			bgMiddle->render();
			bgNear->render();
			ship->render();
		}

		else
		{
			ship->move();
			boss->move();

			bgFar->render();
			planet->render();
			bgMiddle->render();
			bgNear->render();
			ship->render();
			boss->render();
			
			if (boss->hasExploded())
			{
				inBoss = false;
				Mix_HaltMusic();
				Mix_PlayMusic(ingame, -1);
				planet->setX(-1);

				if (bonusactivator == 8)
				{
					if (bossindex == 6)
					{
						cont = false;
					}
				}
				else
				{
					if (bossindex == 3)
					{
						cont = false;
					}
				}

				enemyLoop -= 30;
				EMLoop -= 10;
			}

			

		}

		/*
		std::stringstream("SCORE") >> drawScore;
		std::stringstream(score) >> drawScore;

		scoreText->loadFromRenderedText(drawScore, textColor, font);

		drawScore = "";
		scoreText->render(10, 10);

		*/

		if (mSize >= 1000 || eSize >= 1000)
		{
			printf("ERROR!!!");
		}


		cleanEnemies(enemies, &eSize);
		cleanMissiles(missiles, &mSize);
		cleanEM(em, &EMSize);

		for (int i = 0; i < eSize; i++)
		{
			anEnemy = enemies[i];
				anEnemy->move();
				anEnemy->render();
		}

		for (int i = 0; i < mSize; i++)
		{
			aMissile = missiles[i];

			aMissile->move();
			aMissile->render();
		}

		for (int i = 0; i < EMSize; i++)
		{
			anEM = em[i];

			anEM->move();
			anEM->render();
		}

		if ((*ship).hit) cont = false;

		SDL_RenderPresent(gRenderer);

		missileCounter++;
		enemyCounter++;
		EMCounter++;
	}

	if (bonusactivator == 8)
	{
		if (bossindex == 6 && boss->hasExploded())
		{
			LTexture success;
			success.Init(gWindow, gRenderer);
			success.loadFromFile("images/success.png");
			success.render(0, 0);
			SDL_RenderPresent(gRenderer);
			SDL_Delay(5000);

		}
	}
	else
	{
		if (bossindex == 3 && boss->hasExploded())
		{
			LTexture success;
			success.Init(gWindow, gRenderer);
			success.loadFromFile("images/success.png");
			success.render(0, 0);
			SDL_RenderPresent(gRenderer);
			SDL_Delay(5000);

		}
	}

	if ((*ship).hit)
	{
		LTexture destroyed;
		destroyed.Init(gWindow, gRenderer);
		destroyed.loadFromFile("images/destroyed.png");
		destroyed.render(0, 0);
		SDL_RenderPresent(gRenderer);
		SDL_Delay(3000);
	}

	Mix_HaltMusic();
	Mix_FreeMusic(ingame);
	Mix_FreeMusic(bossmusic);

	for (int i = 0; i < 14; i++)
	{
		Mix_FreeChunk(boomsound[i]);
	}
}

void cleanEnemies(Enemy* a[], int* s)
{
	Enemy* e;
	for (int i = 0; i < *s; i++)
	{
		e = a[i];
		if (e->getX() < -200 || 
			e->getX() > 1224 ||
			e->getY() < -200 ||
			e->getY() > 800 ||
			e->hasExploded())
		{
			delete e;
			for (int j = i; j < *s; j++)
			{
				a[j] = a[j + 1];
			}
			(*s)--;
		}

	}
}

void cleanMissiles(HomingMissile* a[], int* s)
{
	HomingMissile* m;
	for (int i = 0; i < *s; i++)
	{
		m = a[i];
		if (m->getX() < -100 ||
			m->getX() > 1124 ||
			m->getY() < -100 ||
			m->getY() > 700 ||
			m->deleteMe)
		{

			delete m;
			for (int j = i; j < *s; j++)
			{
				a[j] = a[j + 1];
			}
			(*s)--;
		}
	}
}

void cleanEM(EnemyMissile* a[], int* s)
{
	EnemyMissile* m;
	for (int i = 0; i < *s; i++)
	{
		m = a[i];
		if (m->getX() < -100 ||
			m->getX() > 1124 ||
			m->getY() < -100 ||
			m->getY() > 700 ||
			m->deleteMe)
		{

			delete m;
			for (int j = i; j < *s; j++)
			{
				a[j] = a[j + 1];
			}
			(*s)--;
		}
	}
}


void showAbout()
{
	LTexture about, bg;
	about.Init(gWindow, gRenderer);
	bg.Init(gWindow, gRenderer);
	bg.loadFromFile("images/ship_bg.png");
	about.loadFromFile("images/about_bg.png");
	bg.render(0, 0);
	about.render(0, 0);

	int x = 0, y = 0;
	bool cont = true;
	SDL_Event e;
	while (cont)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_GetMouseState(&x, &y);
				if (y > 525 && y < 585)
				{
					if (x > 611 && x < 761)
					{
						showHelp();
					}
					else if (x > 855 && x < 1011)
					{
						cont = false;
					}
				}
			}
			else if (e.type == SDL_QUIT)
			{
				cont = false;
			}

			SDL_Delay(50);
			bg.render(0, 0);
			about.render(0, 0);
			SDL_RenderPresent(gRenderer);
		}

	}
}

void showHelp()
{
	LTexture help, bg;
	help.Init(gWindow, gRenderer);
	bg.Init(gWindow, gRenderer);
	bg.loadFromFile("images/ship_bg.png");
	help.loadFromFile("images/help.png");
	bg.render(0, 0);
	help.render(0, 0);

	int x = 0, y = 0;
	bool cont = true;
	SDL_Event e;
	while (cont)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				SDL_GetMouseState(&x, &y);
				if (y > 525 && y < 585)
				{
					if (x > 855 && x < 1011)
					{
						cont = false;
					}
				}
			}
			else if (e.type == SDL_QUIT)
			{
				cont = false;
			}

			SDL_Delay(50);
			bg.render(0, 0);
			help.render(0, 0);
			SDL_RenderPresent(gRenderer);
		}

	}
}