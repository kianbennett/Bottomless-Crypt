#pragma once

#include "entity_manager.h"
#include "component_manager.h"

class World {
public:
	static ComponentManager* componentManager;
	static EntityManager* entityManager;

	void init();
	void update(float delta);
	void cleanup();
};
