#pragma once

#include <vector>
#include <queue>
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

	Signature& getSignature(Entity entity);

	bool isActive(Entity entity);
	void setActive(Entity entity, bool active);

	// Accessor to display entity list in HUD
	std::vector<unsigned>& getGenerations();

private:
	// Incremented when a new entity is created and no ids are available from freeIds
	unsigned nextId;
	// List of entity ids that are available
	std::queue<Entity> freeEntities;
	// Incremented when an entity at that index is destroyed
	std::vector<unsigned> generations;

	// TODO: Come up with a way to include these within entity struct?
	std::vector<Signature> signatures;
	std::vector<bool> activeEntities;
};