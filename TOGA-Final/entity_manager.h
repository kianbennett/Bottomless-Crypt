#pragma once

#include <vector>
#include "entity.h"

class EntityManager {
public:
	EntityManager();

	// Create a new entity id
	Entity::Id create();

	// Removes an entity id from the list
	void destroy(Entity::Id id);

	// Returns an entity id at index
	Entity::Id get(unsigned index);

	// Checks if generation of entity matches the generation at index
	bool isAlive(Entity::Id id);
	
	// Returns entities that can be stored without resizing the vector
	unsigned getSize();

	// Destroys all entities
	void clear();

private:
	// Incremented when a new entity is created and no ids are available from freeIds
	unsigned nextId;
	// List of entity ids that are available
	std::vector<Entity::Id> freeIds;
	// Incremented when an entity at that index is destroyed
	std::vector<unsigned> generations;
};