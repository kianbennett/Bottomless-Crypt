#pragma once

#include "ecs.h"
#include "matrix.h"
#include "bsp.h"
#include "pathfinder.h"
#include "s_render.h"
#include "components.h"

#include <map>

extern std::shared_ptr<RenderSystem> renderSystem;
extern SDL_Renderer* renderer;
extern SDL_Texture* spritesheet;

class Level {
public:
	const int tileSize = 64;
	int depth;
	Entity player;

	// Store tiles in a single array instead of a 2d array to ensure the values are stored contiguously
	Matrix<Tile> tiles;
	//Matrix<Entity*> levelObjects; // use a pointer so the value can be set to nullptr if no object is at that position
	std::map<Vec2Int, Entity> levelObjects;
	std::vector<Entity> allEntities;
	Tile endTile;

	Level();

	void generate(int depth);
	void build();
	void spawnObjects();
	void update(float dt);
	void dealloc();

	int getWidth();
	int getHeight();

	Vec2Int getTilePos(Tile tile);
	float distToClosest(SDL_Rect room, std::vector<SDL_Rect> existing);
	SDL_Rect getNextPointInArea(std::vector<SDL_Rect> existing, int w, int h);
	bool isTileBlockingCorridor(Tile tile);
	bool isObjectOnTile(Vec2Int pos);

	void drawDebug();

private:
	BSP bsp;
	int levelWidth, levelHeight;
};