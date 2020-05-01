#pragma once

#include "vec2.h"
#include "c_tile.h"

enum class LevelObjectType {
	Player, Chest, Monster, Spikes
};

struct LevelObjectComponent {

	LevelObjectType type;
	Tile tile;

	LevelObjectComponent(LevelObjectType type, Tile tile) : type(type), tile(tile) {}
};