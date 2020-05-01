#include "level.h"
#include "level_object_table.h" // Putting this include in level.h causes problems, no idea why
#include "monster_table.h"

Level::Level() {
	//levelWidth = 26;
	//levelHeight = 26;

	generate();
	build();
	spawnObjects();
}

void Level::generate() {
	// Dungeons size increases as depth increases, but is capped at a max value
	levelWidth = std::min(16 + depth * 2, 50);
	levelHeight = std::min(16 + depth * 2, 50);

	tiles = Matrix<Tile>(levelWidth, levelHeight);

	// Populate the level with empty tiles
	for (int i = 0; i < levelWidth; i++) {
		for (int j = 0; j < levelWidth; j++) {
			tiles.set(i, j, Tile(TileType::Empty, Vec2Int(i, j)));
		}
	}

	// Split the level into rooms
	bsp.build(Vec2Int(), Vec2Int(levelWidth, levelHeight), 5);

	// We don't want only one room, so as a quick solution just regenerate (since this isn't a common occurence)
	if (bsp.rooms.size() == 1) generate();

	Vec2Int startPoint = bsp.rooms.begin()->first->centrePoint();
	std::vector<BSPNode*> orderedRooms;

	// Could talk about preference for using full type instead of auto
	for (std::pair<BSPNode*, SDL_Rect> room : bsp.rooms) {
		//bool useRoom = rand() % 4;
		SDL_Rect rect = room.second;
		for (int x = 0; x < rect.w; x++) {
			for (int y = 0; y < rect.h; y++) {
				Vec2Int pos = Vec2Int(rect.x + x, rect.y + y);
				tiles.get(pos.x, pos.y).type = TileType::Floor;
			}
		}

		orderedRooms.push_back(room.first);
	}

	// Talk about lamba expressions and using [&] from https://stackoverflow.com/questions/26903602/an-enclosing-function-local-variable-cannot-be-referenced-in-a-lambda-body-unles
	std::sort(orderedRooms.begin(), orderedRooms.end(), [&](BSPNode* a, BSPNode* b) -> bool {
		return Vec2Int::distance(startPoint, a->centrePoint()) < Vec2Int::distance(startPoint, b->centrePoint());
	});
	
	Matrix<GridNode> grid = Matrix<GridNode>(levelWidth, levelHeight);
	for (int i = 0; i < levelWidth; i++) {
		for (int j = 0; j < levelHeight; j++) {
			float cost = 0;
			if (tiles.get(i, j).type == TileType::Empty) {
				cost = 10;
			}
			grid.set(i, j, GridNode(Vec2Int(i, j), cost));
		}
	}

	// Use A* pathfinding to find shortest route between rooms to connnect with corridors
	for (BSPNode* room : orderedRooms) {
		Vec2Int destination = room->centrePoint();

		std::vector<Vec2Int> path = Pathfinder::findPathOnGrid(grid, startPoint, destination);

		for (int i = 0; i < path.size(); i++) {
			Vec2Int point = path[i];
			TileType existingType = tiles.get(point.x, point.y).type;
			if (existingType == TileType::Empty) {
				tiles.set(point.x, point.y, Tile(TileType::Corridor, point));
				grid.get(point.x, point.y).cost = 0;
			}
		}
	}

	// End tile is placed in the centre of the further room from the start room
	endTile = tiles.get(orderedRooms.back()->centrePoint());
}

void Level::build() {
	dealloc();

	Entity background = ECS::createEntity();
	ECS::addComponent<TransformComponent>(background, TransformComponent(0, 0));
	ECS::addComponent<RendererComponent>(background, RendererComponent(Vec2Int(getWidth(), getHeight()), { 0x20, 0x20, 0x30, 0xFF }, 0));
	allEntities.push_back(background);

	for (int i = 0; i < levelWidth; i++) {
		for (int j = 0; j < levelHeight; j++) {
			Tile tile = tiles.get(i, j);

			if (tile.type == TileType::Empty) continue;

			SDL_Color colourRoom = { 0x56, 0x4C, 0x6E, 0xFF };
			SDL_Color colourCorridor = { 0x46, 0x3C, 0x5E, 0xFF };

			Vec2Int pos = getTilePos(tile);
			SDL_Color tileColour = tile.type == TileType::Corridor ? colourCorridor : colourRoom;

			Entity entity = ECS::createEntity();
			ECS::addComponent<TransformComponent>(entity, TransformComponent(pos.x, pos.y));
			ECS::addComponent<RendererComponent>(entity, RendererComponent(Vec2Int(tileSize - 4, tileSize - 4), tileColour, 1));
			//ECS::addComponent<RendererComponent>(entity, RendererComponent(Vec2Int(tileSize, tileSize), spritesheet, { 48, 32, 16, 16 }, 1, Vec2()));
			allEntities.push_back(entity);
		}
	}

	renderSystem->sortEntities();
}

void Level::spawnObjects() {
	BSPNode* startNode = bsp.rooms.begin()->first;
	player = LevelObjectTable::player(this, tiles.get(startNode->centrePoint()), "Player");
	allEntities.push_back(player);

	Entity entityEnd = ECS::createEntity();
	ECS::addComponent<TransformComponent>(entityEnd, TransformComponent(getTilePos(endTile)));
	ECS::addComponent<RendererComponent>(entityEnd, RendererComponent(Vec2Int(tileSize, tileSize), spritesheet, { 32, 32, 16, 16 }, 2));
	allEntities.push_back(entityEnd);

	for (std::pair<BSPNode*, SDL_Rect> room : bsp.rooms) {
		SDL_Rect rect = room.second;
		int area = rect.w * rect.h;
		bool bigRoom = area > 50;

		// Spawn chests
		int chests = bigRoom ? (1 + rand() % 4) : (1 + rand() % 2);
		for (int i = 0; i < chests; i++) {
			int x = rand() % rect.w;
			int y = rand() % rect.h;
			Tile& tile = tiles.get(Vec2Int(rect.x + x, rect.y + y));

			// Ensure no chests are blocking corridors or placed on other objects
			if (!isTileBlockingCorridor(tile) && !isObjectOnTile(tile.pos) && tile.pos != endTile.pos) {
				allEntities.push_back(LevelObjectTable::chest(this, tile));
			}
		}

		int spikes = bigRoom ? (5 + rand() % 5) : (2 + rand() % 4);
		for (int i = 0; i < chests; i++) {
			int x = rand() % rect.w;
			int y = rand() % rect.h;
			Tile& tile = tiles.get(Vec2Int(rect.x + x, rect.y + y));

			// Ensure no chests are blocking corridors or placed on other objects
			if (!isTileBlockingCorridor(tile) && !isObjectOnTile(tile.pos) && tile.pos != endTile.pos) {
				allEntities.push_back(LevelObjectTable::spikes(this, tile));
			}
		}

		// Spawn monsters in every room but at the end
		if (room.first != startNode) {
			int monsters = rand() % (bigRoom ? 5 : 3);

			for (int i = 0; i < chests; i++) {
				int x = rand() % rect.w;
				int y = rand() % rect.h;
				Tile& tile = tiles.get(Vec2Int(rect.x + x, rect.y + y));

				// Ensure no chests are blocking corridors or placed on other objects
				if (!isTileBlockingCorridor(tile) && !isObjectOnTile(tile.pos) && tile.pos != endTile.pos) {
					allEntities.push_back(MonsterTable::getMonster(this, tile));
				}
			}
		}
	}

	renderSystem->sortEntities();
}

void Level::createLevel(int depth) {
	this->depth = depth;
	generate();
	build();
	spawnObjects();
}

SDL_Rect Level::getNextPointInArea(std::vector<SDL_Rect> existing, int w, int h) {
	SDL_Rect bestCandidate = { 0, 0, w, h };
	float bestDistance = 0;
	int sampleCount = 20; // High count yields better distrubition but lower performance
	for (int i = 0; i < sampleCount; i++) {
		SDL_Rect candidate = {
			rand() % (levelWidth - w), rand() % (levelHeight - h), w, h
		};
		float distance = distToClosest(candidate, existing);
		if (distance >= bestDistance) {
			bestCandidate = candidate;
			bestDistance = distance;
		}
	}
	return bestCandidate;
}

float Level::distToClosest(SDL_Rect room, std::vector<SDL_Rect> existing) {
	float dist = 0;
	for (SDL_Rect r : existing) {
		float d = Vec2Int::distance(Vec2Int(room.x, room.y), Vec2Int(r.x, r.y));
		if (dist == 0 || d < dist) dist = d;
	}
	return dist;
}

void Level::update(float dt) {
}

void Level::dealloc() {
	for (int i = 0; i < allEntities.size(); i++) {
		ECS::entityManager->setActive(allEntities[i], true);
		ECS::destroyEntity(allEntities[i]);
	}
	allEntities.clear();

	levelObjects = {};
	player = {};
}

void Level::destroyLevelObject(Entity entity) {
	levelObjects.erase(ECS::getComponent<LevelObjectComponent>(entity).tile.pos);
	// Destroying entities here causes problems and I'm not sure why
	// Setting them to inactive isn't optimal but the result is the same
	//ECS::destroyEntity(entity);
	ECS::entityManager->setActive(entity, false);
}

int Level::getWidth() {
	return levelWidth * tileSize;
}

int Level::getHeight() {
	return levelHeight * tileSize;
}

Vec2Int Level::getTilePos(Tile tile) {
	int x = -getWidth() / 2 + tile.pos.x * tileSize;
	int y = -getHeight() / 2 + tile.pos.y * tileSize;
	return Vec2Int(x, y);
}

void Level::drawDebug() {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	
	for (SDL_Rect rect : bsp.leaves) {
		rect.x -= levelWidth / 2;
		rect.y -= levelHeight / 2;
		rect.w *= tileSize * camera.getScale();
		rect.h *= tileSize * camera.getScale();
		rect.x *= tileSize;
		rect.y *= tileSize;
		Vec2 pos = camera.transform(Vec2(rect.x, rect.y));
		rect.x = pos.x;
		rect.y = pos.y;

		SDL_RenderDrawRect(renderer, &rect);
	}
}

bool Level::isTileBlockingCorridor(Tile tile) {
	if (tiles.isInsideMatrix(tile.pos.x, tile.pos.y + 1) && tiles.get(tile.pos.x, tile.pos.y + 1).type == TileType::Corridor) {
		return true;
	}
	if (tiles.isInsideMatrix(tile.pos.x, tile.pos.y - 1) && tiles.get(tile.pos.x, tile.pos.y - 1).type == TileType::Corridor) {
		return true;
	}
	if (tiles.isInsideMatrix(tile.pos.x - 1, tile.pos.y) && tiles.get(tile.pos.x - 1, tile.pos.y).type == TileType::Corridor) {
		return true;
	}
	if (tiles.isInsideMatrix(tile.pos.x + 1, tile.pos.y) && tiles.get(tile.pos.x + 1, tile.pos.y).type == TileType::Corridor) {
		return true;
	}
	return false;
}

bool Level::isObjectOnTile(Vec2Int pos) {
	return levelObjects.count(pos) > 0;
}