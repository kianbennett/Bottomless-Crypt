#pragma once

#include <SDL.h>
#include "ecs.h"

extern SDL_Texture* spritesheet;

class MonsterTable {
public:
	// Get a random monster based on dungeon depth
	static Entity getMonster(Level* level, Tile tile) {
		std::vector<int> monsterIds;

		monsterIds.insert(monsterIds.end(), 5, 0); // Adds 0 to the end of the vector 5 times
		monsterIds.insert(monsterIds.end(), 4, 1);
		monsterIds.insert(monsterIds.end(), 2, 2);

		int n = monsterIds[rand() % monsterIds.size()];

		switch (n) {
			case 0:
				return rat(level, tile);
			case 1:
				return slime(level, tile);
			case 2:
				return goblin(level, tile);
		}

		return rat(level, tile);
	}

	static Entity rat(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 16, 0, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("rat", 1, 0 + rand() % 3));
		ECS::addComponent(entity, MonsterComponent(2));
		return entity;
	}

	static Entity slime(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 0, 0, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("slime", 15, 5 + rand() % 5));
		ECS::addComponent(entity, MonsterComponent(8));
		return entity;
	}
	

	static Entity goblin(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 32, 0, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("goblin", 30, 8 + rand() % 8));
		ECS::addComponent(entity, MonsterComponent(12));
		return entity;
	}

	static Entity kobold(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 48, 0, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("kobold", 35, 10 + rand() % 10));
		ECS::addComponent(entity, MonsterComponent(15));
		return entity;
	}

	static Entity orc(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 0, 16, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("orc", 45, 15 + rand() % 15));
		ECS::addComponent(entity, MonsterComponent(20));
		return entity;
	}

	static Entity orcCaptain(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 16, 16, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("orc captain", 50, 20 + rand() % 20));
		ECS::addComponent(entity, MonsterComponent(22));
		return entity;
	}

	static Entity minotaur(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 32, 16, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("minotaur", 70, 30 + rand() % 30));
		ECS::addComponent(entity, MonsterComponent(30));
		return entity;
	}

	static Entity werewolf(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 48, 16, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("werewolf", 80, 50 + rand() % 50));
		ECS::addComponent(entity, MonsterComponent(40));
		return entity;
	}

private:

	static Entity baseMonster(Level* level, Tile tile) {
		Entity entity = ECS::createEntity();
		ECS::addComponent(entity, TransformComponent(level->getTilePos(tile)));
		ECS::addComponent(entity, LevelObjectComponent(LevelObjectType::Monster, tile));
		level->levelObjects.insert({ tile.pos, entity });

		return entity;
	}
};