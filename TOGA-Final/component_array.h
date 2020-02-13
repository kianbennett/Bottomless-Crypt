#pragma once

#include <vector>
#include <unordered_map>
#include "entity.h"

// Need an interface for ComponentArray so it can be stored in a list without providing a typename
class IComponentArray {
public:
	virtual void onEntityDestroyed(Entity entity) = 0;
};

template<typename Component>
class ComponentArray : public IComponentArray {
public:
	void insertComponent(Entity entity, Component component) {
		// Check no components from the entity have already been added
		if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
			printf("Cannot add more that one component of the same type\n");
			return;
		}

		size_t index = components.size();
		entityToIndexMap[entity] = index;
		indexToEntityMap[index] = entity;

		components.push_back(component);
	}

	void removeComponent(Entity entity) {
		// Check the component exists in the array
		if (entityToIndexMap.find(entity) == entityToIndexMap.end()) {
			printf("Cannot remove component as it doesn't exist!\n");
			return;
		}

		size_t index = entityToIndexMap[entity];
		size_t indexLast = components.size() - 1;

		// Move last component to the place of the removed component
		components[index] = components[indexLast];

		// Update the index maps
		Entity entityLast = indexToEntityMap[indexLast];
		entityToIndexMap[entityLast] = index;
		indexToEntityMap[index] = entityLast;

		// Remove entity from index maps
		entityToIndexMap.erase(entity);
		indexToEntityMap.erase(index);

		// Remove last component from vector (as it got already got placed in the removed position)
		components.pop_back();
	}

	Component& getComponent(Entity entity) {
		// Check the component exists in the array
		if (entityToIndexMap.find(entity) == entityToIndexMap.end()) {
			printf("Cannot get component as it doesn't exist!\n");
			return;
		}

		// Get component in array at the entity's index
		return components[entityToIndexMap[entity]];
	}

	void onEntityDestroyed(Entity entity) override {
		if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
			removeComponent(entity);
		}
	}

private:
	// Vectors are guaranteed to be contiguous so can use while maintaining cache-friendliness
	std::vector<Component> components;
	// Maps entities to the index of their component in the array
	// Use this instead of map<Entity, Component> to ensure memory is stored contiguously
	std::unordered_map<Entity, size_t> entityToIndexMap;
	// Maps indices in the array to the entity they belong to
	std::unordered_map<size_t, Entity> indexToEntityMap;
};