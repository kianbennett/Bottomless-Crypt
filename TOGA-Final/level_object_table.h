#pragma once

extern SDL_Texture* spritesheet;

class LevelObjectTable {
public:
	static Entity player(Level* level, Tile& tile, std::string name) {
		Entity entity = baseObject(level, tile, LevelObjectType::Player);
		ECS::addComponent(entity, RendererComponent(Vec2Int(32, 32), { 0xFF, 0xAA, 0xAA, 0xFF }, 4));
		ECS::addComponent(entity, CharacterComponent(name, 30, 0, 20, 20));

		return entity;
	}

	static Entity chest(Level* level, Tile& tile) {
		Entity entity = baseObject(level, tile, LevelObjectType::Chest);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 0, 32, 16, 16 }, 2));
		// ChestComponent chest = ChestComponent((level->depth + 1) * (2 + rand() % 10));
		// TODO Populate chest inventory
		ECS::addComponent(entity, ChestComponent());

		return entity;
	}

	static Entity spikes(Level* level, Tile& tile) {
		Entity entity = baseObject(level, tile, LevelObjectType::Spikes);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 0, 48, 16, 16 }, 2));

		return entity;
	}

private:
	static Entity baseObject(Level* level, Tile& tile, LevelObjectType type) {
		Entity entity = ECS::createEntity();
		ECS::addComponent(entity, TransformComponent(level->getTilePos(tile)));
		ECS::addComponent(entity, LevelObjectComponent(type, tile));
		level->levelObjects.insert({ tile.pos, entity });

		return entity;
	}
};