#include <iostream>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "ecs.h"
#include "texture.h"
#include "c_transform.h"
#include "c_ui_text.h"
#include "s_render.h"
#include "s_render_text.h"

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
Uint32 fpsCheckTime, framesCounted;

int main(int argc, char* args[]) {
	initSDL();
	loadResources();

	ECS::registerComponent<TransformComponent>();
	ECS::registerComponent<UITextComponent>();

	Signature renderSignature;
	renderSignature.set(ECS::getComponentId<TransformComponent>());
	std::shared_ptr<RenderSystem> renderSystem = ECS::registerSystem<RenderSystem>(renderSignature);

	Signature renderTextSignature;
	renderTextSignature.set(ECS::getComponentId<TransformComponent>());
	renderTextSignature.set(ECS::getComponentId<UITextComponent>());
	std::shared_ptr<RenderTextSystem> renderTextSystem = ECS::registerSystem<RenderTextSystem>(renderTextSignature);
	renderTextSystem->setRenderer(renderer);

	Entity player = ECS::createEntity();
	ECS::addComponent(player, TransformComponent());

	Entity timeText = ECS::createEntity();
	ECS::addComponent(timeText, TransformComponent(5, 5));
	ECS::addComponent(timeText, UITextComponent("asd", { 0xFF, 0xFF, 0xFF }, font));

	renderSystem->init();
	renderTextSystem->init();

	bool quit = false;
	SDL_Event e;
	// Read and write to a string in memory
	std::stringstream timeTextStream;

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

		// Update FPS check
		fpsCheckTime += deltaTime;
		framesCounted++;
		int fpsCheckInterval = 50;
		if (fpsCheckTime > fpsCheckInterval) {
			int fps = (int) (framesCounted / (fpsCheckTime / 1000.0f));
			timeTextStream.str("");
			timeTextStream << "fps: " << fps;
			ECS::getComponent<UITextComponent>(timeText).text = timeTextStream.str();
			ECS::getComponent<UITextComponent>(timeText).textChanged = true;
			fpsCheckTime = 0;
			framesCounted = 0;
		}

		// Clear screen to background colour
		SDL_RenderClear(renderer);

		// Render code
		renderSystem->update(deltaTime);
		renderTextSystem->update(deltaTime);

		// Update screen
		SDL_RenderPresent(renderer);
	}

	renderSystem->dealloc();
	renderTextSystem->dealloc();

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
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED/* | SDL_RENDERER_PRESENTVSYNC*/);

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
	font = TTF_OpenFont(fontPath, 20);

	if (font == NULL) {
		printf("Failed to load font from %s - Error: %s\n", fontPath, SDL_GetError());
		return;
	}

	fontTexture = new Texture();
	fontTexture->loadFromText(renderer, "Hello World!", font, { 0xFF, 0xFF, 0xFF });
}