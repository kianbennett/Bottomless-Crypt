#pragma once

#include "Transform.h"

class Entity {
public:
	Transform* transform;

	Entity(float x, float y, float w, float h);
	~Entity();

	virtual void update(float dt);
	virtual void render();
};