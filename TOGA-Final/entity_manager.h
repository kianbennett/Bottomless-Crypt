#pragma once

#include <vector>
#include "entity.h"

class EntityManager {
public:
	EntityManager();

	// Create a new entity
	Entity create();

	// Removes an entity from the list
	void destroy(Entity entity);

	// Returns an entity id at index
	Entity get(unsigned index);

	// Checks if generation of entity matches the generation at index
	bool isAlive(Entity entity);
	
	// Returns entities that can be stored without resizing the vector
	size_t getSize();

	// Destroys all entities
	void clear();

private:
	// Incremented when a new entity is created and no ids are available from freeIds
	unsigned nextId;
	// List of entity ids that are available
	std::vector<Entity> freeEntities;
	// Incremented when an entity at that index is destroyed
	std::vector<unsigned> generations;
};