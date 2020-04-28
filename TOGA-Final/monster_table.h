#pragma once

#include <SDL.h>
#include "ecs.h"

extern SDL_Texture* spritesheet;

class MonsterTable {
public:
	static Entity rat(Tile tile) {
		Entity entity = baseMonster(tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 16, 0, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("Rat", 1, 0 + rand() % 3, 2, 4));
		ECS::addComponent(entity, MonsterComponent());
		return entity;
	}

	static Entity slime(Tile tile) {
		Entity entity = baseMonster(tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 0, 0, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("Slime", 15, 5 + rand() % 5, 8, 8));
		ECS::addComponent(entity, MonsterComponent());
		return entity;
	}
	

	static Entity goblin(Tile tile) {
		Entity entity = baseMonster(tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 32, 0, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("Goblin", 30, 8 + rand() % 8, 12, 8));
		ECS::addComponent(entity, MonsterComponent());
		return entity;
	}

	static Entity kobold(Tile tile) {
		Entity entity = baseMonster(tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 48, 0, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("Kobold", 35, 10 + rand() % 10, 15, 10));
		ECS::addComponent(entity, MonsterComponent());
		return entity;
	}

	static Entity orc(Tile tile) {
		Entity entity = baseMonster(tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 0, 16, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("Orc", 45, 15 + rand() % 15, 20, 15));
		ECS::addComponent(entity, MonsterComponent());
		return entity;
	}

	static Entity orcCaptain(Tile tile) {
		Entity entity = baseMonster(tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 16, 16, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("Orc Captain", 50, 20 + rand() % 20, 22, 20));
		ECS::addComponent(entity, MonsterComponent());
		return entity;
	}

	static Entity minotaur(Tile tile) {
		Entity entity = baseMonster(tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 32, 16, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("Minotaur", 70, 30 + rand() % 30, 30, 25));
		ECS::addComponent(entity, MonsterComponent());
		return entity;
	}

	static Entity werewolf(Tile tile) {
		Entity entity = baseMonster(tile);
		ECS::addComponent(entity, RendererComponent(Vec2Int(64, 64), spritesheet, { 48, 16, 16, 16 }, 2));
		ECS::addComponent(entity, CharacterComponent("Werewolf", 80, 50 + rand() % 50, 40, 40));
		ECS::addComponent(entity, MonsterComponent(DamageType::Silver));
		return entity;
	}

private:

	static Entity baseMonster(Tile tile) {
		Entity entity = ECS::createEntity();
		ECS::addComponent(entity, TransformComponent());
		ECS::addComponent(entity, LevelObjectComponent(LevelObjectType::Monster, tile));

		return entity;
	}
};