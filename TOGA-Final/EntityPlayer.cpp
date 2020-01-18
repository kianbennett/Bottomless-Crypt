#include "EntityPlayer.h"

EntityPlayer::EntityPlayer(float x, float y, float w, float h, vec4 colour) : Entity(x, y, w, h) {
	moveUp = moveDown = moveLeft = moveRight = false;
	renderer = new QuadRenderer(colour);
}

EntityPlayer::~EntityPlayer() {
	delete renderer;
}


void EntityPlayer::update(float dt) {
	Entity::update(dt);

	if (moveUp) transform->pos.y -= dt * speed;
	if (moveDown) transform->pos.y += dt * speed;
	if (moveLeft) transform->pos.x -= dt * speed;
	if (moveRight) transform->pos.x += dt * speed;
}

void EntityPlayer::render() {
	Entity::render();

	mat4 model = transform->getTransformation();
	renderer->render(model);
}