#pragma once

struct TransformComponent {
	float x, y;
	float rot;
	float scaleX, scaleY;

	TransformComponent(float x = 0.0f, float y = 0.0f, float rot = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f) : 
		x(x), y(y), rot(rot), scaleX(scaleX), scaleY(scaleY) {}
};