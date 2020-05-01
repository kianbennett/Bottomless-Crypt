#pragma once

#include "level.h"
#include "item_table.h"

extern Level* level;
extern HUD* hud;

enum class Direction {
	Up, Down, Left, Right
};

class TurnHandler {
public:
	static void takeTurn(Direction direction) {
		if (!ECS::entityManager->isAlive(level->player)) return;

		Tile& tileCurrent = ECS::getComponent<LevelObjectComponent>(level->player).tile;
		Tile tileTarget;
		bool canMove = false;
		switch (direction) {
			case Direction::Up:
				if (level->tiles.isInsideMatrix(tileCurrent.pos.x, tileCurrent.pos.y - 1)) {
					tileTarget = level->tiles.get(tileCurrent.pos.x, tileCurrent.pos.y - 1);
					canMove = true;
				}
				break;
			case Direction::Down:
				if (level->tiles.isInsideMatrix(tileCurrent.pos.x, tileCurrent.pos.y + 1)) {
					tileTarget = level->tiles.get(tileCurrent.pos.x, tileCurrent.pos.y + 1);
					canMove = true;
				}
				break;
			case Direction::Left:
				if (level->tiles.isInsideMatrix(tileCurrent.pos.x - 1, tileCurrent.pos.y)) {
					tileTarget = level->tiles.get(tileCurrent.pos.x - 1, tileCurrent.pos.y);
					canMove = true;
				}
				break;
			case Direction::Right:
				if (level->tiles.isInsideMatrix(tileCurrent.pos.x + 1, tileCurrent.pos.y)) {
					tileTarget = level->tiles.get(tileCurrent.pos.x + 1, tileCurrent.pos.y);
					canMove = true;
				}
				break;
		}

		if (canMove && tileTarget.type != TileType::Empty) {
			moveCharacterToTile(level->player, tileCurrent, tileTarget);
		}
	}

	static void descend() {
		if (!ECS::entityManager->isAlive(level->player)) return;

		Tile tileCurrent = ECS::getComponent<LevelObjectComponent>(level->player).tile;

		if (tileCurrent.pos == level->endTile.pos) {
			level->createLevel(++level->depth);
			hud->setTooltip("");
		}
	}

private:

	static void moveCharacterToTile(Entity entity, Tile tileCurrent, Tile tileTarget) {
		if (!level->isObjectOnTile(tileTarget.pos)) {
			if (entity == level->player) {
				if (tileTarget.pos == level->endTile.pos) {
					hud->setTooltip("Press SPACE to descend");
				}
				else if (tileCurrent.pos == level->endTile.pos) {
					hud->setTooltip("");
				}
			}

			level->levelObjects.erase(tileCurrent.pos);
			level->levelObjects.insert({ tileTarget.pos, entity });
			ECS::getComponent<LevelObjectComponent>(entity).tile = tileTarget;
		}
		else {
			Entity entityOnTarget = level->levelObjects[tileTarget.pos];
			// This isn't good practice as there is nothing to ensure the entity has the required components
			// Not so much a problem for this program as I know every entity added to the map has also got a LevelObjectComponent
			LevelObjectComponent levelObject = ECS::getComponent<LevelObjectComponent>(entityOnTarget);

			if (entity == level->player) {
				switch (levelObject.type) {
					case LevelObjectType::Chest:
						openChest(entityOnTarget);
					break;
					case LevelObjectType::Monster:
						attack(entity, entityOnTarget);
						//level->levelObjects.erase(ECS::getComponent<LevelObjectComponent>(entityOnTarget).tile.pos);
						break;
					case LevelObjectType::Spikes:
						int damage = 5;
						ECS::getComponent<RendererComponent>(entityOnTarget).textureClip = { 16, 48, 16, 16 };
						damageCharacter(level->player, damage);
						hud->addNotification("You ran into a spike and lost " + std::to_string(damage) + " health.");
						break;
				}
			}
			else {
				if (levelObject.type == LevelObjectType::Player) {
					attack(entity, entityOnTarget);
				}
			}
		}
	}

	static bool damageCharacter(Entity entity, int value) {
		CharacterComponent& character = ECS::getComponent<CharacterComponent>(entity);

		character.healthCurrent -= value;
		if (entity == level->player) {
			hud->updatePlayerStats(ECS::getComponent<CharacterComponent>(level->player));
		}
		// If healing don't let health go above max health
		if (character.healthCurrent > character.healthMax) {
			character.healthCurrent = character.healthMax;
		}
		if (character.healthCurrent <= 0) {
			if (entity == level->player) {
				level->createLevel(0);
			} else {
				level->destroyLevelObject(entity);
			}
			return true;
		}
		return false;
	}

	static void attack(Entity attacker, Entity defender) {
		CharacterComponent& charAttacker = ECS::getComponent<CharacterComponent>(attacker);
		CharacterComponent& charDefender = ECS::getComponent<CharacterComponent>(defender);

		int damage = charAttacker.strength / 2.0f - charDefender.defence / 4.0f;
		if (damage < 1) damage = 1; // damage must always be a positive value

		std::string str = "";

		if (attacker == level->player) {
			str += "You attacked " + nounArticle(charDefender.name) + " " + charDefender.name;
		}
		else {
			std::string str = nounArticle(charAttacker.name, true) + " " + charAttacker.name + " attacked you";
		}

		str += " dealing " + std::to_string(damage) + " damage.";
		hud->addNotification(str);

		LevelObjectComponent& objectAttacker = ECS::getComponent<LevelObjectComponent>(attacker);
		LevelObjectComponent& objectDefender = ECS::getComponent<LevelObjectComponent>(defender);

		if (damageCharacter(defender, damage)) {
			moveCharacterToTile(attacker, objectAttacker.tile, objectDefender.tile);
		}
	}

	static std::string nounArticle(std::string name, bool capitalise = false) {
		if (name[0] == 'a' || name[0] == 'e' || name[0] == 'i' || name[0] == 'o' || name[0] == 'u') {
			return capitalise ? "An" : "an";
		}
		return capitalise ? "A" : "a";
	}

	static void openChest(Entity entity) {
		RendererComponent& renderer = ECS::getComponent<RendererComponent>(entity);
		ChestComponent& chest = ECS::getComponent<ChestComponent>(entity);
		if (!chest.opened) {
			CharacterComponent& playerCharacter = ECS::getComponent<CharacterComponent>(level->player);
			renderer.textureClip = { 16, 32, 16, 16 };
			hud->addNotification("Opened chest, received " + std::to_string(chest.gold) + " gold.");
			playerCharacter.gold += chest.gold;
			hud->updatePlayerStats(playerCharacter);
			chest.opened = true;

			Entity item = ItemTable::getItem(level);
			if (playerCharacter.inventory.size() < playerCharacter.inventoryLimit) {
				playerCharacter.inventory.push_back(item);
				hud->updateInventory(playerCharacter);
				hud->addNotification("You found a " + ECS::getComponent<ItemComponent>(item).name + "!");
			}
			else {
				hud->addNotification("Your inventory is full, unable to pick up item.");
			}
		}
	}
};