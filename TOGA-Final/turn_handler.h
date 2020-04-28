#pragma once

#include "level.h"

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
			moveToTile(tileCurrent, tileTarget);
		}
	}

	static void descend() {
		if (!ECS::entityManager->isAlive(level->player)) return;

		Tile tileCurrent = ECS::getComponent<LevelObjectComponent>(level->player).tile;

		if (tileCurrent.pos == level->endTile.pos) {
			level->generate(++level->depth);
			level->build();
			level->spawnObjects();
			hud->setTooltip("");
		}
	}

private:

	static void moveToTile(Tile tileCurrent, Tile tileTarget) {
		if (!level->isObjectOnTile(tileTarget.pos)) {
			if (tileTarget.pos == level->endTile.pos) {
				hud->setTooltip("Press SPACE to descend");
			}
			else if (tileCurrent.pos == level->endTile.pos) {
				hud->setTooltip("");
			}

			level->levelObjects.erase(tileCurrent.pos);
			level->levelObjects.insert({ tileTarget.pos, level->player });
			ECS::getComponent<LevelObjectComponent>(level->player).tile = tileTarget;
		}
		else {
			Entity entity = level->levelObjects[tileTarget.pos];
			// This isn't good practice as there is nothing to ensure the entity has the required components
			// Not so much a problem for this program as I know every entity added to the map has also got a LevelObjectComponent
			LevelObjectComponent levelObject = ECS::getComponent<LevelObjectComponent>(entity);
			switch (levelObject.type) {
				case LevelObjectType::Player:
					break;
				case LevelObjectType::Chest:
					{ // Put this in its own scope stops errors about renderer not being able to be initialised
						RendererComponent& renderer = ECS::getComponent<RendererComponent>(entity);
						ChestComponent chest = ECS::getComponent<ChestComponent>(entity);
						renderer.textureClip = { 16, 32, 16, 16 };
						hud->addNotification("Opened chest, received " + std::to_string() + " gold.");
					}
					break;
				case LevelObjectType::Monster:
					break;
			}
		}
	}
};