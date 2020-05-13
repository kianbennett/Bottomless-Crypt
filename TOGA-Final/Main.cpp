#include <iostream>
#include <sstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "ecs.h"
#include "camera.h"
#include "hud.h"
#include "start_screen.h"
#include "end_screen.h"
#include "level_object_table.h"
#include "level.h"
#include "pathfinder.h"
#include "turn_handler.h"
#include "s_render.h"
#include "s_render_text.h"
#include "s_move_objects.h"
#include "s_monster_handler.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

void changeState(GameState state);
void initSDL();
void loadResources();

// Definition of static variables in ECS since they can't be defined in the ECS class at compile time
EntityManager* ECS::entityManager = new EntityManager();
ComponentManager* ECS::componentManager = new ComponentManager();
SystemManager* ECS::systemManager = new SystemManager();

RenderSystem* renderSystem;
RenderTextSystem* renderTextSystem;
MoveObjectSystem* moveObjectSystem;
MonsterHandlerSystem* monsterHandlerSystem;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* spritesheet;
TTF_Font* font, *titleFont;
HUD* hud;
Level* level;
StartScreen startScreen;
EndScreen endScreen;
Camera camera;

bool showInventory;
float deltaTime;
Uint32 lastTime;
GameState gameState;

int main(int argc, char* args[]) {
	initSDL();
	loadResources();

	ECS::registerComponent<CharacterComponent>();
	ECS::registerComponent<ChestComponent>();
	ECS::registerComponent<ItemComponent>();
	ECS::registerComponent<LevelObjectComponent>();
	ECS::registerComponent<MonsterComponent>();
	ECS::registerComponent<RendererComponent>();
	ECS::registerComponent<TransformComponent>();
	ECS::registerComponent<UITextComponent>();

	renderSystem = ECS::registerSystem<RenderSystem>();
	renderTextSystem = ECS::registerSystem<RenderTextSystem>();
	moveObjectSystem = ECS::registerSystem<MoveObjectSystem>();
	monsterHandlerSystem = ECS::registerSystem<MonsterHandlerSystem>();

	hud = new HUD();
	level = new Level();
	startScreen.init();
	endScreen.init();

	renderSystem->init();
	renderTextSystem->init();
	moveObjectSystem->init();
	monsterHandlerSystem->init();

	showInventory = false;
	bool quit = false;
	SDL_Event e;

	changeState(GameState::Start);

	while (!quit) {
		// Get deltaTime but measuring difference in time elapsed since last frame
		Uint32 time = SDL_GetTicks();
		deltaTime = (time - lastTime) * 0.001f; // ticks is in milliseconds - convert to seconds
		lastTime = time;

		// Iterate over all events that have been captured since last frame (for pressed keys)
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			if (e.type == SDL_KEYDOWN && !e.key.repeat) {
				switch (gameState) {
				case GameState::Start:
				case GameState::GameOver:
					switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						quit = true;
						break;
					case SDLK_SPACE:
						changeState(GameState::InGame);
						break;
					}
					break;
				case GameState::InGame:
					switch (e.key.keysym.sym) {
					case SDLK_ESCAPE:
						changeState(GameState::Start);
						//TurnHandler::damageCharacter(level->player, 10000);
						break;
					case SDLK_r:
						level->createLevel(level->depth);
						break;
					case SDLK_w:
						if (showInventory) hud->prevInventoryItem();
						else TurnHandler::takeTurn(Direction::Up);
						break;
					case SDLK_s:
						if (showInventory) hud->nextInventoryItem();
						else TurnHandler::takeTurn(Direction::Down);
						break;
					case SDLK_a:
						if (!showInventory) TurnHandler::takeTurn(Direction::Left);
						break;
					case SDLK_d:
						if (!showInventory) TurnHandler::takeTurn(Direction::Right);
						break;
					case SDLK_SPACE:
						if (!showInventory) TurnHandler::descend();
						break;
					case SDLK_i:
						showInventory = !showInventory;
						hud->updateInventory();
						break;
					case SDLK_e:
						if (showInventory) TurnHandler::useItem();
						break;
					case SDLK_x:
						if (showInventory) TurnHandler::dropItem();
						break;
					}
					break;
				}
			}
		}

		// Check keystate (for held keys)
		const Uint8* keyState = SDL_GetKeyboardState(NULL);

		if (keyState[SDL_SCANCODE_LEFTBRACKET]) camera.zoom(-1.0f * deltaTime);
		if (keyState[SDL_SCANCODE_RIGHTBRACKET]) camera.zoom(1.0f * deltaTime);

		camera.setPosition(ECS::getComponent<TransformComponent>(level->player).getPosInt() * camera.getScale());
		
		hud->update(deltaTime);
		level->update(deltaTime);

		// Clear screen to background colour
		SDL_SetRenderDrawColor(renderer, 24, 23, 41, 0xFF);
		SDL_RenderClear(renderer);

		// Update systems
		renderSystem->update(deltaTime);
		renderTextSystem->update(deltaTime);
		moveObjectSystem->update(deltaTime);
		monsterHandlerSystem->update(deltaTime);

		// Update screen (switches buffers)
		SDL_RenderPresent(renderer);
	}

	renderSystem->dealloc();
	renderTextSystem->dealloc();
	moveObjectSystem->dealloc();
	monsterHandlerSystem->dealloc();

	TTF_CloseFont(font);
	TTF_CloseFont(titleFont);
	TTF_Quit();

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	IMG_Quit();

	return 0;
}

void changeState(GameState state) {
	gameState = state;

	startScreen.setActive(state == GameState::Start);
	endScreen.setActive(state == GameState::GameOver);
	hud->setActive(state == GameState::InGame);
	hud->clearNotifications();

	if (state == GameState::InGame) {
		level->createLevel(0);
		hud->updatePlayerStats(ECS::getComponent<CharacterComponent>(level->player));
		hud->updateInventory();
	}
	else {
		level->dealloc();
	}
}

void initSDL() {
	// Initialise SDL with video flag
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not intialise SDL - Error: %s\n", SDL_GetError());
		return;
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

	// Initialise PNG
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not be initialised - Error: %s\n", IMG_GetError());
	}

	// Intialise TTF
	if (TTF_Init() == -1) {
		printf("SDL_ttf could not be intialised - Error: %s\n", TTF_GetError());
	}
}

void loadResources() {
	// Load the font
	const char* fontPath = "res/fonts/consolas.ttf";

	font = TTF_OpenFont(fontPath, 20);
	titleFont = TTF_OpenFont(fontPath, 72);

	if (font == NULL) {
		printf("Failed to load font from %s - Error: %s\n", fontPath, SDL_GetError());
	}

	// Load the spritesheet image
	const char* spritesheetPath = "res/spritesheet.png";
	SDL_Surface* loadedSurface = IMG_Load(spritesheetPath);

	if (loadedSurface == NULL) {
		printf("Failed to spritesheet from %s - Error: %s\n", spritesheetPath, SDL_GetError());
		return;
	}

	spritesheet = SDL_CreateTextureFromSurface(renderer, loadedSurface);

	if (spritesheet == NULL) {
		printf("Unable to create texture from %s - Error: %s\n", spritesheetPath, SDL_GetError());
	}

	SDL_FreeSurface(loadedSurface);
}