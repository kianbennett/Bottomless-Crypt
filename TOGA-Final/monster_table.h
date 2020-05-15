#pragma once

#include <SDL.h>
#include "ecs.h"

extern SDL_Texture* spritesheet;

class MonsterTable {
public:
	// Get a random monster based on dungeon depth
	static Entity getMonster(Level* level, Tile tile) {
		std::vector<int> monsterIds;

		monsterIds.insert(monsterIds.end(), 4, 0); // Adds 0 to the end of the vector 5 times

		if (level->depth > 0) {
			monsterIds.insert(monsterIds.end(), 4, 1);
			monsterIds.insert(monsterIds.end(), 2, 2);
		}

		if (level->depth > 2) {
			monsterIds.insert(monsterIds.end(), 4, 3);
			monsterIds.insert(monsterIds.end(), 4, 4);
		}

		if (level->depth > 5) {
			monsterIds.insert(monsterIds.end(), 5, 5);
			monsterIds.insert(monsterIds.end(), 5, 6);
		}

		if (level->depth > 8) {
			monsterIds.insert(monsterIds.end(), 10, 7);
		}

		int n = monsterIds[rand() % monsterIds.size()];

		switch (n) {
			case 0:
				return rat(level, tile);
			case 1:
				return slime(level, tile);
			case 2:
				return goblin(level, tile);
			case 3:
				return kobold(level, tile);
			case 4:
				return orc(level, tile);
			case 5:
				return orcCaptain(level, tile);
			case 6:
				return minotaur(level, tile);
			case 7:
				return werewolf(level, tile);
		}

		return rat(level, tile);
	}

	static Entity rat(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 16, 0, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("rat", 1, 0 + rand() % 3));
		ECS::addComponent(entity, MonsterComponent(1));
		return entity;
	}

	static Entity slime(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 0, 0, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("slime", 3, 5 + rand() % 5));
		ECS::addComponent(entity, MonsterComponent(2));
		return entity;
	}
	

	static Entity goblin(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 32, 0, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("goblin", 6, 8 + rand() % 8));
		ECS::addComponent(entity, MonsterComponent(4));
		return entity;
	}

	static Entity kobold(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 48, 0, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("kobold", 8, 10 + rand() % 10));
		ECS::addComponent(entity, MonsterComponent(5));
		return entity;
	}

	static Entity orc(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 0, 16, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("orc", 10, 15 + rand() % 15));
		ECS::addComponent(entity, MonsterComponent(8));
		return entity;
	}

	static Entity orcCaptain(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 16, 16, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("orc captain", 12, 20 + rand() % 20));
		ECS::addComponent(entity, MonsterComponent(10));
		return entity;
	}

	static Entity minotaur(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 32, 16, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("minotaur", 30, 20 + rand() % 30));
		ECS::addComponent(entity, MonsterComponent(15));
		return entity;
	}

	static Entity werewolf(Level* level, Tile tile) {
		Entity entity = baseMonster(level, tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 48, 16, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("werewolf", 40, 30 + rand() % 50));
		ECS::addComponent(entity, MonsterComponent(20));
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