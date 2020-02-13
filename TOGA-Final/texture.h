#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

class Texture {
public:
	Texture();
	~Texture();

	// Load texture from file
	bool loadFromFile(SDL_Renderer* renderer, std::string path);
	// Load texture from font string
	bool loadFromText(SDL_Renderer* renderer, std::string text, TTF_Font* font, SDL_Color colour);

	// Frees texture resources
	void dealloc();

	void setColour(Uint8 r, Uint8 g, Uint8 b);
	void setAlpha(Uint8 a);
	void setBlendMode(SDL_BlendMode blendMode);

	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL, float angle = 0.0f, SDL_Point* centre = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	int getWidth();
	int getHeight();

private:
	SDL_Texture* texture;
	int width, height;
};
