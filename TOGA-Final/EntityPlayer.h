#pragma once

#include "Entity.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>

class EntityPlayer : public Entity {
public:
	Renderer* renderer;
	bool moveUp, moveDown, moveLeft, moveRight;

	EntityPlayer(float x, float y, float w, float h, vec4 colour);
	~EntityPlayer();

	void update(float dt);
	void render();
private:
	const float speed = 300;
};