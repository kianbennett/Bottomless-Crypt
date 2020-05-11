#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

struct UITextComponent {
	std::string text;
	SDL_Texture* texture;
	SDL_Color colour;
	TTF_Font* font;
	int width, height;
	Vec2 pivot;
	bool textChanged;

	UITextComponent(std::string text, SDL_Color colour, TTF_Font* font, Vec2 pivot = Vec2()) :
		text(text), texture(NULL), colour(colour), font(font), width(0), height(0), textChanged(false), pivot(pivot) {}
};