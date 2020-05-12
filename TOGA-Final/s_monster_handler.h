#pragma once

#include "ecs.h"

extern Level* level;

class MonsterHandlerSystem : public System {
public:

	MonsterHandlerSystem() {
		signature.set(ECS::getComponentId<TransformComponent>());
		signature.set(ECS::getComponentId<LevelObjectComponent>());
		signature.set(ECS::getComponentId<MonsterComponent>());
	}

	void init() override {
	}

	void update(float dt) override {
	}

	void dealloc() {
	}
};