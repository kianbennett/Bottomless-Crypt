#include <iostream>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "ecs.h"
#include "texture.h"
#include "c_transform.h"
#include "s_render.h"

using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void initSDL();
void loadResources();

// Definition of static variables in ECS
std::unique_ptr<EntityManager> ECS::entityManager = std::make_unique<EntityManager>();
std::unique_ptr<ComponentManager> ECS::componentManager = std::make_unique<ComponentManager>();
std::unique_ptr<SystemManager> ECS::systemManager = std::make_unique<SystemManager>();

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;
Texture* fontTexture;

Uint32 deltaTime, lastTime;

int main(int argc, char* args[]) {
	initSDL();
	loadResources();

	ECS::registerComponent<TransformComponent>();

	std::shared_ptr<RenderSystem> renderSystem = ECS::registerSystem<RenderSystem>();
	{
		Signature signature;
		signature.set(ECS::getComponentId<TransformComponent>());
		ECS::setSystemSignature<RenderSystem>(signature);
	}
	renderSystem->init();

	Entity player = ECS::createEntity();
	ECS::addComponent(player, TransformComponent {
		0.0f, 0.0f, 0.0f, 1.0f, 1.0f
	});

	bool quit = false;
	SDL_Event e;
	// Read and write to a string in memory
	std::stringstream timeText;

	while (!quit) {
		// Iterate over all events that have been captured since last frame
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					quit = true;
					break;
				}
			}
		}

		// Get deltaTime but measuring difference in time elapsed since last frame
		Uint32 time = SDL_GetTicks();
		deltaTime = time - lastTime;
		lastTime = time;

		renderSystem->update(deltaTime);

		// Clear screen to background colour
		SDL_RenderClear(renderer);

		// Render code
		timeText.str("");
		timeText << "elapsed time: " << time;
		fontTexture->loadFromText(renderer, timeText.str(), font, { 0xFF, 0xFF, 0xFF });
		fontTexture->render(renderer, 5, 5);

		// Update screen
		SDL_RenderPresent(renderer);
		SDL_Delay(30);
	}

	ECS::destroyEntity(player);

	delete fontTexture;
	TTF_CloseFont(font);
	TTF_Quit();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	IMG_Quit();

	return 0;
}

void initSDL() {
	// Initialise SDL with video flag
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not intialise SDL - Error: %s\n", SDL_GetError());
		return;
	}

	// Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
		printf("Warning - linear texture filtering not enabled\n");
	}

	// Create SDL window
	window = SDL_CreateWindow("CT4TOGA Artefact", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		printf("Could not create SDL window - Error: %s\n", SDL_GetError());
		return;
	}

	// Create window renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == NULL) {
		printf("Could not create window renderer - Error: %s\n", SDL_GetError());
		return;
	}

	SDL_SetRenderDrawColor(renderer, 0x20, 0x20, 0x30, 0xFF);

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not be initialised - Error: %s\n", IMG_GetError());
	}

	if (TTF_Init() == -1) {
		printf("SDL_ttf could not be intialised - Error: %s\n", TTF_GetError());
	}
}

void loadResources() {
	const char* fontPath = "res/fonts/consolas.ttf";
	font = TTF_OpenFont(fontPath, 16);

	if (font == NULL) {
		printf("Failed to load font from %s - Error: %s\n", fontPath, SDL_GetError());
		return;
	}

	fontTexture = new Texture();
	fontTexture->loadFromText(renderer, "Hello World!", font, { 0xFF, 0xFF, 0xFF });
}