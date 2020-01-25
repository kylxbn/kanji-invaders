#pragma once

#include <string>

class LTexture
{
public:
	LTexture();

	void Init(SDL_Window* w, SDL_Renderer* r);

	~LTexture();

	bool loadFromFile(std::string path, int r, int g, int b);
	bool loadFromFile(std::string path);


#ifdef _SDL_TTF_H
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* gFont);
#endif

	void free();

	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	void setBlendMode(SDL_BlendMode blending);

	void setAlpha(Uint8 alpha);

	void render(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidth();
	int getHeight();

private:
	SDL_Texture* mTexture;

	int mWidth;
	int mHeight;

	SDL_Renderer* gRenderer;
	SDL_Window* gWindow;
};
