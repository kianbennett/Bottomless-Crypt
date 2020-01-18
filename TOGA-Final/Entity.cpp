#include "Entity.h"

Entity::Entity(float x, float y, float w, float h) {
	transform = new Transform(x, y, w, h, 0.0f);
}

Entity::~Entity() {
	delete transform;
}

void Entity::update(float dt) {
}

void Entity::render() {
}
