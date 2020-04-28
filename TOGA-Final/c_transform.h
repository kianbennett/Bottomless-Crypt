#pragma once

struct TransformComponent {
	float x, y;
	float rot;
	float scaleX, scaleY;

	Vec2 getPos() {
		return Vec2(x, y);
	}
	Vec2Int getPosInt() {
		return Vec2Int(x, y);
	}
	void setPos(Vec2 pos) {
		x = pos.x;
		y = pos.y;
	}
	void setPos(Vec2Int pos) {
		x = pos.x;
		y = pos.y;
	}

	TransformComponent(float x = 0.0f, float y = 0.0f, float rot = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f) : 
		x(x), y(y), rot(rot), scaleX(scaleX), scaleY(scaleY) {}

	TransformComponent(Vec2Int pos, float rot = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f) :
		x(pos.x), y(pos.y), rot(rot), scaleX(scaleX), scaleY(scaleY) {}
};