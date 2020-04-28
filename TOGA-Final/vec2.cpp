#include "vec2.h"

Vec2::operator Vec2Int() const {
	return Vec2Int((int)x, (int)y);
}