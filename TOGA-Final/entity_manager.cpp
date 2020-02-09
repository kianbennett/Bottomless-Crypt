#include "entity_manager.h"

EntityManager::EntityManager() {
	nextId = 0;
}

Entity::Id EntityManager::create() {
	Entity::Id id;

	if (!freeIds.empty()) {
		// If there are ids not being used then use (and remove) the last one
		id = freeIds.back();
		freeIds.pop_back();
	}
	else {
		// Otherwise increment the index and set the generation to 1 (0 is destroyed entity)
		id.index = nextId++;
		id.generation = 1;
		generations.push_back(1);
	}

	return id;
}

void EntityManager::destroy(Entity::Id id) {
	// Increment the generation at the id's index and add to the list of ids that can be reused
	generations[id.index]++;
	freeIds.push_back(id);
}

Entity::Id EntityManager::get(unsigned index) {
	unsigned generation = 0;
	if (index < generations.size()) {
		generation = generations[index];
	}
	// If the entity isn't in the list then return with generation = 0 (destroyed)
	return Entity::Id{ index, 0 };
}

bool EntityManager::isAlive(Entity::Id id) {
	if (id.index < generations.size()) {
		return id.generation > 0 && id.generation == generations[id.index];
	}
	return false;
}

unsigned EntityManager::getSize() {
	return generations.size();
}

void EntityManager::clear() {
	generations.clear();
	freeIds.clear();
	nextId = 0;
}