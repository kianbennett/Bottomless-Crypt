#pragma once

#include "vec2.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class Camera {
public:
	Camera(Vec2 pos = Vec2()) {
		position = pos;
		scale = 0.5f;
	}

	Vec2 getPosition() {
		Vec2 p = position;
		p.x -= SCREEN_WIDTH / 2;
		p.y -= SCREEN_HEIGHT / 2;
		return p;
	}

	float getScale() {
		return scale;
	}

	void translate(float x, float y) {
		position.x += x;
		position.y += y;
	}

	void translate(Vec2Int delta) {
		translate(delta.x, delta.y);
	}

	void setPosition(float x, float y) {
		position.x = x;
		position.y = y;
	}

	void setPosition(Vec2Int pos) {
		setPosition(pos.x, pos.y);
	}

	void zoom(float delta) {
		scale += delta;
	}

	Vec2Int transform(Vec2Int pos) {
		pos.x *= scale;
		pos.y *= scale;
		pos.x -= position.x;
		pos.y -= position.y;
		return pos;
	}

	Vec2 transform(Vec2 pos) {
		pos.x *= scale;
		pos.y *= scale;
		pos.x -= getPosition().x;
		pos.y -= getPosition().y;
		return pos;
	}

private:
	Vec2 position;
	float scale;
};
