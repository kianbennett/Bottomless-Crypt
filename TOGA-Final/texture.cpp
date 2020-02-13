#include "texture.h"

Texture::Texture() {
	width = height = 0;
	texture = NULL;
}

Texture::~Texture() {
	dealloc();
}

bool Texture::loadFromFile(SDL_Renderer* renderer, std::string path) {
	// Delete existing texture
	dealloc();

	SDL_Surface* surface = IMG_Load(path.c_str());

	if (surface != NULL) {
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (texture != NULL) {
			width = surface->w;
			height = surface->h;
		}
		else {
			printf("Could not load texture from %s - Error: %s", path.c_str(), SDL_GetError());;
		}

		// Free surface from memory since it's no longer needed
		SDL_FreeSurface(surface);
	} else {
		printf("Unable to load image from %s - Error: %s", path.c_str(), SDL_GetError());
	}

	return texture != NULL;
}

bool Texture::loadFromText(SDL_Renderer* renderer, std::string text, TTF_Font* font, SDL_Color colour) {
	// Delete existing texture
	dealloc();

	SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), colour);

	if (surface != NULL) {
		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (texture != NULL) {
			// Get image dimensions
			//width = surface->w;
			//height = surface->h;
			SDL_QueryTexture(texture, NULL, NULL, &width, &height);
		}
		else {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}

		// Free surface from memory since it's no longer needed
		SDL_FreeSurface(surface);
	}
	else {
		printf("Unable to render text surface - Error: %s", TTF_GetError());
	}

	return texture != NULL;
}

void Texture::dealloc() {
	if (texture != NULL) {
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}

void Texture::setColour(Uint8 r, Uint8 g, Uint8 b) {
	SDL_SetTextureColorMod(texture, r, g, b);
}

void Texture::setAlpha(Uint8 a) {
	SDL_SetTextureAlphaMod(texture, a);
}

void Texture::setBlendMode(SDL_BlendMode blendMode) {
	SDL_SetTextureBlendMode(texture, blendMode);
}

void Texture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, float angle, SDL_Point* centre, SDL_RendererFlip flip) {
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };

	//Set clip rendering dimensions
	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, centre, flip);
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}