#pragma once

#include "ecs.h"
#include "level.h"

extern Level* level;

class MoveObjectSystem : public System {
public:

	MoveObjectSystem() {
		signature.set(ECS::getComponentId<TransformComponent>());
		signature.set(ECS::getComponentId<LevelObjectComponent>());
	}

	void init() override {
	}

	void update(float dt) override {
		for (Entity entity : entities) {
			if (!ECS::entityManager->isActive(entity)) continue;
			TransformComponent& transform = ECS::getComponent<TransformComponent>(entity);
			LevelObjectComponent levelObject = ECS::getComponent<LevelObjectComponent>(entity);

			transform.setPos(Vec2::moveTowards(transform.getPos(), level->getTilePos(levelObject.tile), dt * 600));
		}
	}

	void dealloc() override {
	}
};