#pragma once

#include <unordered_map>
#include <bitset>
#include <memory>
#include "system.h"
#include "types.h"

class ECS;

class SystemManager {
public:
	// Registers a system in the map
	template<typename System>
	std::shared_ptr<System> registerSystem() {
		const char* typeName = typeid(System).name();

		if (hasSystemBeenRegistered(typeName)) {
			printf("System has already been registered\n");
			return NULL;
		}

		std::shared_ptr<System> system = std::make_shared<System>();
		systems[typeName] = system;
		return system;
	}

	// Called when an entity is destroyed, goes through each system and removes that entity
	void onEntityDestroyed(Entity entity) {
		for (auto pair : systems) {
			std::shared_ptr<System> system = pair.second;
			// Entities is a set (unique values) so no exists check is needed
			int size = system->entities.size();
			system->entities.erase(entity);
			if (size != system->entities.size()) {
				system->onEntitiesChanged();
			}
		}
	}

	// Goes through each system and checks if the new signature matches and add/remove entities accordingly
	void onEntitySignatureChanged(Entity entity, Signature signature) {
		// Go through each system and check the signature matches the new signature
		for (auto pair : systems) {
			const char* typeName = pair.first;
			std::shared_ptr<System> system = pair.second;

			int size = system->entities.size();

			// Use AND operator to check if component signature has all the required bits for system
			// This way if the entity has more components than the system needs it will still be added
			// If the signatures match add the entity to the system, otherwise remove the entity
			if ((signature & system->signature) == system->signature) {
				system->entities.insert(entity);
			}
			else {
				system->entities.erase(entity);
			}
	
			// If the set insert/erase was successful
			if (size != system->entities.size()) {
				system->onEntitiesChanged();
			}
		}
	}

private:
	// Store systems in a map with the key as the string representation of the type name
	std::unordered_map<const char*, std::shared_ptr<System>> systems;

	bool hasSystemBeenRegistered(const char* typeName) {
		return systems.find(typeName) != systems.end();
	}
};