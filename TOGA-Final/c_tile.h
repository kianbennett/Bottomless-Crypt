#pragma once

#include "vec2.h"

enum class TileType {
	Empty, Floor, Corridor
};

// Forward declare struct
//struct LevelObjectComponent;

struct Tile {
	TileType type;
	Vec2Int pos;
	//LevelObjectComponent* object;

	Tile() : Tile(TileType::Empty, Vec2Int()) {}
	Tile(TileType type, Vec2Int pos) : type(type), pos(pos)/*, object(nullptr)*/ {}
};
