#pragma once

#include <SDL.h>
#include "vec2.h"

// TODO: Add support for other shapes/textures
struct RendererComponent {
	Vec2Int size;
	Vec2 pivot; // (0,0 top left) to (1,1 bottom right)
	SDL_Color colour;
	SDL_Texture* texture;
	SDL_Rect textureClip; // If using a spritesheet, which part to render
	int zIndex;

	// If rendering a solid colour
	RendererComponent(Vec2Int size, SDL_Color colour, int zIndex = 0, Vec2 pivot = Vec2(0.5f, 0.5f)) : 
		size(size), colour(colour), pivot(pivot), zIndex(zIndex), texture(nullptr), textureClip() { }

	// If renderering a texture
	RendererComponent(Vec2Int size, SDL_Texture* texture, SDL_Rect textureClip, int zIndex = 0, Vec2 pivot = Vec2(0.5f, 0.5f)) :
		size(size), colour(), pivot(pivot), zIndex(zIndex), texture(texture), textureClip(textureClip) { }
};