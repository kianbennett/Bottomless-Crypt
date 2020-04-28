#include "entity_manager.h"

EntityManager::EntityManager() {
	nextId = 0;
}

Entity EntityManager::create() {
	Entity id;

	if (!freeEntities.empty()) {
		// If there are ids not being used then use (and remove) the next in the queue
		id = freeEntities.front();
		freeEntities.pop();
		id.generation++;
	}
	else {
		// Otherwise increment the index and set the generation to 1 (0 is destroyed entity)
		id.index = nextId++;
		id.generation = 1;
		generations.push_back(1);
		signatures.push_back(Signature());
		activeEntities.push_back(true);
	}

	return id;
}

void EntityManager::destroy(Entity entity) {
	// Increment the generation at the id's index and add to the list of ids that can be reused
	signatures[entity.index] = {};
	generations[entity.index]++;
	freeEntities.push(entity);
}

Entity EntityManager::get(unsigned index) {
	unsigned generation = 0;
	if (index < generations.size()) {
		generation = generations[index];
	}
	// If the entity isn't in the list then return with generation = 0 (destroyed)
	return Entity { index, 0 };
}

bool EntityManager::isAlive(Entity id) {
	if (id.index < generations.size()) {
		return id.generation > 0 && id.generation == generations[id.index];
	}
	return false;
}

size_t EntityManager::getSize() {
	return generations.size();
}

void EntityManager::clear() {
	generations.clear();
	freeEntities = {};
	signatures = {};
	activeEntities = {};
	nextId = 0;
}

Signature& EntityManager::getSignature(Entity entity) {
	return signatures[entity.index];
}

bool EntityManager::isActive(Entity entity) {
	return activeEntities[entity.index];
}

void EntityManager::setActive(Entity entity, bool active) {
	activeEntities[entity.index] = active;
}

std::vector<unsigned>& EntityManager::getGenerations() {
	return generations;
}
