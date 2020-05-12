#pragma once

#include "vec2.h"

enum class TileType {
	Empty, Floor, Corridor
};

struct Tile {
	TileType type;
	Vec2Int pos;

	Tile() : Tile(TileType::Empty, Vec2Int()) {}
	Tile(TileType type, Vec2Int pos) : type(type), pos(pos) {}
};
